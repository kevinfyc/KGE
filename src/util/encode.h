#pragma once

#include <Windows.h>
#include <tchar.h>
#include <string>

namespace kge
{
	typedef std::string tstring;
	
	//default encode is utf-8
	extern DWORD defaultCodePage;

    namespace Encode
    {
        const tstring gbk = _T("gbk");
        const tstring big5 = _T("big5");
        const tstring utf7 = _T("utf-7");
        const tstring utf8 = _T("utf-8");
        const tstring ucs2 = _T("ucs2");
        const tstring ucs_2le = _T("ucs-2le");
        const tstring ucs_2be = _T("ucs-2be");
        const tstring None = _T("");
    }

    namespace CP
    {
        const size_t None = 0;
        const size_t gbk = 936;
        const size_t big5 = 950;
        const size_t utf7 = 65000;
        const size_t utf8 = 65001;
        const size_t ucs_2le = 1200;
        const size_t ucs_2be = 1201;
        const size_t ucs2 = ucs_2le;
    }

    //编码与编码页转换
    const tstring & cpToCode(tstring & code, size_t cp);
    size_t codeToCp(const tstring & code);


    //多字节字符串转换成宽字符字符串
    std::wstring charToWChar(const std::string & str, DWORD code=defaultCodePage);

    //宽字符串转换成多字节字符串
    std::string wcharToChar(const std::wstring & wstr, DWORD code=defaultCodePage);

    //多字节字符串转换成宽字符字符串
    bool charToWChar(std::wstring & dest, const std::string & str, DWORD code=defaultCodePage);

    //宽字符串转换成多字节字符串
    bool wcharToChar(std::string & dest, const std::wstring & wstr, DWORD code=defaultCodePage);

    //////////////////////////////////////////////////////////////////////////
    //格式化字符串
    //////////////////////////////////////////////////////////////////////////


    bool formatStringVSA(std::string & dest, LPCSTR pFormat, va_list pArgList);

    bool formatStringVSW(std::wstring & dest, LPCWSTR pFormat, va_list pArgList);

    bool formatStringA(std::string & dest, LPCSTR pFormat, ...);

    bool formatStringW(std::wstring & dest, LPCWSTR pFormat, ...);

    void debugMessageA(LPCSTR pFormat, ...);

    void debugMessageW(LPCWSTR pFormat, ...);


#ifdef _UNICODE
#   define trimString				::Lazy::trimStringW
#	define formatString				::Lazy::formatStringW
#	define formatStringVS			::Lazy::formatStringVSW
#	define debugMessage				::Lazy::debugMessageW
#else
#   define trimString				::Lazy::trimStringA
#	define formatString				::Lazy::formatStringA
#	define formatStringVS			::Lazy::formatStringVSA
#	define debugMessage				::Lazy::debugMessageA
#endif

}// end namespace ora

