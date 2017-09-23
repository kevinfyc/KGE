//
//  frame_buffer.h
//  libkge
//
//  Created by kevin on 17-9-23.
//
//

#ifndef __KGE_GRAPHICS_FRAME_BUFFER_H__
#define __KGE_GRAPHICS_FRAME_BUFFER_H__

#include "core/pre_define.h"

namespace kge
{
	class RenderTexture;

	struct FrameBuffer
	{
		Ref<RenderTexture> color_texture;
		Ref<RenderTexture> depth_texture;
	};
}

#endif // __KGE_GRAPHICS_FRAME_BUFFER_H__