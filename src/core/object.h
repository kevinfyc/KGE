﻿//
//  object.h
//  libkge
//
//  Created by kevin on 17-9-22.
//
//

#ifndef __KGE_CORE_OBJECT_H__
#define __KGE_CORE_OBJECT_H__

#include "pre_define.h"
#include <mutex>

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
		virtual void DeepCopy(const Ref<Object>& source);													\
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


#define DEFINE_COM_BASE(base)																				\
    std::map<std::string, base::gen> base::_class_pool;														\
	std::vector<std::string> base::_class_names;

#define DECLARE_COM_CLASS(derived, super)																	\
    public:																									\
		static void RegisterComponent()																		\
		{																									\
			Register(#derived, derived::Create);															\
		}																									\
		static std::string ClassName() { return #derived; }													\
		static const std::vector<std::string>& ClassNames() {												\
			if(_class_names.empty()) {																		\
				_class_names = super::ClassNames();															\
				_class_names.push_back(#derived);															\
			}																								\
			return _class_names;																			\
		}																									\
        virtual std::string GetTypeName() const { return #derived; }										\
		virtual const std::vector<std::string>& GetClassNames() const { return derived::ClassNames(); }		\
		virtual void DeepCopy(const Ref<Object>& source);													\
    private:																								\
        static void* Create()																				\
		{																									\
            return new derived();																			\
        }																									\
		static std::vector<std::string> _class_names;

#define DECLARE_COM_CLASS_ABSTRACT(derived, super)															\
    public:																									\
		static std::string ClassName() { return #derived; }													\
		static const std::vector<std::string>& ClassNames() {												\
			if(_class_names.empty()) {																		\
				_class_names = super::ClassNames();															\
				_class_names.push_back(#derived);															\
			}																								\
			return _class_names;																			\
		}																									\
        virtual std::string GetTypeName() const { return #derived; }										\
		virtual const std::vector<std::string>& GetClassNames() const { return derived::ClassNames(); }		\
		virtual void DeepCopy(const Ref<Object>& source);													\
    private:																								\
		static std::vector<std::string> _class_names;

#define DEFINE_COM_CLASS(derived)																			\
    std::vector<std::string> derived::_class_names;


namespace kge
{
	class Object
	{
	public:
		Object();
		virtual ~Object();

		static bool Init();
		static void Fini();

		static Ref<Object> GetCache(const std::string& path);
		static void AddCache(const std::string& path, const Ref<Object>& obj);

		virtual void DeepCopy(const Ref<Object>& source);

		inline void SetName(const std::string& name) { _name = name; }
		inline const std::string& GetName() const { return _name; }

		inline uint32 GetID() const  { return _id; }

	protected:
		Object(const Object&) {}
		Object& operator = (const Object& obj) { return *this; }

	private:
		std::string _name;
		uint32 _id;

		static std::map<std::string, Ref<Object>> s_caches;
		static std::mutex s_mutex;
	};
}

#endif // __KGE_CORE_OBJECT_H__
