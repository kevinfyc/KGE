//
//  uniform_buffer.cpp
//  libkge
//
//  Created by kevin on 17-9-27.
//
//


#include "uniform_buffer.h"

namespace kge
{
	Ref<UniformBuffer> UniformBuffer::Create(int size)
	{
		Ref<UniformBuffer> buffer(new UniformBuffer());

		buffer->_size = size;
		buffer->_Create(BufferType::Uniform);

		return buffer;
	}
}
