//
//  resource.h
//  libkge
//
//  Created by kevin on 17-10-10.
//
//

#ifndef __KGE_CORE_RESOURCE_H__
#define __KGE_CORE_RESOURCE_H__

#include "object.h"
#include "game_object.h"
#include "graphics/mesh.h"
#include "graphics/texture2D.h"
#include <functional>
#include "ui/sprite_group.h"
#include "ui/font.h"

namespace kge
{

	class ThreadPool;

	class Resource
	{
	public:
		typedef std::function<void(const Ref<Object>& obj)> LoadFiniCallback;

		static bool Init();
		static void Fini();

		static Ref<GameObject> LoadGameObject(const std::string& path, bool static_back = false, LoadFiniCallback callback = nullptr);
		static Ref<Texture> LoadTexture(const std::string& path);
		static Ref<Mesh> LoadMesh(const std::string& path);
		static Ref<Font> LoadFont(const std::string& path);

		static void LoadGameObjectAsync(const std::string& path, bool static_batch = false, LoadFiniCallback callback = nullptr);
		static void LoadTextureAsync(const std::string& path, LoadFiniCallback callback = nullptr);
		static void LoadMeshAsync(const std::string& path, LoadFiniCallback callback = nullptr);

		static Ref<SpriteGroup> read_sprite_group(const std::string& path);
	private:
		static Ref<ThreadPool> s_thread_res_load;
	};
}

#endif // __KGE_CORE_RESOURCE_H__
