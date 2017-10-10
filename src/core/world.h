//
//  world.h
//  libkge
//
//  Created by kevin on 17-9-23.
//
//

#ifndef __KGE_CORE_WORLD_H__
#define __KGE_CORE_WORLD_H__

#include "game_object.h"

namespace kge
{
	class Renderer;

	class World
	{
	public:
		static void AddGameObject(const Ref<GameObject>& obj);
		static void AddGameObjects(const std::list<Ref<GameObject>>& objs);
		
		static bool Init();
		static void Fini();
		static void Tick();

	private:
		static void FindAllRenders(const std::list<Ref<GameObject>>& objs, std::list<Renderer*>& renderers, bool include_inactive, bool include_disable, bool static_only);

	private:
		static std::list<Ref<GameObject>> _gameObjects;
		static std::list<Ref<GameObject>> _gameObjects_start;
		static std::mutex _mutex;
	};
}

#endif // __KGE_CORE_WORLD_H__
