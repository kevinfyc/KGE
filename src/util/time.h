//
//  time.h
//  libkge
//
//  Created by kevin on 17-9-23.
//
//

#ifndef __KGE_UTIL_TIME_H__
#define __KGE_UTIL_TIME_H__

namespace kge
{
	class Time
	{
	public:
		static float GetTime();
		static long long GetTimeMS();

	private:
		static long long _time_startup;
	};
} // end of namespace kge

#endif // __KGE_UTIL_TIME_H__