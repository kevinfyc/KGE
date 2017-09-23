//
//  buffer_GLES.cpp
//  libkge
//
//  Created by kevin on 17-9-23.
//
//

#include "buffer_GLES.h"
#include "util/log.h"

namespace kge
{
	BufferGLES::BufferGLES() :_size(0)
		, _buffer(0)
		, _type(0)
		, _usage(0)
	{
	
	}

	BufferGLES::~BufferGLES()
	{
		glDeleteBuffers(1, &_buffer);
	}

	const Ref<ByteBuffer>& BufferGLES::GetLocalBuffer()
	{
		if (!_local_buffer)
			_local_buffer = RefMake<ByteBuffer>(_size);

		return _local_buffer;
	}

	void BufferGLES::Create(BufferType type, bool dynamic)
	{
		KGE_LOG_GL_ERROR();
		switch (type)
		{
		case kge::BufferType::Vertex:
			_type = GL_ARRAY_BUFFER;
			_usage = dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;
			break;
		case kge::BufferType::Index:
			_type = GL_ELEMENT_ARRAY_BUFFER;
			_usage = dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;
			break;
		default:
			_type = _usage = 0;
			break;
		}

		glGenBuffers(1, &_buffer);

		if (_usage == GL_DYNAMIC_DRAW)
		{
			glBindBuffer(_type, _buffer);
			glBufferData(_type, _size, nullptr, _usage);
			glBindBuffer(_type, 0);
		}

		KGE_LOG_GL_ERROR();
	}

	void BufferGLES::UpdateRange(int offset, int size, const void* data)
	{
		KGE_LOG_GL_ERROR();

		if (_usage == GL_DYNAMIC_DRAW)
		{
			glBindBuffer(_type, _buffer);
			glBufferSubData(_type, offset, size, data);
			glBindBuffer(_type, 0);
		}

		KGE_LOG_GL_ERROR();
	}

	void BufferGLES::Fill(void* param, FillFunc fill)
	{
		KGE_LOG_GL_ERROR();

		glBindBuffer(_type, _buffer);

		if (_usage == GL_DYNAMIC_DRAW)
		{
			auto& buffer = *GetLocalBuffer().get();
			fill(param, buffer);
			glBufferSubData(_type, 0, _size, buffer.Bytes());
		}
		else
		{
			ByteBuffer buffer(_size);
			fill(param, buffer);
			glBufferData(_type, _size, buffer.Bytes(), _usage);
		}

		glBindBuffer(_type, 0);

		KGE_LOG_GL_ERROR();
	}
}

