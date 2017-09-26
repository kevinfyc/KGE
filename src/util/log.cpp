#include "log.h"

#include <cstdarg>
#include <string>

#ifdef WIN32
#include <Windows.h>
#include <iostream>
#else
#include <iostream>
#endif
#include "string_tool.h"

namespace kge
{
	namespace LogPrefix
	{
		const std::string Trace = "Trace: ";
		const std::string Debug = "DEBUG: ";
		const std::string Info = "INFO : ";
		const std::string Warn = "WARN : ";
		const std::string Error = "ERROR: ";
		const std::string Fatal = "FATAL: ";

		const std::string & GetLogPrefix(int lvl)
		{
			if (lvl >= KGE_LOG_LVL_FATAL) return Fatal;
			if (lvl >= KGE_LOG_LVL_ERROR) return Error;
			if (lvl >= KGE_LOG_LVL_WARN) return Warn;
			if (lvl >= KGE_LOG_LVL_INFO) return Info;
			if (lvl >= KGE_LOG_LVL_DEBUG) return Debug;
			return Trace;
		}
	}

	void kge_log(int lvl, const char* format, ...)
	{
		std::string msg;

		va_list va;
		va_start(va, format);

		StringFromVAList(msg, format, va);

		va_end(va);

		const std::string & prefix = LogPrefix::GetLogPrefix(lvl);
		msg.insert(0, prefix);

#ifdef WIN32
		OutputDebugString((msg + "\r").c_str());
		std::cout << msg << std::endl;
#elif defined(ANDROID)
		__android_log_print(logLvlOrangeToAndroid(lvl), "orange", "%s", msg.c_str());
#else
		std::cout << msg << std::endl;
#endif

		if (lvl >= KGE_LOG_LVL_FATAL)
		{
			abort();
		}
	}

} // end of namespace TVR