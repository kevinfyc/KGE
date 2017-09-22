//
//  object.h
//  libkge
//
//  Created by kevin on 17-9-22.
//
//

#ifndef __KGE_CORE_OBJECT_H__
#define __KGE_CORE_OBJECT_H__

#include "pre_define.h"

#define DECLARE_COM_BASE(base)																				\
public:																										\
	typedef void* (*gen)();																					\
	static base* Create(const std::string& class_name)														\
	{																										\
		return (base*)GetClassPool()[class_name]();															\
	}																										\
	static std::string ClassName(){return #base;}															\
	static const std::vector<std::string>& ClassNames()														\
	{																										\
		if(_class_names.empty())																			\
		{																									\
			_class_names.push_back(#base);																	\
		}																									\
		return _class_names;																				\
	}																										\
	virtual std::string GetTypeName() const { return #base; }												\
		virtual const std::vector<std::string>& GetClassNames() const { return base::ClassNames(); }		\
	protected:																								\
		static void Register(const std::string& class_name, gen class_gen)									\
		{																									\
			GetClassPool()[class_name] = class_gen;															\
		}																									\
    private:																								\
        static std::map<std::string, gen>& GetClassPool()													\
		{																									\
            return _class_pool;																				\
        }																									\
        static std::map<std::string, gen> _class_pool;														\
		static std::vector<std::string> _class_names;

#define DEFINE_COM_BASE(base)								\
    std::map<std::string, base::gen> base::_class_pool;	\
	std::vector<std::string> base::_class_names;

#define DECLARE_COM_CLASS(derived, super) \
    public: \
		static void RegisterComponent() \
		{ \
			Register(#derived, derived::Create); \
		} \
		static const std::string& ClassName() { return #derived; } \
		static const std::vector<std::string>& ClassNames() { \
			if(_class_names.empty()) { \
				_class_names = super::ClassNames(); \
				_class_names.push_back(#derived); \
			} \
			return _class_names; \
		} \
        virtual std::string GetTypeName() const { return #derived; } \
		virtual const std::vector<std::string>& GetClassNames() const { return derived::ClassNames(); } \
    private: \
        static void* Create() \
		{ \
            return new derived(); \
        } \
		static std::ector<std::string> _class_names;

#define DECLARE_COM_CLASS_ABSTRACT(derived, cuper) \
    public: \
		static std::string ClassName() { return #derived; } \
		static const std::vector<std::string>& ClassNames() { \
			if(_class_names.Empty()) { \
				_class_names = super::ClassNames(); \
				_class_names.push_back(#derived); \
			} \
			return _class_names; \
		} \
        virtual std::string GetTypeName() const { return #derived; } \
		virtual const std::vector<std::string>& GetClassNames() const { return derived::ClassNames(); } \
    private: \
		static std::vector<std::string> _class_names;

#define DEFINE_COM_CLASS(derived) \
    std::vector<std::tring> derived::_class_names;


namespace kge
{
	class Object
	{
	public:
		Object();
		virtual ~Object();

		inline void SetName(const std::string& name) { _name = name; }
		inline const std::string& GetName() { return _name; }

	protected:
		Object(const Object&) {}
		Object& operator = (const Object& obj) { return *this; }

	private:
		std::string _name;
		uint32 _id;
	};
}

#endif // __KGE_CORE_OBJECT_H__
