//
//  uniform_buffer.h
//  libkge
//
//  Created by kevin on 17-9-27.
//
//

#ifndef __KGE_GRAPHICS_UNIFORM_BUFFER_H__
#define __KGE_GRAPHICS_UNIFORM_BUFFER_H__

#if KGE_GLES
#include "buffer_GLES.h"
#endif

namespace kge
{
#if KGE_GLES
	class UniformBuffer: public BufferGLES
	{
#endif
	public:
		static Ref<UniformBuffer> Create(int size);

	private:
		UniformBuffer() { }
	};
} // end of namespace kge

#endif // __KGE_GRAPHICS_UNIFORM_BUFFER_H__
