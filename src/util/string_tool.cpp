﻿//
//  string_tool.cpp
//  libkge
//
//  Created by kevin on 14-4-18.
//

#include "string_tool.h"
#include <algorithm>
#include <cassert>
#include <cstdarg>
#include "math\_math.hpp"

namespace kge
{

	bool IsWhiteSpace(char ch)
	{
		return ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n' || ch == '\0';
	}

	void TrimString(std::string & str)
	{
		if (str.empty()) return;

		int length = int(str.size());

		int p1 = 0;
		for (; p1 < length; ++p1)
		{
			if (!IsWhiteSpace(str[p1])) break;
		}

		int p2 = length - 1;
		for (; p2 >= 0; --p2)
		{
			if (!IsWhiteSpace(str[p2])) break;
		}

		if (p1 > p2)
		{
			str.clear();
			return;
		}

		int n = p2 - p1 + 1;
		if (p1 != 0)
		{
			for (int i = 0; i < n; ++i)
			{
				str[i] = str[p1 + i];
			}
		}

		str.erase(n, str.npos);
	}

	void LowerString(std::string& str)
	{
		std::vector<char> tmp;

		for (auto c : str)
		{
			if (c >= 'A' && c <= 'Z')
			{
				c -= 'A' - 'a';
			}
			tmp.push_back(c);
		}
		tmp.push_back(0);

		str = (&tmp[0]);
	}

	void UpperString(std::string& str)
	{
		std::vector<char> tmp;

		for (auto c : str)
		{
			if (c >= 'A' && c <= 'Z')
			{
				c += 'A' - 'a';
			}
			tmp.push_back(c);
		}
		tmp.push_back(0);

		str = (&tmp[0]);
	}

	void ToUnicode32(const std::string& str, std::vector<char32_t>& ret)
	{
		uint32 size = str.size();

		for (uint32 i = 0; i < size; ++i)
		{
			char32_t unicode32 = 0;
			uint32 byte_count = utf82unicode32(&str[i], unicode32);

			if (byte_count > 0)
			{
				ret.push_back(unicode32);
				//i += byte_count - 1;
			}
			else
			{
				break;
			}
		}
	}

	std::string RemoveFileExt(const std::string & name)
	{
		size_t pos = name.find_last_of('.');
		if (pos != name.npos)
		{
			return name.substr(0, pos);
		}

		return name;
	}

	std::string GetFileExt(const std::string & name)
	{
		size_t pos = name.find_last_of('.');
		if (pos != name.npos)
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
		if (path.empty()) return;

		FormatPathSlash(path);
		if (path.back() != '/') path += '/';
	}

	std::string GetFilePath(const std::string & filename)
	{
		if (filename.empty())
			return std::string();

		std::string name = filename;
		FormatPathSlash(name);
		if (name.back() == '/')
			name.pop_back();

		size_t pos = name.find_last_of('/');
		if (pos == std::string::npos)
			return std::string();

		return name.substr(0, pos + 1);
	}

	std::string GetFileName(const std::string & filename)
	{
		if (filename.empty())
			return std::string();

		std::string name = filename;
		FormatPathSlash(name);
		if (name.back() == '/')
			name.pop_back();

		size_t pos = name.find_last_of('/');
		if (pos == std::string::npos)
			return name;

		return name.substr(pos + 1);
	}

	bool StringFromFormat(std::string & out, const char * format, ...)
	{
		assert(format != nullptr);

		va_list pArgList;
		va_start(pArgList, format);

		bool ret = StringFromVAList(out, format, pArgList);

		va_end(pArgList);

		return ret;
	}

	bool StringFromVAList(std::string & out, const char * format, va_list va)
	{
		int len = 128;
		int ret = -1;

		while (ret < 0)
		{
			len <<= 1;

			out.resize(len);
			ret = vsnprintf(&out[0], len, format, va);

			//mac上，如果长度不够长，会返回需要的长度
			if (ret > len)
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
		if (str.length() < prefix.length()) return false;

		for (size_t i = 0; i < prefix.length(); ++i)
		{
			if (str[i] != prefix[i]) return false;
		}

		return true;
	}

	bool StringEndWith(const std::string & str, const std::string & postfix)
	{
		if (str.length() < postfix.length()) return false;

		size_t offset = str.length() - postfix.length();
		for (size_t i = 0; i < postfix.length(); ++i)
		{
			if (str[i + offset] != postfix[i]) return false;
		}

		return true;
	}


	bool StringSplit(const std::string& res, const std::string& sep, StringVector& ret, bool exclude_empty)
	{
		uint32 start = 0;
		while (true)
		{
			uint32 index = res.find(sep, start);
			if (index >= 0)
			{
				std::string str = res.substr(start, index - start);
				if (!str.empty() || !exclude_empty)
					ret.push_back(str);
				start = index + sep.size();
			}
			else
			{
				break;
			}
		}

		std::string str = res.substr(start, -1);
		if (!str.empty() || !exclude_empty)
			ret.push_back(str);

		return ret.size() > 0;
	}


	OutputNChar::OutputNChar(char ch, int nRepeat)
		: _ch(ch)
		, _nRepeat(nRepeat)
	{
	}

	std::ostream & operator<<(std::ostream & stream, const OutputNChar & output)
	{
		int n = output._nRepeat;
		while (n-- > 0) stream << output._ch;

		return stream;
	}


	TabProxy::TabProxy(int n)
		: _n(n)
	{}


	std::ostream & operator << (std::ostream & stream, const TabProxy & tab)
	{
		int n = tab._n;
		while (n-- > 0) stream << '\t';

		return stream;
	}

	std::vector<char> unicode322utf8(char32_t c32)
	{
		std::vector<char> buffer;
		uint32 byte_count = 0;

		if (c32 <= 0x7f)
			byte_count = 1;
		else if (c32 <= 0x7ff)
			byte_count = 2;
		else if (c32 <= 0xffff)
			byte_count = 3;
		else if (c32 <= 0x1fffff)
			byte_count = 4;
		else if (c32 <= 0x3ffffff)
			byte_count = 5;
		else if (c32 <= 0x7fffffff)
			byte_count = 6;

		std::vector<char> bytes;
		for (uint32 i = 0; i < byte_count - 1; i++)
		{
			bytes.push_back((c32 & 0x3f) | 0x80);
			c32 >>= 6;
		}

		if (byte_count > 1)
			bytes.push_back((char)(c32 | (0xffffff80 >> (byte_count - 1))));
		else
			bytes.push_back((char)(c32));

		for (uint32 i = 0; i < byte_count; i++)
			buffer.push_back(bytes[byte_count - 1 - i]);

		return buffer;
	}

	uint32 utf82unicode32(const char* utf8, char32_t& c32)
	{
		uint32 byte_count = 0;

		for (uint32 i = 0; i < 8; i++)
		{
			uint8 c = utf8[0];

			if (((c << i) & 0x80) == 0)
			{
				byte_count = i == 0 ? 1 : i;
				break;
			}
		}

		if (byte_count >= 1 && byte_count <= 6)
		{
			char32_t code = 0;

			for (uint32 i = 0; i < byte_count; i++)
			{
				uint32 c = utf8[i];
				uint8 part = i == 0 ? (c << (byte_count + 24)) >> (byte_count + 24) : (c & 0x3f);
				code = (code << 6) | part;
			}

			c32 = code;

			return byte_count;
		}
		else
		{
			return 0;
		}
	}
}