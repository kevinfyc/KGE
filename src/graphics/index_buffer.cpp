//
//  index_buffer.cpp
//  libkge
//
//  Created by kevin on 17-9-23.
//
//

#include "index_buffer.h"

namespace kge
{
	Ref<IndexBuffer> IndexBuffer::Create(int size, bool dynamic/*=false*/)
	{
		Ref<IndexBuffer> buffer(new IndexBuffer());

		buffer->_size = size;
		buffer->_Create(BufferType::Index, dynamic);

		return buffer;
	}
}

