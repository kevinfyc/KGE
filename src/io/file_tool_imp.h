#ifndef FILE_TOOL_IMP_H
#define FILE_TOOL_IMP_H

#include "file_tool.h"

namespace kge
{
    class FileSystemImp : public IFileSystem
    {
    public:
        FileSystemImp();
        ~FileSystemImp();

        virtual bool init() override;
        
        virtual std::string getCurrentPath() override;
        
        /** ios & android，返回是res路径，其他平台返回app所在路径 */
        virtual std::string getModulePath() override;
        
        virtual std::string getWritablePath() override;
        
        //@{ 以下几个方法，参数必须是绝对路径
        virtual bool createDir(const std::string & fullPath) override;
        virtual bool deleteDir(const std::string & fullPath) override;
        
        virtual bool existFile(const std::string & fullPath) override;
        virtual bool deleteFile(const std::string & fullPath) override;
        virtual bool renameFile(const std::string & fullPath) override;
        //@}
    };
}

#endif //FILE_TOOL_IMP_H
