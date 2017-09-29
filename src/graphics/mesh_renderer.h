//
//  mesh_renderer.h
//  libkge
//
//  Created by kevin on 17-9-25.
//
//

#ifndef __KGE_GRAPHICS_MESH_RENDERER_H__
#define __KGE_GRAPHICS_MESH_RENDERER_H__

#include "renderer.h"
#include "mesh.h"

namespace kge
{
	class MeshRenderer : public Renderer
	{
		DECLARE_COM_CLASS(MeshRenderer, Renderer)
	public:
		virtual const VertexBuffer* GetVertexBuffer() const;
		virtual const IndexBuffer* GetIndexBuffer() const;
		virtual void GetIndexRange(uint32 material_index, uint32& start, uint32& count) const;
		virtual bool IsValidPass(uint32 material_index) const;

		void SetSharedMesh(const Ref<Mesh>& mesh) { _mesh = mesh; }
		const Ref<Mesh>& GetSharedMesh() const { return _mesh; }

	private:
		MeshRenderer();

	private:
		Ref<Mesh> _mesh;
	};
}

#endif // __KGE_GRAPHICS_MESH_RENDERER_H__