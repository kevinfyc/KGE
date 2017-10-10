//
//  renderer.cpp
//  libkge
//
//  Created by kevin on 17-9-25.
//
//

#include "renderer.h"
#include "graphics.h"
#include "shader.h"
#include "camera.h"
#include "material.h"
#include "display.h"

#include "core/game_object.h"
#include "core/transform.h"

#include "util/time.h"
#include "render_queue.h"
#include "mesh_renderer.h"
#include "io/memory_stream.h"

#include "math/math_def.h"

namespace kge
{
	DEFINE_COM_CLASS(Renderer);

	std::list<Renderer*> Renderer::m_renderers;
	std::map<Camera*, Renderer::Passes> Renderer::m_passes;
	bool Renderer::m_renderers_dirty = true;
	bool Renderer::m_passes_dirty = true;
	Ref<VertexBuffer> Renderer::m_static_vertex_buffer;
	Ref<IndexBuffer> Renderer::m_static_index_buffer;
	bool Renderer::m_static_buffers_binding = false;
	int32 Renderer::m_batching_start = -1;
	int32 Renderer::m_batching_count = -1;

	bool Renderer::Init()
	{
		return true;
	}

	void Renderer::Fini()
	{
		m_renderers.clear();
		m_passes.clear();
		m_renderers_dirty = true;
		m_passes_dirty = true;
		m_static_vertex_buffer.reset();
		m_static_index_buffer.reset();
		m_static_buffers_binding = false;
		m_batching_start = -1;
		m_batching_count = -1;
	}

	void Renderer::OnResize(uint32 width, uint32 height)
	{
		Shader::ClearAllPipelines();
	}

	void Renderer::DeepCopy(const Ref<Object>& source)
	{
		Component::DeepCopy(source);

		auto src = RefCast<Renderer>(source);
		SetSharedMaterials(src->GetSharedMaterials());
	}

	void Renderer::PreRenderByMaterial(uint32 material_index)
	{
		auto& vp = Camera::Current()->GetViewProjectionMatrix();
		auto& mat = this->GetSharedMaterials()[material_index];
		mat->SetMatrix("_ViewProjection", vp);
		mat->SetVector("_WorldSpaceCameraPos", Camera::Current()->GetTransform()->GetWorldPosition());
		mat->SetVector("_Time", Vector4(Time::GetTime()));

		//if (!Light::main.expired())
		//{
		//	auto light = Light::main.lock();
		//	mat->SetVector("_WorldSpaceLightPos", -light->GetTransform()->GetForward());
		//	mat->SetColor("_LightColor", light->color * light->intensity);
		//}
	}

	void Renderer::PreRenderByRenderer(uint32 material_index)
	{
		struct UniformBufferObject
		{
			Matrix world_matrix;
			Vector4 lightmap_sacle_offset;
		};
		UniformBufferObject buffer;
		uint32 size;

		bool static_batch = this->m_batch_indices.size() > 0;
		if (static_batch)
		{
			buffer.world_matrix = Matrix::identity;
			buffer.lightmap_sacle_offset = Vector4(1, 1, 0, 0);
			size = sizeof(Matrix) + sizeof(Vector4);
		}
		else
		{
			buffer.world_matrix = this->GetWorldMatrix();
			if (m_lightmap_index >= 0)
			{
				buffer.lightmap_sacle_offset = this->GetLightmapScaleOffset();
				size = sizeof(Matrix) + sizeof(Vector4);
			}
			else
			{
				size = sizeof(Matrix);
			}
		}

		auto& shader = this->GetSharedMaterials()[material_index]->GetShader();
		shader->UpdateRendererDescriptorSet(m_descriptor_set_buffer, &buffer, size);
	}

	Matrix Renderer::GetWorldMatrix()
	{
		return GetTransform()->GetLocal2WorldMatrix();
	}

	void Renderer::Render(uint32 material_index, uint32 pass_index)
	{
		auto& mat = this->GetSharedMaterials()[material_index];
		auto& shader = mat->GetShader();
		bool static_batch = this->m_batch_indices.size() > 0;
		bool batching = m_batching_start >= 0 && m_batching_count > 0;

		auto index_type = GetIndexType();
		if (static_batch)
		{
			index_type = IndexType::U32;
		}

		if (this->GetVertexBuffer() || static_batch)
		{
			if (!static_batch)
			{
				m_static_buffers_binding = false;
				Graphics::GetDisplay()->BindVertexBuffer(this->GetVertexBuffer());
				Graphics::GetDisplay()->BindIndexBuffer(this->GetIndexBuffer(), index_type);
			}
			else
			{
				if (!m_static_buffers_binding)
				{
					m_static_buffers_binding = true;
					BindStaticBuffers();
				}
			}

			if (!static_batch || !batching)
			{
				Graphics::GetDisplay()->BindVertexArray(shader, pass_index);
			}

			uint32 start, count;
			if (static_batch)
			{
				start = m_batch_indices[material_index].index_start;
				count = m_batch_indices[material_index].index_count;
			}
			else
			{
				this->GetIndexRange(material_index, start, count);
			}

			if (static_batch)
			{
				if (!batching)
				{
					// 第一批
					m_batching_start = start;
					m_batching_count = count;
				}
			}
			else
			{
				Graphics::GetDisplay()->DrawIndexed(start, count, index_type);
				Graphics::GetDisplay()->DisableVertexArray(shader, pass_index);
			}
		}
	}

	void Renderer::CommitPass(std::list<MaterialPass>& pass)
	{
		auto& first = pass.front();
		auto& shader = first.renderer->GetSharedMaterials()[first.material_index]->GetShader();

		if (first.shader_pass_count == 1)
		{
			shader->BeginPass(0);

			int old_id = -1;
			int old_lightmap_index = -1;
			for (auto& i : pass)
			{
				auto& mat = i.renderer->GetSharedMaterials()[i.material_index];
				bool bind_shared_mat = false;
				bool bind_lightmap = false;
				bool static_batch = i.renderer->m_batch_indices.size() > 0;
				bool batching = m_batching_start >= 0 && m_batching_count > 0;
				bool batching_break = false;

				int mat_id = mat->GetID();
				if (old_id == -1 || old_id != mat_id)
				{
					bind_shared_mat = true;
					bind_lightmap = true;
				}
				old_id = mat_id;

				if (old_lightmap_index == -1 || old_lightmap_index != i.renderer->m_lightmap_index)
				{
					bind_lightmap = true;
				}
				old_lightmap_index = i.renderer->m_lightmap_index;

				if (batching)
				{
					if (bind_shared_mat || bind_lightmap || !static_batch)
					{
						batching_break = true;
					}
				}

				if (static_batch)
				{
					uint32 start, count;
					start = i.renderer->m_batch_indices[i.material_index].index_start;
					count = i.renderer->m_batch_indices[i.material_index].index_count;

					if (batching)
					{
						if (m_batching_start + m_batching_count == start)
						{
							// 连续索引，追加到合并批次
							m_batching_count += count;
						}
						else
						{
							batching_break = true;
						}
					}
				}

				if (batching_break)
				{
					// 提交合并绘制
					Graphics::GetDisplay()->DrawIndexed(m_batching_start, m_batching_count, IndexType::U32);
					Graphics::GetDisplay()->DisableVertexArray(shader, 0);

					m_batching_start = -1;
					m_batching_count = -1;
					batching = false;
				}

				if (bind_shared_mat)
				{
					shader->BindSharedMaterial(0, mat);
				}

				Matrix world_matrix;
				Vector4 lightmap_sacle_offset;

				if (static_batch)
				{
					world_matrix = Matrix::identity;
					lightmap_sacle_offset = Vector4(1, 1, 0, 0);
				}
				else
				{
					world_matrix = i.renderer->GetTransform()->GetLocal2WorldMatrix();
					lightmap_sacle_offset = i.renderer->GetLightmapScaleOffset();
				}

				// 非静态或第一批
				if (!static_batch || !batching)
				{
					shader->BindMaterial(0, mat);
					shader->BindRendererDescriptorSet(0, i.renderer->m_descriptor_set_buffer);
				}

				i.renderer->Render(i.material_index, 0);
			}

			// pass完成，提交剩余批次
			if (m_batching_start >= 0 && m_batching_count > 0)
			{
				Graphics::GetDisplay()->DrawIndexed(m_batching_start, m_batching_count, IndexType::U32);
				Graphics::GetDisplay()->DisableVertexArray(shader, 0);

				m_batching_start = -1;
				m_batching_count = -1;
			}

			shader->EndPass(0);
		}
		else
		{
			assert(pass.size() == 1);

			auto& i = first;
			for (uint32 j = 0; j < i.shader_pass_count; j++)
			{
				Matrix world_matrix;
				Vector4 lightmap_sacle_offset;

				bool static_batch = i.renderer->m_batch_indices.size() > 0;
				if (static_batch)
				{
					world_matrix = Matrix::identity;
					lightmap_sacle_offset = Vector4(1, 1, 0, 0);
				}
				else
				{
					world_matrix = i.renderer->GetTransform()->GetLocal2WorldMatrix();
					lightmap_sacle_offset = i.renderer->GetLightmapScaleOffset();
				}

				shader->BeginPass(j);

				auto& mat = i.renderer->GetSharedMaterials()[i.material_index];
				shader->BindSharedMaterial(j, mat);
				shader->BindMaterial(j, mat);
				shader->BindRendererDescriptorSet(j, i.renderer->m_descriptor_set_buffer);

				i.renderer->Render(i.material_index, j);

				shader->EndPass(j);
			}
		}
	}

	void Renderer::PreparePass(std::list<MaterialPass>& pass)
	{
		auto& first = pass.front();
		auto& shader = first.renderer->GetSharedMaterials()[first.material_index]->GetShader();

		if (first.shader_pass_count == 1)
		{
			shader->PreparePass(0);

			int old_id = -1;
			for (auto& i : pass)
			{
				auto& mat = i.renderer->GetSharedMaterials()[i.material_index];
				int mat_id = mat->GetID();

				i.renderer->PreRenderByRenderer(i.material_index);

				if (old_id == -1 || old_id != mat_id)
				{
					i.renderer->PreRenderByMaterial(i.material_index);
					mat->UpdateUniforms(0);
				}

				old_id = mat_id;
			}
		}
		else
		{
			auto& mat = first.renderer->GetSharedMaterials()[first.material_index];
			first.renderer->PreRenderByRenderer(first.material_index);
			first.renderer->PreRenderByMaterial(first.material_index);

			for (uint32 i = 0; i < first.shader_pass_count; i++)
			{
				shader->PreparePass(i);
				mat->UpdateUniforms(i);
			}
		}
	}

	bool Renderer::IsRenderersDirty()
	{
		bool dirty;
		dirty = m_renderers_dirty;
		return dirty;
	}

	void Renderer::SetRenderersDirty(bool dirty)
	{
		m_renderers_dirty = dirty;
	}

	void Renderer::ClearPasses()
	{
		m_passes.clear();
	}

	void Renderer::SetCullingDirty(Camera* cam)
	{
		auto iter = m_passes.find(cam);
		if (m_passes.count(cam) > 0)
		{
			m_passes[cam].culling_dirty = true;
		}
	}

	std::list<Renderer*>& Renderer::GetRenderers()
	{
		return m_renderers;
	}

	void Renderer::HandleUIEvent()
	{
		//std::list<UICanvasRenderer*> canvas_list;
		//for (auto i : m_renderers)
		//{
		//	if (!i->GetGameObject()->IsActiveInHierarchy() ||
		//		!i->IsEnable())
		//	{
		//		continue;
		//	}

		//	auto canvas = dynamic_cast<UICanvasRenderer*>(i);
		//	if (canvas != NULL)
		//	{
		//		canvas_list.AddLast(canvas);
		//	}
		//}

		//canvas_list.Sort([](UICanvasRenderer* a, UICanvasRenderer* b) {
		//	return a->GetSortingOrder() < b->GetSortingOrder();
		//});

		//UICanvasRenderer::HandleUIEvent(canvas_list);
	}

	void Renderer::CheckPasses()
	{
		std::vector<Camera*> invalid_cams;
		for (auto& i : m_passes)
		{
			if (!Camera::IsValidCamera(i.first))
			{
				invalid_cams.push_back(i.first);
			}
		}
		for (auto i : invalid_cams)
		{
			m_passes.erase(i);
		}

		auto cam = Camera::Current();
		if (!(m_passes.count(cam) > 0))
		{
			m_passes[cam] = Passes();
		}
	}

	void Renderer::CameraCulling()
	{
		auto cam = Camera::Current();

		if (m_passes[cam].culling_dirty)
		{
			m_passes[cam].culling_dirty = false;

			std::list<Renderer*> renderers;
			auto& culled_renderers = m_passes[cam].culled_renderers;
			bool diff = false;

			// TODO: m_renderers in octree

			for (auto i : m_renderers)
			{
				if (!i->GetGameObject()->IsActiveInHierarchy() ||
					!i->IsEnable() ||
					cam->IsCulling(i->GetGameObject()))
				{
					continue;
				}

				if (cam->IsOrthographic())
				{
					renderers.push_back(i);
				}
				else
				{
					auto& bounds = i->GetBounds();
					auto ret = cam->GetFrustum().ContainsBounds(bounds.Min(), bounds.Max());
					if (ret == ContainsResult::Cross || ret == ContainsResult::In)
					{
						renderers.push_back(i);
					}
				}
			}

			if (renderers.size() != culled_renderers.size())
			{
				diff = true;
			}
			else
			{
				auto ia = renderers.begin();
				auto ib = culled_renderers.begin();

				while (ia != renderers.end() && ib != culled_renderers.end())
				{
					auto a = *ia;
					auto b = *ib;
					if (a != b)
					{
						diff = true;
						break;
					}

					ia++;
					ib++;
				}
			}

			if (diff)
			{
				culled_renderers = renderers;
				m_passes[cam].passes_dirty = true;
			}
		}
	}

	void Renderer::BuildPasses(const std::list<Renderer*>& renderers, std::list<std::list<MaterialPass>>& passes)
	{
		std::list<Renderer::MaterialPass> mat_passes;

		for (auto i : renderers)
		{
			auto& mats = i->GetSharedMaterials();

			for (uint32 j = 0; j < mats.size(); j++)
			{
				auto& mat = mats[j];

				if (!mat || !i->IsValidPass(j))
				{
					continue;
				}

				auto shader = mat->GetShader();
				uint32 pass_count = shader->GetPassCount();

				assert(pass_count >= 1);

				MaterialPass pass;
				pass.queue = shader->GetQueue();
				pass.shader_pass_count = pass_count;
				pass.renderer = i;
				pass.material_index = j;
				pass.shader_id = shader->GetID();
				pass.material_id = mat->GetID();

				mat_passes.push_back(pass);
			}
		}

		mat_passes.sort(
			[](const MaterialPass& a, const MaterialPass& b)->bool {
			//if (dynamic_cast<UICanvasRenderer*>(a.renderer) || dynamic_cast<UICanvasRenderer*>(b.renderer))
			//{
			//	return false;
			//}

			if (a.queue == b.queue)
			{
				int static_a = a.renderer->GetGameObject()->IsStatic() ? 0 : 1;
				int static_b = b.renderer->GetGameObject()->IsStatic() ? 0 : 1;

				if (static_a == static_b)
				{
					if (a.shader_pass_count == 1 && b.shader_pass_count == 1)
					{
						if (a.shader_id == b.shader_id)
						{
							if (a.material_id == b.material_id)
							{
								if (a.renderer->m_lightmap_index == b.renderer->m_lightmap_index)
								{
									if (a.renderer->GetID() == b.renderer->GetID())
									{
										return a.material_index < b.material_index;
									}
									else
									{
										return a.renderer->GetID() < b.renderer->GetID();
									}
								}
								else
								{
									return a.renderer->m_lightmap_index < b.renderer->m_lightmap_index;
								}
							}
							else
							{
								return a.material_id < b.material_id;
							}
						}
						else
						{
							return a.shader_id < b.shader_id;
						}
					}
					else
					{
						return a.shader_pass_count < b.shader_pass_count;
					}
				}
				else
				{
					return static_a < static_b;
				}
			}
			else
			{
				return a.queue < b.queue;
			}
		});

		passes.clear();

		std::list<MaterialPass> pass;
		for (auto& i : mat_passes)
		{
			if (pass.empty())
			{
				pass.push_back(i);
			}
			else
			{
				const auto& last = pass.back();
				if (/*dynamic_cast<UICanvasRenderer*>(i.renderer) == NULL &&*/
					i.queue == last.queue &&
					i.shader_pass_count == 1 && last.shader_pass_count == 1 &&
					i.shader_id == last.shader_id)
				{
					pass.push_back(i);
				}
				else
				{
					passes.push_back(pass);

					pass.clear();
					pass.push_back(i);
				}
			}
		}

		if (!pass.empty())
		{
			passes.push_back(pass);
		}
	}

	void Renderer::BuildPasses()
	{
		auto cam = Camera::Current();

		if (m_passes[cam].passes_dirty)
		{
			m_passes[cam].passes_dirty = false;

			BuildPasses(m_passes[cam].culled_renderers, m_passes[cam].list);
		}
	}

	void Renderer::PrepareAllPass()
	{
		CheckPasses();
		CameraCulling();
		BuildPasses();

		auto& passes = m_passes[Camera::Current()].list;
		for (auto& i : passes)
		{
			Renderer::PreparePass(i);
		}
	}

	void Renderer::BindStaticBuffers()
	{
		if (m_static_vertex_buffer && m_static_index_buffer)
		{
			Graphics::GetDisplay()->BindVertexBuffer(m_static_vertex_buffer.get());
			Graphics::GetDisplay()->BindIndexBuffer(m_static_index_buffer.get(), IndexType::U32);
		}
	}

	void Renderer::RenderAllPass()
	{
		BindStaticBuffers();
		m_static_buffers_binding = true;
		m_batching_start = -1;
		m_batching_count = -1;

		auto cam = Camera::Current();
		auto& passes = m_passes[cam].list;
		std::list<std::list<MaterialPass>> passes_transparent;
		std::list<std::list<MaterialPass>> passes_ui;

		for (auto& i : passes)
		{
			if (i.front().queue < (uint32)RenderQueue::Transparent)
			{
				Renderer::CommitPass(i);
			}
			else
			{
				//auto ui = dynamic_cast<UICanvasRenderer*>(i.First().renderer);
				//if (ui)
				//{
				//	passes_ui.AddLast(i);
				//}
				//else
				{
					passes_transparent.push_back(i);
				}
			}
		}

		for (auto& i : passes_transparent)
		{
			Renderer::CommitPass(i);
		}

		passes_ui.sort([](const std::list<MaterialPass>& a, const std::list<MaterialPass>& b) {
			if (a.front().queue == b.front().queue)
			{
				return a.front().renderer->GetSortingOrder() < b.front().renderer->GetSortingOrder();
			}
			else
			{
				return a.front().queue < b.front().queue;
			}
		});

		for (auto& i : passes_ui)
		{
			Renderer::CommitPass(i);
		}
	}

	Renderer::Renderer() :
		m_sorting_order(0),
		m_lightmap_index(-1),
		m_lightmap_scale_offset(),
		m_bounds(Vector3::one() * MIN_FLOAT_VALUE, Vector3::one() * MAX_FLOAT_VALUE)
	{
	}

	Renderer::~Renderer()
	{
		SetRenderersDirty(true);
	}

	void Renderer::Start()
	{
		SetRenderersDirty(true);
	}

	void Renderer::OnEnable()
	{
		SetRenderersDirty(true);
	}

	void Renderer::OnDisable()
	{
		SetRenderersDirty(true);
	}

	Ref<Material> Renderer::GetSharedMaterial() const
	{
		Ref<Material> mat;

		auto& mats = this->GetSharedMaterials();
		if (!mats.empty())
		{
			mat = mats[0];
		}

		return mat;
	}

	void Renderer::SetSharedMaterial(const Ref<Material>& mat)
	{
		auto mats = std::vector<Ref<Material>>();
		mats.push_back(mat);

		this->SetSharedMaterials(mats);
	}

	void Renderer::BuildStaticBatch(const Ref<GameObject>& obj)
	{
		std::list<Renderer*> renderers;
		std::list<std::list<MaterialPass>> passes;
		std::list<MaterialPass> passes_all;

		auto rs = obj->GetComponentsInChildren<MeshRenderer>();
		for (const auto& r : rs)
		{
			renderers.push_back(r.get());
		}

		BuildPasses(renderers, passes);

		uint32 vertex_count = 0;
		uint32 index_count = 0;

		for (const auto& i : passes)
		{
			auto& first = i.front();

			if (first.shader_pass_count == 1)
			{
				for (auto& j : i)
				{
					passes_all.push_back(j);
				}
			}
			else
			{
				passes_all.push_back(first);
			}
		}

		for (const auto& i : passes_all)
		{
			auto r = (MeshRenderer*)i.renderer;
			auto& mesh = r->GetSharedMesh();
			uint32 submesh = i.material_index;

			if (submesh >= r->m_batch_indices.size())
			{
				r->m_batch_indices.resize(submesh + 1);
			}

			uint32 start;
			uint32 count;
			mesh->GetIndexRange(submesh, start, count);

			r->m_batch_indices[submesh].index_start = index_count;
			r->m_batch_indices[submesh].index_count = count;

			vertex_count += mesh->vertices.size();
			index_count += count;
		}

		m_static_vertex_buffer = VertexBuffer::Create(vertex_count * VERTEX_STRIDE, false);
		m_static_vertex_buffer->Fill(NULL,
			[&](void* param, const ByteBuffer& buffer) {
			MemoryStream ms(buffer);

			for (const auto& i : passes_all)
			{
				auto r = (MeshRenderer*)i.renderer;
				auto& mesh = r->GetSharedMesh();
				auto& mat = r->GetTransform()->GetLocal2WorldMatrix();

				for (uint32 i = 0; i < mesh->vertices.size(); i++)
				{
					auto pos_world = mat.applyPoint(mesh->vertices[i]);
					ms.Write<Vector3>(pos_world);

					if (mesh->colors.empty())
					{
						ms.Write<Color>(Color(1, 1, 1, 1));
					}
					else
					{
						ms.Write<Color>(mesh->colors[i]);
					}

					if (mesh->uv.empty())
					{
						ms.Write<Vector2>(Vector2(0, 0));
					}
					else
					{
						ms.Write<Vector2>(mesh->uv[i]);
					}

					if (mesh->uv2.empty())
					{
						ms.Write<Vector2>(Vector2(0, 0));
					}
					else
					{
						auto& scale_offset = r->GetLightmapScaleOffset();
						auto uv2 = mesh->uv2[i];
						float x = uv2.x;
						float y = 1.0f - uv2.y;
						x = x * scale_offset.x + scale_offset.z;
						y = y * scale_offset.y + scale_offset.w;
						y = 1.0f - y;
						ms.Write<Vector2>(Vector2(x, y));
					}

					if (mesh->normals.empty())
					{
						ms.Write<Vector3>(Vector3(0, 0, 0));
					}
					else
					{
						auto normal_world = mat.applyVector(mesh->normals[i]);
						ms.Write<Vector3>(normal_world);
					}

					if (mesh->tangents.empty())
					{
						ms.Write<Vector4>(Vector4(0, 0, 0, 0));
					}
					else
					{
						auto tangent = mesh->tangents[i];
						auto tangent_xyz = Vector3(tangent.x, tangent.y, tangent.z);
						auto tangent_xyz_world = mat.applyVector(tangent_xyz);
						auto tangent_world = Vector4(tangent_xyz_world.x, tangent_xyz_world.y, tangent_xyz_world.z, tangent.w);
						ms.Write<Vector4>(tangent_world);
					}

					if (mesh->bone_weights.empty())
					{
						ms.Write<Vector4>(Vector4(0, 0, 0, 0));
					}
					else
					{
						ms.Write<Vector4>(mesh->bone_weights[i]);
					}

					if (mesh->bone_indices.empty())
					{
						ms.Write<Vector4>(Vector4(0, 0, 0, 0));
					}
					else
					{
						ms.Write<Vector4>(mesh->bone_indices[i]);
					}
				}
			}

			ms.Close();
		});

		m_static_index_buffer = IndexBuffer::Create(index_count * sizeof(uint32), false);
		m_static_index_buffer->Fill(NULL,
			[&](void* param, const ByteBuffer& buffer) {
			MemoryStream ms(buffer);

			uint32 mesh_index_offset = 0;

			for (const auto& i : passes_all)
			{
				auto r = (MeshRenderer*)i.renderer;
				auto& mesh = r->GetSharedMesh();
				uint32 submesh = i.material_index;

				uint32 start;
				uint32 count;
				mesh->GetIndexRange(submesh, start, count);

				for (uint32 j = 0; j < count; j++)
				{
					uint32 index = mesh->triangles[start + j] + mesh_index_offset;
					ms.Write<uint32>(index);
				}

				mesh_index_offset += mesh->vertices.size();
			}

			ms.Close();
		});
	}
}

