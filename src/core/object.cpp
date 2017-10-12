//
//  object.cpp
//  libkge
//
//  Created by kevin on 17-9-22.
//
//

#include "object.h"

namespace kge
{

	std::map<std::string, Ref<Object>> Object::s_caches;
	std::mutex Object::s_mutex;

	bool Object::Init()
	{
		return true;
	}

	void Object::Fini()
	{
		s_mutex.lock();
		s_caches.clear();
		s_mutex.unlock();
	}

	Object::Object() :_name("Object")
	{
		static int s_id = 0;
		_id = s_id++;
	}

	Object::~Object()
	{

	}

	void Object::AddCache(const std::string& path, const Ref<Object>& obj)
	{
		s_mutex.lock();
		s_caches.insert(std::pair<std::string, Ref<Object>>(path, obj));
		s_mutex.unlock();
	}

	Ref<Object> Object::GetCache(const std::string& path)
	{
		Ref<Object> obj;

		s_mutex.lock();

		auto iter = s_caches.find(path);
		if (iter != s_caches.end())
			obj = iter->second;

		s_mutex.unlock();

		return obj;
	}

	void Object::DeepCopy(const Ref<Object>& source)
	{
		_name = source->_name;
	}

} // end namespace kge
