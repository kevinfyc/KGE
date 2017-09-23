//
//  vertex_buffer.cpp
//  libkge
//
//  Created by kevin on 17-9-23.
//
//

#include "vertex_buffer.h"

namespace kge
{
	Ref<VertexBuffer> VertexBuffer::Create(int size, bool dynamic/*=false*/)
	{
		Ref<VertexBuffer> buffer(new VertexBuffer());

		buffer->_size = size;
		buffer->_Create(BufferType::Vertex, dynamic);

		return buffer;
	}
}

