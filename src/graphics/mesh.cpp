//
//  mesh.cpp
//  libkge
//
//  Created by kevin on 17-9-23.
//
//

#include "mesh.h"
#include "io/memory_stream.h"
#include "vertex_attribute.h"

namespace kge
{
	Mesh::Mesh():_dynamic(false)
	{
		SetName("Mesh");
	}

	Ref<Mesh> Mesh::Create(bool dynamic/*=false*/)
	{
		Ref<Mesh> mesh(new Mesh);

		mesh->_dynamic = dynamic;

		return mesh;
	}

	void Mesh::Tick()
	{
		UpdateVertexBuffer();
		UpdateIndexBuffer();
	}

	void Mesh::UpdateVertexBuffer()
	{
		uint32 buffer_size = VertexBufferSize();
		bool dynamic = IsDynamic();

		if (!_vertex_buffer || _vertex_buffer->GetSize() < buffer_size)
			_vertex_buffer = VertexBuffer::Create(buffer_size, dynamic);

		_vertex_buffer->Fill(this, Mesh::FillVertexBuffer);
	}

	void Mesh::UpdateIndexBuffer()
	{
		uint32 buffer_size = IndexBufferSize();
		bool dynamic = IsDynamic();

		if (!_index_buffer || _index_buffer->GetSize() < buffer_size)
			_index_buffer = IndexBuffer::Create(buffer_size, dynamic);

		_index_buffer->Fill(this, Mesh::FillIndexBuffer);
	}

	uint32 Mesh::VertexBufferSize() const
	{
		return VERTEX_STRIDE * vertices.size();
	}

	uint32 Mesh::IndexBufferSize() const
	{
		return triangles.size() * sizeof(uint16);
	}

	void Mesh::FillVertexBuffer(void* param, const ByteBuffer& buffer)
	{
		Mesh* mesh = (Mesh*)param;
		MemoryStream ms = MemoryStream(buffer);

		uint32 count = mesh->vertices.size();
		for (uint32 i = 0; i < count; ++i)
		{
			ms.Write<Vector3>(mesh->vertices[i]);

			if (mesh->colors.empty())
				ms.Write<Color>(Color(1, 1, 1, 1));
			else
				ms.Write<Color>(mesh->colors[i]);

			if (mesh->uv.empty())
				ms.Write<Vector2>(Vector2(0, 0));
			else
				ms.Write<Vector2>(mesh->uv[i]);

			if (mesh->uv2.empty())
				ms.Write<Vector2>(Vector2(0, 0));
			else
				ms.Write<Vector2>(mesh->uv2[i]);

			if (mesh->normals.empty())
				ms.Write<Vector3>(Vector3(0, 0, 0));
			else
				ms.Write<Vector3>(mesh->normals[i]);

			if (mesh->tangents.empty())
				ms.Write<Vector4>(Vector4(0, 0, 0, 0));
			else
				ms.Write<Vector4>(mesh->tangents[i]);

			if (mesh->bone_weights.empty())
				ms.Write<Vector4>(Vector4(0, 0, 0, 0));
			else
				ms.Write<Vector4>(mesh->bone_weights[i]);

			if (mesh->bone_indices.empty())
				ms.Write<Vector4>(Vector4(0, 0, 0, 0));
			else
				ms.Write<Vector4>(mesh->bone_indices[i]);
		}

		ms.Close();
	}

	void Mesh::FillIndexBuffer(void* param, const ByteBuffer& buffer)
	{
		Mesh* mesh = (Mesh*)param;
		::memcpy(buffer.Bytes(), (void*)&mesh->triangles[0], mesh->IndexBufferSize());
	}

	void Mesh::GetIndexRange(uint32 submesh_index, uint32& start, uint32& count)
	{
		if (submeshes.empty())
		{
			start = 0;
			count = triangles.size();
		}
		else
		{
			SubMesh submesh = submeshes[submesh_index];

			start = submesh.start;
			count = submesh.count;
		}
	}
}

