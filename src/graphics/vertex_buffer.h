//
//  vertex_bufer.h
//  libkge
//
//  Created by kevin on 17-9-23.
//
//

#ifndef __KGE_GRAPHICS_VERTEX_BUFFER_H__
#define __KGE_GRAPHICS_VERTEX_BUFFER_H__

#if KGE_GLES
#include "buffer_GLES.h"
#endif

namespace kge
{
#if KGE_GLES
	class VertexBuffer : public BufferGLES
	{
#endif
	public:
		static Ref<VertexBuffer> Create(int size, bool dynamic = false);

	private:
		VertexBuffer() {}
	};
}

#endif // __KGE_GRAPHICS_VERTEX_BUFFER_H__