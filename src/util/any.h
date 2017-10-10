//
//  any.h
//  libkge
//
//  Created by kevin on 17-10-10.
//
//
// 

#ifndef __KGE_UTIL_ANY_H__
#define __KGE_UTIL_ANY_H__

namespace kge
{
	class Any
	{
	public:
		template<class T>
		Any(const T& v) :_value(new Type<T>(v))
		{

		}

		~Any()
		{
			delete _value;
			_value = nullptr;
		}

		template<class T>
		const T& Get()const
		{
			return ((Type<T>*)_value)->v;
		}

		template<class T>
		T& Get()
		{
			return ((Type<T>*)_value)->v;
		}

		template<class T>
		void Set(const T& v)
		{
			((Type<T>*)_value)-> = v;
		}

	private:
		struct Base
		{
			virtual ~Base()
			{

			}
		};

		template<class T>
		struct Type :public Base
		{
			Type(const T& _v):v(_v)
			{

			}

			virtual ~Type()
			{

			}

			T v;
		};

		Base* _value;
	};
} // end of namespace kge

#endif // __KGE_UTIL_ANY_H__