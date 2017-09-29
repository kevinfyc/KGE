//
//  mesh_renderer.cpp
//  libkge
//
//  Created by kevin on 17-9-25.
//
//

#include "mesh_renderer.h"

namespace kge
{
	DEFINE_COM_CLASS(MeshRenderer);

	MeshRenderer::MeshRenderer()
	{
	}

	const VertexBuffer* MeshRenderer::GetVertexBuffer() const
	{
		return GetSharedMesh()->GetVertexBuffer().get();
	}

	const IndexBuffer* MeshRenderer::GetIndexBuffer() const
	{
		return GetSharedMesh()->GetIndexBuffer().get();
	}

	void MeshRenderer::GetIndexRange(uint32 material_index, uint32& start, uint32& count) const
	{
		GetSharedMesh()->GetIndexRange(material_index, start, count);
	}

	bool MeshRenderer::IsValidPass(uint32 material_index) const
	{
		if (_mesh)
		{
			uint32 submesh_count = _mesh->submeshes.size();

			if (material_index == 0)
			{
				return true;
			}
			else if (material_index > 0 && submesh_count > 0 && material_index < submesh_count)
			{
				return true;
			}
		}

		return false;
	}
}

