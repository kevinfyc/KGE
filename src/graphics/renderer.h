//
//  renderer.h
//  libkge
//
//  Created by kevin on 17-9-25.
//
//

#ifndef __KGE_GRAPHICS_RENDERER_H__
#define __KGE_GRAPHICS_RENDERER_H__

#include "core/component.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "math/bounds.h"

namespace kge
{
	class Material;
	class Camera;
	class DescriptorSet;
	class UniformBuffer;

	class Renderer : public Component
	{
		DECLARE_COM_CLASS_ABSTRACT(Renderer, Component);
	public:
		static bool Init();
		static void Fini();
		static bool IsRenderersDirty();
		static void SetRenderersDirty(bool dirty);
		static void ClearPasses();
		static void SetCullingDirty(Camera* cam);
		static std::list<Renderer*>& GetRenderers();
		static void PrepareAllPass();
		static void RenderAllPass();
		static void HandleUIEvent();
		static void BuildStaticBatch(const Ref<GameObject>& obj);

		Ref<Material> GetSharedMaterial() const;
		void SetSharedMaterial(const Ref<Material>& mat);
		virtual ~Renderer();
		virtual const VertexBuffer* GetVertexBuffer() const = 0;
		virtual const IndexBuffer* GetIndexBuffer() const = 0;
		virtual void GetIndexRange(uint32 material_index, uint32& start, uint32& count) const = 0;
		virtual bool IsValidPass(uint32 material_index) const { return true; }
		virtual IndexType GetIndexType() const { return IndexType::U16; }
		const std::vector<Ref<Material>>& GetSharedMaterials() const { return m_shared_materials; }
		void SetSharedMaterials(const std::vector<Ref<Material>>& mats) { m_shared_materials = mats; }
		int GetSortingOrder() const { return m_sorting_order; }
		void SetSortingOrder(uint32 order) { m_sorting_order = order; }
		int GetLightmapIndex() const { return m_lightmap_index; }
		void SetLightmapIndex(uint32 index) { m_lightmap_index = index; }
		const Vector4& GetLightmapScaleOffset() const { return m_lightmap_scale_offset; }
		void SetLightmapScaleOffset(const Vector4& scale_offset) { m_lightmap_scale_offset = scale_offset; }
		void SetBounds(const Bounds& bounds) { m_bounds = bounds; }
		const Bounds& GetBounds() const { return m_bounds; }

	protected:
		Renderer();
		virtual void Start();
		virtual void OnEnable();
		virtual void OnDisable();
		virtual void PreRenderByMaterial(uint32 material_index);
		virtual void PreRenderByRenderer(uint32 material_index);
		virtual Matrix GetWorldMatrix();
		void Render(uint32 material_index, uint32 pass_index);

	private:
		struct MaterialPass
		{
			uint32 queue;
			uint32 shader_pass_count;
			Renderer* renderer;
			uint32 material_index;
			uint32 shader_id;
			uint32 material_id;
		};

		struct Passes
		{
			std::list<std::list<MaterialPass>> list;
			std::list<Renderer*> culled_renderers;
			bool passes_dirty;
			bool culling_dirty;

			Passes() : passes_dirty(true), culling_dirty(true) { }
		};

		struct RenderBuffer
		{
			const VertexBuffer* vb;
			const IndexBuffer* ib;
			uint32 start;
			uint32 count;

			RenderBuffer() :
				vb(NULL),
				ib(NULL),
				start(0),
				count(0)
			{
			}
		};

		struct BatchInfo
		{
			uint32 index_start;
			uint32 index_count;
		};

		static void CheckPasses();
		static void CameraCulling();
		static void BuildPasses(const std::list<Renderer*>& renderers, std::list<std::list<MaterialPass>>& passes);
		static void BuildPasses();
		static void PreparePass(std::list<MaterialPass>& pass);
		static void CommitPass(std::list<MaterialPass>& pass);
		static void BindStaticBuffers();

		static std::list<Renderer*> m_renderers;
		static std::map<Camera*, Passes> m_passes;
		static bool m_renderers_dirty;
		static bool m_passes_dirty;
		static Ref<VertexBuffer> m_static_vertex_buffer;
		static Ref<IndexBuffer> m_static_index_buffer;
		static bool m_static_buffers_binding;
		static int32 m_batching_start;
		static int32 m_batching_count;

	protected:
		std::vector<Ref<Material>> m_shared_materials;
		uint32 m_sorting_order;
		uint32 m_lightmap_index;
		Vector4 m_lightmap_scale_offset;
		Bounds m_bounds;
		std::vector<BatchInfo> m_batch_indices;
		Ref<DescriptorSet> m_descriptor_set;
		Ref<UniformBuffer> m_descriptor_set_buffer;
	};
}

#endif // __KGE_GRAPHICS_RENDERER_H__