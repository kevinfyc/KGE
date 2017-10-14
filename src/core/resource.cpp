//
//  resource.cpp
//  libkge
//
//  Created by kevin on 17-10-10.
//
//

#include "resource.h"

#include "io/memory_stream.h"
#include "io/file_tool.h"

#include "util/thread.h"
#include "util/log.h"
#include "graphics/graphics.h"
#include "graphics/display.h"
#include "graphics/renderer.h"
#include "world.h"
#include "graphics/mesh_renderer.h"
#include "graphics/material.h"
#include "ui/ui_image.h"
#include "ui/ui_canvas_renderer.h"
#include "ui/ui_label.h"

namespace kge
{
	Ref<ThreadPool> Resource::s_thread_res_load;

	static std::string read_string(MemoryStream& ms)
	{
		uint32 size = ms.Read<uint32>();
		return ms.ReadString(size);
	}

	static Ref<Texture> read_texture(const std::string& path)
	{
		Ref<Texture> texture;

		if (!FileSystemMgr::GetInstance()->getFileSystem()->isFileExist(path))
		{
			KGE_LOG_ERROR("Failed to create file '%s'", path.c_str());
			return texture;
		}

		auto cache = Object::GetCache(path);
		if (cache)
		{
			texture = RefCast<Texture>(cache);
		}
		else
		{
			auto ms = MemoryStream(ReadFile(path, true));
			auto texture_name = read_string(ms);

			ms.Read<uint32>(); // width
			ms.Read<uint32>(); // height

			auto wrap_mode = (TextureWrapMode)ms.Read<uint32>();
			auto filter_mode = (FilterMode)ms.Read<uint32>();
			auto texture_type = read_string(ms);

			if (texture_type == "Texture2D")
			{
				uint32 mipmap_count = ms.Read<uint32>();
				auto png_path = read_string(ms);

				bool mipmap = mipmap_count > 1;
				texture = Texture2D::LoadFromFile(png_path, wrap_mode, filter_mode, mipmap);

				if (texture)
					Object::AddCache(path, texture);
			}

			ms.Close();
		}

		return  texture;
	}

	static Ref<Font> read_font(const std::string& path)
	{
		Ref<Font> font;

		if (!FileSystemMgr::GetInstance()->getFileSystem()->isFileExist(path))
		{
			KGE_LOG_ERROR("Failed to create file '%s'", path.c_str());
			return font;
		}

		auto cache = Object::GetCache(path);
		if (cache)
		{
			font = RefCast<Font>(cache);
		}
		else
		{
			font = Font::LoadFromFile(path);

			if (font)
				Object::AddCache(path, font);
		}

		return font;
	}

	static Ref<Mesh> read_mesh(const std::string& path)
	{
		Ref<Mesh> mesh;

		if (!FileSystemMgr::GetInstance()->getFileSystem()->isFileExist(path))
		{
			KGE_LOG_ERROR("Failed to create file '%s'", path.c_str());
			return mesh;
		}

		auto cache = Object::GetCache(path);
		if (cache)
		{
			mesh = RefCast<Mesh>(cache);
		}
		else
		{
			auto ms = MemoryStream(ReadFile(path, true));
			mesh = Mesh::Create();
			Object::AddCache(path, mesh);

			auto mesh_name = read_string(ms);
			mesh->SetName(mesh_name);

			auto vertex_count = ms.Read<uint32>();
			if (vertex_count > 0)
			{
				mesh->vertices.resize(vertex_count);
				ms.Read(&mesh->vertices[0], vertex_count * sizeof(Vector3));
			}

			auto uv_count = ms.Read<uint32>();
			if (uv_count > 0)
			{
				mesh->uv.resize(uv_count);
				ms.Read(&mesh->uv[0], uv_count * sizeof(Vector2));
			}

			auto color_count = ms.Read<uint32>();
			if (color_count > 0)
			{
				mesh->colors.resize(color_count);
				ms.Read(&mesh->colors[0], color_count * sizeof(Color));
			}

			auto uv2_count = ms.Read<uint32>();
			if (uv2_count > 0)
			{
				mesh->uv2.resize(uv2_count);
				ms.Read(&mesh->uv2[0], uv2_count * sizeof(Vector2));
			}

			auto normal_count = ms.Read<uint32>();
			if (normal_count > 0)
			{
				mesh->normals.resize(normal_count);
				ms.Read(&mesh->normals[0], normal_count * sizeof(Vector3));
			}

			auto tangent_count = ms.Read<uint32>();
			if (tangent_count > 0)
			{
				mesh->tangents.resize(tangent_count);
				ms.Read(&mesh->tangents[0], tangent_count * sizeof(Vector4));
			}

			auto bone_weight_count = ms.Read<uint32>();
			if (bone_weight_count > 0)
			{
				mesh->bone_weights.resize(bone_weight_count);
				ms.Read(&mesh->bone_weights[0], bone_weight_count * sizeof(Vector4));
			}

			auto bone_index_count = ms.Read<uint32>();
			if (bone_index_count > 0)
			{
				mesh->bone_indices.resize(bone_index_count);
				ms.Read(&mesh->bone_indices[0], bone_index_count * sizeof(Vector4));
			}

			auto bind_pose_count = ms.Read<uint32>();
			if (bind_pose_count > 0)
			{
				mesh->bind_poses.resize(bind_pose_count);
				ms.Read(&mesh->bind_poses[0], bind_pose_count * sizeof(Matrix));
			}

			auto index_count = ms.Read<uint32>();
			if (index_count > 0)
			{
				mesh->triangles.resize(index_count);
				ms.Read(&mesh->triangles[0], index_count * sizeof(unsigned short));
			}

			auto submesh_count = ms.Read<uint32>();
			if (submesh_count > 0)
			{
				mesh->submeshes.resize(submesh_count);
				ms.Read(&mesh->submeshes[0], submesh_count * sizeof(Mesh::SubMesh));
			}

			ms.Close();
		}

		return mesh;
	}

	static Ref<Material> read_material(const std::string& path)
	{
		Ref<Material> mat;

		if (!FileSystemMgr::GetInstance()->getFileSystem()->isFileExist(path))
		{
			KGE_LOG_ERROR("Failed to create file '%s'", path.c_str());
			return mat;
		}

		auto cache = Object::GetCache(path);
		if (cache)
		{
			mat = RefCast<Material>(cache);
		}
		else
		{
			auto ms = MemoryStream(ReadFile(path, true));

			auto mat_name = read_string(ms);
			auto shader_name = read_string(ms);

			mat = Material::Create(shader_name);
			Object::AddCache(path, mat);

			mat->SetName(mat_name);

			auto property_count = ms.Read<uint32>();
			for (uint32 i = 0; i < property_count; i++)
			{
				auto property_name = read_string(ms);
				auto property_type = read_string(ms);

				if (property_type == "Color")
				{
					auto c = ms.Read<Color>();
					mat->SetColor(property_name, c);
				}
				else if (property_type == "Vector")
				{
					auto v = ms.Read<Vector4>();
					mat->SetVector(property_name, v);
				}
				else if (property_type == "Float" || property_type == "Range")
				{
					auto f = ms.Read<float>();
					mat->SetVector(property_name, Vector4(f, 0, 0, 0));
				}
				else if (property_type == "TexEnv")
				{
					auto tex_st = ms.Read<Vector4>();
					auto tex_path = read_string(ms);

					if (!tex_path.empty())
					{
						auto texture = read_texture(tex_path);

						if (texture)
						{
							mat->SetTexture(property_name, texture);
							mat->SetVector(property_name + "_ST", tex_st);
						}
					}
				}
			}

			ms.Close();
		}

		return mat;
	}

	static void read_renderer_materials(MemoryStream& ms, Renderer* renderer)
	{
		auto mat_count = ms.Read<int>();
		if (mat_count > 0)
		{
			std::vector<Ref<Material>> mats(mat_count);

			for (int i = 0; i < mat_count; i++)
			{
				auto mat_path = read_string(ms);

				if (!mat_path.empty())
				{
					auto mat = read_material(mat_path);

					mats[i] = mat;
				}
			}

			renderer->SetSharedMaterials(mats);
		}
	}

	static void read_mesh_renderer(MemoryStream& ms, Ref<MeshRenderer>& renderer)
	{
		auto mesh_path = read_string(ms);
		if (!mesh_path.empty())
		{
			auto mesh = read_mesh(mesh_path);
			renderer->SetSharedMesh(mesh);
		}

		auto bounds_min = ms.Read<Vector3>();
		auto bounds_max = ms.Read<Vector3>();
		renderer->SetBounds(Bounds(bounds_min, bounds_max));

		read_renderer_materials(ms, renderer.get());

		if (renderer->GetSharedMaterials().size() > 0)
		{
			auto mesh = renderer->GetSharedMesh();
			if (mesh)
				mesh->Tick();
		}

		auto lightmap_index = ms.Read<int32>();
		auto lightmap_scale_offset = ms.Read<Vector4>();

		renderer->SetLightmapIndex(lightmap_index);
		renderer->SetLightmapScaleOffset(lightmap_scale_offset);
	}

	static void read_rect(MemoryStream& ms, Ref<UIRect>& rect)
	{
		auto anchor_min = ms.Read<Vector2>();
		auto anchor_max = ms.Read<Vector2>();
		auto offset_min = ms.Read<Vector2>();
		auto offset_max = ms.Read<Vector2>();
		auto pivot = ms.Read<Vector2>();

		rect->SetAnchors(anchor_min, anchor_max);
		rect->SetOffsets(offset_min, offset_max);
		rect->SetPivot(pivot);

		auto canvas = RefCast<UICanvasRenderer>(rect);
		if (canvas)
		{
			uint32 screen_w = Graphics::GetDisplay()->GetWidth();
			uint32 screen_h = Graphics::GetDisplay()->GetHeight();
			canvas->SetSize(Vector2((float)screen_w, (float)screen_h));
		}

		rect->OnAnchor();
	}

	static void read_canvas(MemoryStream& ms, Ref<UICanvasRenderer>& canvas)
	{
		auto sorting_order = ms.Read<int>();

		canvas->SetSortingOrder(sorting_order);
	}

	static Ref<SpriteGroup> read_sprite_group(const std::string& path)
	{
		Ref<SpriteGroup> sg;

		if (!FileSystemMgr::GetInstance()->getFileSystem()->isFileExist(path))
		{
			KGE_LOG_ERROR("Failed to create file '%s'", path.c_str());
			return sg;
		}

		auto cache = Object::GetCache(path);
		if (cache)
		{
			sg = RefCast<SpriteGroup>(cache);
		}
		else
		{
			auto ms = MemoryStream(ReadFile(path, true));

			sg = SpriteGroup::Create();

			auto texture_path = read_string(ms);
			auto texture = RefCast<Texture2D>(read_texture(texture_path));
			sg->SetTexture(texture);

			auto sprite_count = ms.Read<int>();
			for (int i = 0; i < sprite_count; i++)
			{
				auto name = read_string(ms);
				auto rect = ms.Read<Rect>();
				ms.Read<Vector2>();//pivot
				ms.Read<float>();//pixel per unit
				auto border = ms.Read<Vector4>();

				auto sprite = Sprite::Create(rect, border);
				sprite->SetName(name);
				sg->AddSprite(name, sprite);
			}

			Object::AddCache(path, sg);

			ms.Close();
		}

		return sg;
	}

	static void read_image(MemoryStream& ms, Ref<UIImage>& view)
	{
		auto color = ms.Read<Color>();
		auto sprite_type = (SpriteType)ms.Read<int>();
		auto fill_method = (SpriteFillMethod)ms.Read<int>();
		auto fill_origin = ms.Read<int>();
		auto fill_amount = ms.Read<float>();
		auto fill_clock_wise = ms.Read<bool>();
		auto sprite_name = read_string(ms);

		if (!sprite_name.empty())
		{
			auto atlas_path = read_string(ms);
			auto atlas = read_sprite_group(atlas_path);

			view->SetSpriteGroup(atlas);
			view->SetSpriteName(sprite_name);
		}

		view->SetColor(color);
		view->SetSpriteType(sprite_type);
		view->SetFillMethod(fill_method);
		view->SetFillOrigin(fill_origin);
		view->SetFillAmount(fill_amount);
		view->SetFillClockWise(fill_clock_wise);
	}

	static void read_label(MemoryStream& ms, Ref<UILabel>& view)
	{
		auto color = ms.Read<Color>();
		auto text = read_string(ms);
		auto font_name = read_string(ms);
		auto font_style = read_string(ms);
		auto font_size = ms.Read<int>();
		auto line_space = ms.Read<float>();
		auto rich = ms.Read<bool>();
		auto alignment = read_string(ms);

		if (!font_name.empty())
		{
			auto font = read_font("Assets/font/" + font_name + ".ttf");

			view->SetFont(font);

			if (font_style == "Normal")
			{
				view->SetFontStyle(FontStyle::Normal);
			}
			else if (font_style == "Bold")
			{
				view->SetFontStyle(FontStyle::Bold);
			}
			else if (font_style == "Italic")
			{
				view->SetFontStyle(FontStyle::Italic);
			}
			else if (font_style == "BoldAndItalic")
			{
				view->SetFontStyle(FontStyle::BoldAndItalic);
			}

			view->SetFontSize(font_size);
		}

		view->SetColor(color);
		view->SetText(text);
		view->SetLineSpace((int)line_space);
		view->SetRich(rich);

		if (alignment == "UpperLeft")
		{
			view->SetAlignment(TextAlignment::UpperLeft);
		}
		else if (alignment == "UpperCenter")
		{
			view->SetAlignment(TextAlignment::UpperCenter);
		}
		else if (alignment == "UpperRight")
		{
			view->SetAlignment(TextAlignment::UpperRight);
		}
		else if (alignment == "MiddleLeft")
		{
			view->SetAlignment(TextAlignment::MiddleLeft);
		}
		else if (alignment == "MiddleCenter")
		{
			view->SetAlignment(TextAlignment::MiddleCenter);
		}
		else if (alignment == "MiddleRight")
		{
			view->SetAlignment(TextAlignment::MiddleRight);
		}
		else if (alignment == "LowerLeft")
		{
			view->SetAlignment(TextAlignment::LowerLeft);
		}
		else if (alignment == "LowerCenter")
		{
			view->SetAlignment(TextAlignment::LowerCenter);
		}
		else if (alignment == "LowerRight")
		{
			view->SetAlignment(TextAlignment::LowerRight);
		}
	}

	static Ref<Transform> read_transform(MemoryStream& ms, const Ref<Transform>& parent, std::list<Ref<GameObject>>& objs, std::map<uint32, Ref<Transform>>& transform_instances)
	{
		auto name = read_string(ms);
		auto active = ms.Read<bool>();
		auto is_static = ms.Read<bool>();

		auto obj = GameObject::Create(name, false);
		objs.push_back(obj);
		obj->SetActive(active);
		obj->SetStatic(is_static);

		auto transform = obj->GetTransform();
		if (parent)
			transform->SetParent(parent);

		auto local_position = ms.Read<Vector3>();
		auto local_rotation = ms.Read<Quaternion>();
		auto local_scale = ms.Read<Vector3>();

		auto instance_id = ms.Read<uint32>();
		transform_instances.insert(std::pair<uint32, Ref<Transform>>(instance_id, transform));

		transform->SetLocalPosition(local_position);
		transform->SetLocalRotation(local_rotation);
		transform->SetLocalScale(local_scale);

		uint32 com_count = ms.Read<uint32>();

		for (uint32 i = 0; i < com_count; i++)
		{
			auto component_name = read_string(ms);

			if (component_name == "MeshRenderer")
			{
				auto com = obj->AddComponent<MeshRenderer>();

				read_mesh_renderer(ms, com);
			}
			else if (component_name == "Canvas")
			{
				auto com = obj->AddComponent<UICanvasRenderer>();

				auto rect = RefCast<UIRect>(com);
				read_rect(ms, rect);
				read_canvas(ms, com);
			}
			else if (component_name == "Image")
			{
				auto com = obj->AddComponent<UIImage>();

				auto rect = RefCast<UIRect>(com);
				read_rect(ms, rect);
				read_image(ms, com);
			}
			else if (component_name == "Text")
			{
				auto com = obj->AddComponent<UILabel>();

				auto rect = RefCast<UIRect>(com);
				read_rect(ms, rect);
				read_label(ms, com);
			}
			else if (component_name == "RectTransform")
			{
				auto com = obj->AddComponent<UIView>();

				auto rect = RefCast<UIRect>(com);
				read_rect(ms, rect);
			}
		}

		uint32 child_count = ms.Read<uint32>();
		for (uint32 i = 0; i < child_count; i++)
		{
			read_transform(ms, transform, objs, transform_instances);
		}

		return transform;
	}

	static Ref<GameObject> read_gameObject(const std::string& path, bool static_batch)
	{
		Ref<GameObject> obj;

		if (!FileSystemMgr::GetInstance()->getFileSystem()->isFileExist(path))
		{
			KGE_LOG_ERROR("Failed to create file '%s'", path.c_str());
			return obj;
		}

		auto cache = Object::GetCache(path);
		if (cache)
		{
			obj = GameObject::Instantiate(RefCast<GameObject>(cache));
		}
		else
		{
			auto ms = MemoryStream(ReadFile(path, true));

			std::list<Ref<GameObject>> objs;
			std::map<uint32, Ref<Transform>> transform_instances;

			obj = read_transform(ms, Ref<Transform>(), objs, transform_instances)->GetGameObject();

#if KGE_GLES
			if (static_batch)
				Renderer::BuildStaticBatch(obj);
#endif

			World::AddGameObjects(objs);

			if (obj)
				Object::AddCache(path, obj);

			ms.Close();
		}

		return obj;
	}

	bool Resource::Init()
	{
		auto thread_init = []() {
			Graphics::GetDisplay()->CreateSharedContext();
		};
		auto thread_deinit = []() {
			Graphics::GetDisplay()->DestroySharedContext();
		};
		std::vector<ThreadNode> info;
		info.push_back({ thread_init, thread_deinit });
		s_thread_res_load = RefMake<ThreadPool>(info);

		return true;
	}

	void Resource::Fini()
	{
		s_thread_res_load->Wait();
		s_thread_res_load.reset();
	}

	Ref<Texture> Resource::LoadTexture(const std::string& path)
	{
		return read_texture(path);
	}

	Ref<Mesh> Resource::LoadMesh(const std::string& path)
	{
		return read_mesh(path);
	}

	Ref<Font> Resource::LoadFont(const std::string& path)
	{
		return read_font(path);
	}

	Ref<GameObject> Resource::LoadGameObject(const std::string& path, bool static_back /* = false */, LoadFiniCallback callback /* = nullptr */)
	{
		auto obj = read_gameObject(path, static_back);
		if (callback)
			callback(obj);
		return obj;
	}

	void Resource::LoadTextureAsync(const std::string& path, LoadFiniCallback callback)
	{
		s_thread_res_load->AddTask(
		{
			[=]() {
			auto tex = Resource::LoadTexture(path);
			Graphics::GetDisplay()->FlushContext();
			return RefMake<Any>(tex);
		},
			[=](Ref<Any> any) {
			auto tex = any->Get<Ref<Texture>>();
			if (callback)
			{
				callback(tex);
			}
		}
		}
		);
	}

	void Resource::LoadMeshAsync(const std::string& path, LoadFiniCallback callback)
	{
		s_thread_res_load->AddTask(
		{
			[=]() {
			auto mesh = Resource::LoadMesh(path);
			Graphics::GetDisplay()->FlushContext();
			return RefMake<Any>(mesh);
		},
			[=](Ref<Any> any) {
			auto mesh = any->Get<Ref<Mesh>>();
			if (callback)
			{
				callback(mesh);
			}
		}
		}
		);
	}

	void Resource::LoadGameObjectAsync(const std::string& path, bool static_batch, LoadFiniCallback callback)
	{
		s_thread_res_load->AddTask(
		{
			[=]() {
			auto obj = Resource::LoadGameObject(path, static_batch, NULL);
			Graphics::GetDisplay()->FlushContext();
			return RefMake<Any>(obj);
		},
			[=](Ref<Any> any) {
			auto obj = any->Get<Ref<GameObject>>();
			if (callback)
			{
				callback(obj);
			}
		}
		}
		);
	}

} // end namespace kge
