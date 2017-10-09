//
//  world.cpp
//  libkge
//
//  Created by kevin on 17-9-23.
//
//

#include "world.h"

#include "graphics/camera.h"
#include "graphics/shader.h"

#include "Object.h"
#include "graphics/renderer.h"

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

		if (!Shader::Init())
			return false;

		if (!Camera::Init())
			return false;

		if (!Renderer::Init())
			return false;

		return true;
	}

	void World::Fini()
	{
		_gameObjects.clear();
		_gameObjects_start.clear();

		Renderer::Fini();
		Camera::Fini();
		Shader::Fini();

	}

	void World::Tick()
	{
		for (Ref<GameObject> obj : _gameObjects_start)
		{
			if (!obj->_deleted)
			{
				if (obj->IsActiveInHierarchy())
					obj->Start();

				if (!obj->_in_world_update)
				{
					obj->_in_world_update = true;
					_gameObjects.push_back(obj);
				}
			}
		}
		_gameObjects_start.clear();

		for (auto iter = _gameObjects.begin(); iter != _gameObjects.end();)
		{
			auto& obj = *iter;
			if (!obj->_deleted)
			{
				if (obj->IsActiveInHierarchy())
				{
					obj->Start();
					obj->Update();
				}
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
				if(obj->IsActiveInHierarchy())
					obj->LateUpdate();
			}
			else
			{
				iter = _gameObjects.erase(iter);
				continue;
			}

			iter++;
		}

		if (Renderer::IsRenderersDirty())
		{
			Renderer::SetRenderersDirty(false);
			Renderer::ClearPasses();

			std::list<Renderer*>& renderers = Renderer::GetRenderers();
			renderers.clear();

			FindAllRenders(_gameObjects, renderers, false, false, false);
		}
	}

	void World::FindAllRenders(const std::list<Ref<GameObject>>& objs, std::list<Renderer*>& renderers, bool include_inactive, bool include_disable, bool static_only)
	{
		for (Ref<GameObject> obj : objs)
		{
			if (obj->_deleted)
				continue;

			if (include_inactive || obj->IsActiveInHierarchy())
			{
				if (!static_only || obj->IsStatic())
				{
					std::vector<Ref<Renderer>> rs;
					if (!obj->GetComponents<Renderer>(rs))
						continue;

					for (Ref<Renderer>& renderer : rs)
					{
						if (include_disable || renderer->IsEnable())
							renderers.push_back(renderer.get());
					}
				}
			}
		}
	}

} // end namespace kge
