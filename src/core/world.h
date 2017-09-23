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
	class World
	{
	public:
		static void AddGameObject(const Ref<GameObject>& obj);
		static void AddGameObjects(const std::list<Ref<GameObject>>& objs);
		
		static bool Init();
		static void Fini();
		static void Tick();

	private:
		static std::list<Ref<GameObject>> _gameObjects;
		static std::list<Ref<GameObject>> _gameObjects_start;
	};
}

#endif // __KGE_CORE_WORLD_H__
