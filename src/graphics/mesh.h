//
//  mesh.h
//  libkge
//
//  Created by kevin on 17-9-23.
//
//

#ifndef __KGE_GRAPHICS_MESH_H__
#define __KGE_GRAPHICS_MESH_H__

#include "core/object.h"
#include "vertex_buffer.h"
#include "index_buffer.h"

#include "math/vector2.hpp"
#include "math/vector3.hpp"
#include "math/matrix.hpp"

#include "color.h"

#include "common.h"

namespace kge
{
	const uint32 VERTEX_STRIDE = 9 * 4;

	class Mesh : public Object
	{
	public:
		static Ref<Mesh> Create(bool dynamic = false);

		void Tick();

		const Ref<VertexBuffer>& GetVertexBuffer()const { return _vertex_buffer; }
		const Ref<IndexBuffer>& GetIndexBuffer()const { return _index_buffer; }

		void GetIndexRange(uint32 submesh_index, uint32& start, uint32& count);

		bool IsDynamic()const { return _dynamic; }

		std::vector<Vector3> vertices;
		std::vector<Vector2> uv;
		std::vector<Color> colors;

		struct SubMesh
		{
			uint32 start;
			uint32 count;
		};

		std::vector<uint16> triangles;
		std::vector<SubMesh> submeshes;

	private:
		static void FillVertexBuffer(void* param, const ByteBuffer& buffer);
		static void FillIndexBuffer(void* param, const ByteBuffer& buffer);

		Mesh();
		void UpdateVertexBuffer();
		void UpdateIndexBuffer();

		uint32 VertexBufferSize()const;
		uint32 IndexBufferSize()const;

	private:
		bool _dynamic;

		Ref<VertexBuffer> _vertex_buffer;
		Ref<IndexBuffer> _index_buffer;
	};
}

#endif // __KGE_GRAPHICS_MESH_H__