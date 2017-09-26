//
//  singleton.h
//  dydg_b
//
//  Created by kevin on 17-9-25.
//
//

#ifndef __KGE_UTIL_SINGLETON_H__
#define __KGE_UTIL_SINGLETON_H__

#include <cassert>

namespace kge
{
	//需要使用单例类，必须从Singleton派生，并且用宏IMPLEMENT_SINGLETON，完成定义。
	template<typename T>
	class Singleton
	{
	public:

		Singleton()
		{
			assert(s_pInstance == nullptr && "Singleton Class can has only one instance!");

			s_pInstance = static_cast<T*>(this);
		}

		~Singleton()
		{
			if (s_pInstance == this)
				s_pInstance = nullptr;
		}

		static void InitInstance()
		{
			assert(s_pInstance == nullptr);

			new T();
		}

		static void FiniInstance()
		{
			if (!s_pInstance) return;

			delete s_pInstance;
			s_pInstance = nullptr;
		}

		static bool HasInstance()
		{
			return s_pInstance != nullptr;
		}

		inline static T * GetInstance() { return s_pInstance; }

		inline static T * Instance()
		{
			assert(s_pInstance && "Singleton<T>::instance");

			return s_pInstance;
		}

	private:
		static T * s_pInstance;
	};

} // end namespace kge

#define IMPLEMENT_SINGLETON(T) \
template<> T * ::kge::Singleton<T>::s_pInstance = nullptr

#endif //__KGE_UTIL_SINGLETON_H__
