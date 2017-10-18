//
//  skinned_mesh_renderer.h
//  libkge
//
//  Created by kevin on 17-10-16.
//
//

#ifndef __KGE_GRAPHICS_SKINNED_MESH_RENDERER_H__
#define __KGE_GRAPHICS_SKINNED_MESH_RENDERER_H__

#include "renderer.h"
#include "mesh.h"

#define BONE_MAX 80

namespace kge
{
	class SkinnedMeshRenderer : public Renderer
	{
		DECLARE_COM_CLASS(SkinnedMeshRenderer, Renderer);
	public:
		virtual const VertexBuffer* GetVertexBuffer() const;
		virtual const IndexBuffer* GetIndexBuffer() const;
		virtual void GetIndexRange(uint32 material_index, uint32& start, uint32& count) const;
		virtual bool IsValidPass(uint32 material_index) const;
		const Ref<Mesh>& GetSharedMesh() const { return m_mesh; }
		void SetSharedMesh(const Ref<Mesh>& mesh) { m_mesh = mesh; }
		const std::vector<WeakRef<Transform>>& GetBones() const { return m_bones; }
		std::vector<WeakRef<Transform>>& GetBones() { return m_bones; }
		void SetBones(const std::vector<WeakRef<Transform>>& bones) { m_bones = bones; }

	protected:
		virtual void PreRenderByRenderer(uint32 material_index);

	private:
		SkinnedMeshRenderer();

	private:
		Ref<Mesh> m_mesh;
		std::vector<WeakRef<Transform>> m_bones;
	};
}

#endif // __KGE_GRAPHICS_SKINNED_MESH_RENDERER_H__