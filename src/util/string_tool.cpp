//
//  string_tool.cpp
//  libkge
//
//  Created by kevin on 14-4-18.
//

#include "string_tool.h"
#include <algorithm>
#include <cassert>
#include <cstdarg>

namespace kge
{

    bool IsWhiteSpace(char ch)
    {
        return ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n' || ch == '\0';
    }

    void TrimString(std::string & str)
    {
        if(str.empty()) return;

        int length = int(str.size());

        int p1 = 0;
        for(; p1 < length; ++p1)
        {
            if(!IsWhiteSpace(str[p1])) break;
        }

        int p2 = length - 1;
        for(; p2 >= 0; --p2)
        {
            if(!IsWhiteSpace(str[p2])) break;
        }

        if(p1 > p2)
        {
            str.clear();
            return;
        }

        int n = p2 - p1 + 1;
        if(p1 != 0)
        {
            for(int i = 0; i < n; ++i)
            {
                str[i] = str[p1 + i];
            }
        }

        str.erase(n, str.npos);
    }

    std::string RemoveFileExt(const std::string & name)
    {
        size_t pos = name.find_last_of('.');
        if(pos != name.npos)
        {
            return name.substr(0, pos);
        }

        return name;
    }

    std::string GetFileExt(const std::string & name)
    {
        size_t pos = name.find_last_of('.');
        if(pos != name.npos)
        {
            return name.substr(pos);
        }

        return "";
    }

    void FormatPathSlash(std::string & path)
    {
        std::replace(path.begin(), path.end(), '\\', '/');
    }

    void FormatFilePath(std::string & path)
    {
        if(path.empty()) return;

        FormatPathSlash(path);
        if(path.back() != '/') path += '/';
    }

    std::string GetFilePath(const std::string & filename)
    {
        if(filename.empty())
            return std::string();

        std::string name = filename;
        FormatPathSlash(name);
        if(name.back() == '/')
            name.pop_back();

        size_t pos = name.find_last_of('/');
        if(pos == std::string::npos)
            return std::string();

        return name.substr(0, pos + 1);
    }

    std::string GetFileName(const std::string & filename)
    {
        if(filename.empty())
            return std::string();

        std::string name = filename;
        FormatPathSlash(name);
        if(name.back() == '/')
            name.pop_back();

        size_t pos = name.find_last_of('/');
        if(pos == std::string::npos)
            return name;

        return name.substr(pos + 1);
    }
    
    bool StringFromFormat(std::string & out, const char * format, ...)
    {
        assert(format != nullptr);
        
        va_list pArgList;
        va_start (pArgList, format);
        
        bool ret = StringFromVAList(out, format, pArgList);
        
        va_end(pArgList);
        
        return ret;
    }
    
    bool StringFromVAList(std::string & out, const char * format, va_list va)
    {
        int len = 128;
        int ret = -1;
        
        while(ret < 0)
        {
            len <<= 1;
            
            out.resize(len);
            ret = vsnprintf(&out[0], len, format, va);
            
            //mac上，如果长度不够长，会返回需要的长度
            if(ret > len)
            {
                len = (ret >> 1) + 1;
                ret = -1;
            }
            
            assert(len < 65535 && "stringFromVAList");
        }
        
        out.erase(ret);
        return true;
    }
    
    bool StringBeginWith(const std::string & str, const std::string & prefix)
    {
        if(str.length() < prefix.length()) return false;
        
        for(size_t i = 0; i < prefix.length(); ++i)
        {
            if(str[i] != prefix[i]) return false;
        }
        
        return true;
    }
    
    bool StringEndWith(const std::string & str, const std::string & postfix)
    {
        if(str.length() < postfix.length()) return false;
        
        size_t offset = str.length() - postfix.length();
        for(size_t i = 0; i < postfix.length(); ++i)
        {
            if(str[i + offset] != postfix[i]) return false;
        }
        
        return true;
    }



    OutputNChar::OutputNChar(char ch, int nRepeat)
        : _ch(ch)
        , _nRepeat(nRepeat)
    {
    }

    std::ostream & operator<<(std::ostream & stream, const OutputNChar & output)
    {
        int n = output._nRepeat;
        while(n-- > 0) stream << output._ch;

        return stream;
    }


    TabProxy::TabProxy(int n)
        : _n(n)
    {}


    std::ostream & operator << (std::ostream & stream, const TabProxy & tab)
    {
        int n = tab._n;
        while(n-- > 0) stream << '\t';

        return stream;
    }
}