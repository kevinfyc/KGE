#include "file_tool.h"
#include "util/log.h"

namespace kge
{

    bool ReadFile(Content & content, const std::string & filename, bool isBinary)
    {
        IFile::Mode mode = IFile::Mode(IFile::ModeRead | (isBinary ? IFile::ModeBinary : 0));
        Ref<IFile> file = FileSystemMgr::Instance()->FileSystem()->openFile(filename, mode);
        if (!file)
        {
            KGE_LOG_ERROR("Failed to open file '%s'", filename.c_str());
            return false;
        }

        content.Resize(file->length());
        if (!content.Empty())
        {
            file->read(content.CData(), content.CSize());
        }
        
        return true;
    }

	Ref<ByteBuffer> ReadBinary(const std::string & filename)
    {
		Content content;
        if (!ReadFile(content, filename, true))
			return nullptr;

        return Ref<ByteBuffer>(content.CBuffer());
    }

    bool WriteFile(const Ref<Content> & content, const std::string & filename, bool isBinary)
    {
        IFile::Mode mode = IFile::Mode(IFile::ModeWrite | (isBinary ? IFile::ModeBinary : 0));
		Ref<IFile> file = FileSystemMgr::Instance()->FileSystem()->openFile(filename, mode);
        if (!file)
        {
			KGE_LOG_ERROR("Failed to create file '%s'", filename.c_str());
            return false;
        }

        if (!content->Empty())
        {
            file->write(content->CData(), content->CSize());
        }
        
        return true;
    }

    bool WriteBinary(const std::string & filename, Ref<ByteBuffer> block)
    {
        assert(block);

		Ref<Content> content = Ref<Content>(new Content(block.get()));
        return WriteFile(content, filename, true);
    }

    
    ////////////////////////////////////////////////////////////////////////////
    ///
    ////////////////////////////////////////////////////////////////////////////
    IFile::IFile()
    {
    }
    
    IFile::~IFile()
    {
    }
    
    class FileImp : public IFile
    {
        FILE   *_pFile;
        size_t  _length;
        Mode    _mode;
    public:
        
        FileImp(FILE * p, Mode mode)
            : _pFile(p)
            , _length(-1)
            , _mode(mode)
        {}
        
        ~FileImp()
        {
            close();
        }
        
        size_t length()
        {
            if(_length == -1)
            {
                size_t cur = tell();
                seek(0, SeekEnd);
                _length = tell();
                seek(cur, SeekBeg);
            }
            return _length;
        }
        
        size_t tell()
        {
            assert(_pFile);
            
            return size_t(ftell(_pFile));
        }
        
        void seek(size_t pos, Seek seekMode)
        {
            assert(_pFile);
            
            int seek_mode = SEEK_CUR;
            if(seekMode == SeekBeg) seek_mode = SEEK_SET;
            else if(seekMode == SeekEnd) seek_mode = SEEK_END;
            
            fseek(_pFile, pos, seek_mode);
        }
        
        size_t read(void * buffer, size_t n)
        {
            assert(_pFile);
            
            if(_mode & ModeRead)
            {
                return fread(buffer, 1, n, _pFile);
            }
            else
            {
                assert("this file does't readable!");
                return 0;
            }
        }
        
        size_t write(const void * buffer, size_t n)
        {
            assert(_pFile);
            
            if(_mode & ModeWrite)
            {
                return fwrite(buffer, 1, n, _pFile);
            }
            else
            {
                assert("this file does't writable!");
                return 0;
            }
        }
        
        void flush()
        {
            if(_pFile != nullptr)
            {
                fflush(_pFile);
            }
        }
        
        void close()
        {
            if(_pFile != nullptr)
            {
                fclose(_pFile);
                _pFile = nullptr;
            }
        }
    };
    
    ////////////////////////////////////////////////////////////////////////////
    ///
    ////////////////////////////////////////////////////////////////////////////
    
    IFileSystem::IFileSystem()
    {
    }
    
    IFileSystem::~IFileSystem()
    {
    }
    
    bool IFileSystem::init()
    {
        KGE_LOG_INFO("default path:  %s", _defaultPath.c_str());
		KGE_LOG_INFO("current path:  %s", getCurrentPath().c_str());
		KGE_LOG_INFO("module path:   %s", getModulePath().c_str());
		KGE_LOG_INFO("writable path: %s", getWritablePath().c_str());
        return true;
    }
    
    bool IFileSystem::isFileExist(const std::string & filename)
    {
        return !((getFullPath(filename)).empty());
    }
    
    std::string IFileSystem::getFullPath(const std::string & filename)
    {
        std::string destPath;
        for(const std::string & path : _paths)
        {
            destPath = path + filename;
            
            if(existFile(destPath)) return destPath;
        }
        
        destPath.clear();
        return destPath;
    }
    
    std::string IFileSystem::getRelativePath(const std::string & fullPath)
    {
    	std::string destPath;
        for(const std::string & path : _paths)
        {
            if(StringBeginWith(fullPath, path))
                return fullPath.substr(path.length());
        }
        
        destPath.clear();
        return destPath;
    }
    
    void IFileSystem::AddSearchPath(const std::string & path)
    {
        std::string relPath = _defaultPath + path;
        FormatFilePath(relPath);
        
        _paths.push_back(relPath);
        
		KGE_LOG_INFO("add search path: %s", relPath.c_str());
    }
    
    void IFileSystem::setSearchPath(const StringVector & paths)
    {
        _paths.clear();
        
        for(const std::string & path : paths)
        {
            AddSearchPath(path);
        }
    }
    
    Ref<IFile> IFileSystem::openFile(const std::string & filename, IFile::Mode mode)
    {
        std::string fullPath;
        std::string strMode;
        if(mode & IFile::ModeRead)
        {
            fullPath = getFullPath(filename);
            if(fullPath.empty())
            {
                KGE_LOG_ERROR("Failed to open file. Can't find file '%s'", filename.c_str());
                return nullptr;
            }
            
            strMode += "r";
            if(mode & IFile::ModeBinary)
                strMode += "b";
            if(mode & IFile::ModeWrite)
                strMode += "+";
        }
        else if(mode & IFile::ModeWrite)
        {
            fullPath = getWritablePath() + filename;
            
            strMode += "w";
            if(mode & IFile::ModeBinary)
                strMode += "b";
            
            //KGE_LOG_INFO("Write File: %s", fullPath.c_str());
        }
        else
        {
            return nullptr;
        }
        
        FILE *pFile = fopen(fullPath.c_str(), strMode.c_str());
        if(!pFile) return nullptr;
        
        return Ref<IFile>(new FileImp(pFile, mode));
    }
    
    ////////////////////////////////////////////////////////////////////////////
    ///
    ////////////////////////////////////////////////////////////////////////////
    IMPLEMENT_SINGLETON(FileSystemMgr);
    
    FileSystemMgr::FileSystemMgr()
    {
    }
    
    FileSystemMgr::~FileSystemMgr()
    {
    }

}// end namespace ora
