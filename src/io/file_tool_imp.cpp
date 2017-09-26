

#include <windows.h>
#include "file_tool_imp.h"

#ifndef MAX_PATH
#define MAX_PATH 1024
#endif

namespace kge
{
	FileSystemImp::FileSystemImp()
	{

	}

    FileSystemImp::~FileSystemImp()
    {

    }

    bool FileSystemImp::init()
    {
        _defaultPath = getModulePath();

        return IFileSystem::init();
    }
    
    std::string FileSystemImp::getCurrentPath()
    {
		TCHAR buffer[MAX_PATH];
		::GetCurrentDirectory(MAX_PATH, buffer);
		
		return buffer;
    }
    
    /** ios & android，返回是res路径，其他平台返回app所在路径 */
    std::string FileSystemImp::getModulePath()
    {
		TCHAR buffer[MAX_PATH];
		::GetModuleFileName(NULL, buffer, MAX_PATH);
        
		return GetFilePath(buffer);
    }
    
    std::string FileSystemImp::getWritablePath()
    {
		return getModulePath();
    }
    
    
    bool FileSystemImp::createDir(const std::string & fullPath)
    {
    	return false;
    }

    bool FileSystemImp::deleteDir(const std::string & fullPath)
    {
    	return false;
    }
    
    bool FileSystemImp::existFile(const std::string & fullPath)
    {
    	if (fullPath.empty())
	        return false;
	    
		return ::GetFileAttributes(fullPath.c_str()) != -1;
    }

    bool FileSystemImp::deleteFile(const std::string & fullPath)
    {
    	return false;
    }

    bool FileSystemImp::renameFile(const std::string & fullPath)
    {
    	return false;
    }

} // end namespace kge
