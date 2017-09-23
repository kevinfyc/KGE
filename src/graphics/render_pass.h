//
//  render_pass.h
//  libkge
//
//  Created by kevin on 17-9-23.
//
//

#ifndef __KGE_GRAPHICS_RENDER_PASS_H__
#define __KGE_GRAPHICS_RENDER_PASS_H__

#if KGE_GLES
#include "render_pass_GLES.h"
#endif

#include "frame_buffer.h"

namespace kge
{
#if KGE_GLES
	class RenderPass : public RenderPassGLES
#endif
	{
	public:
		// for temp
		bool HasFrameBuffer() { return false; }
	};
}

#endif // __KGE_GRAPHICS_RENDER_PASS_H__