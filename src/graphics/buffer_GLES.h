//
//  buffer_GLES.h
//  libkge
//
//  Created by kevin on 17-9-23.
//
//

#ifndef __KGE_GRAPHICS_BUFFER_GLES_H__
#define __KGE_GRAPHICS_BUFFER_GLES_H__

#include "common.h"
#include "buffer_type.h"
#include "util/byte_buffer.h"

#include <functional>

namespace kge
{
    class BufferGLES
    {
	public:
		~BufferGLES();
		GLuint GetBuffer()const { return _buffer; }
		uint32 GetSize()const { return _size; }
		const Ref<ByteBuffer>& GetLocalBuffer();


		typedef std::function<void(void* param, const ByteBuffer& buffer)> FillFunc;
		void Fill(void* param, FillFunc fill);
		void UpdateRange(int offset, int size, const void* data);

	protected:
		BufferGLES();
		void _Create(BufferType type, bool dynamic);

		uint32 _size;

	private:
		GLuint _buffer;
		GLenum _type;
		GLenum _usage;
		Ref<ByteBuffer> _local_buffer;
    };
}

#endif // __KGE_GRAPHICS_BUFFER_GLES_H__
