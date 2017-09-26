//
//  world.cpp
//  libkge
//
//  Created by kevin on 17-9-23.
//
//

#include "world.h"

#include "graphics/camera.h"
#include "Object.h"

namespace kge
{
	std::list<Ref<GameObject>> World::_gameObjects;
	std::list<Ref<GameObject>> World::_gameObjects_start;

	void World::AddGameObject(const Ref<GameObject>& obj)
	{
		_gameObjects_start.push_back(obj);
	}

	void World::AddGameObjects(const std::list<Ref<GameObject>>& objs)
	{
		for (Ref<GameObject> obj : objs)
			_gameObjects_start.push_back(obj);
	}

	bool World::Init()
	{
		Component::RegisterComponents();

		if (!Camera::Init())
			return false;

		return true;
	}

	void World::Fini()
	{
		_gameObjects.clear();
		_gameObjects_start.clear();

		Camera::Fini();
	}

	void World::Tick()
	{
		for (Ref<GameObject> obj : _gameObjects_start)
		{
			obj->Start();

			if (!obj->_in_world_update)
			{
				obj->_in_world_update = true;
				_gameObjects.push_back(obj);
			}
		}
		_gameObjects_start.clear();

		for (auto iter = _gameObjects.begin(); iter != _gameObjects.end();)
		{
			auto& obj = *iter;
			if (!obj->_deleted)
			{
				obj->Start();
				obj->Update();
			}
			else
			{
				iter = _gameObjects.erase(iter);
				continue;
			}

			iter++;
		}

		for (auto iter = _gameObjects.begin(); iter != _gameObjects.end();)
		{
			auto& obj = *iter;
			if (!obj->_deleted)
			{
				obj->LateUpdate();
			}
			else
			{
				iter = _gameObjects.erase(iter);
				continue;
			}

			iter++;
		}
	}
} // end namespace kge
