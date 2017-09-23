//
//  depth_buffer.h
//  libkge
//
//  Created by kevin on 17-9-21.
//
//

#ifndef __KGE_GRAPHICS_DEPTH_BUFFER_H__
#define __KGE_GRAPHICS_DEPTH_BUFFER_H__

namespace kge
{
	enum class DepthBuffer
	{
		Depth_0,
		Depth_16,
		Depth_24,
		Depth_24_Stencil_8,
		Depth_32,
	};
} // end of namespace kge

#endif // __KGE_GRAPHICS_DEPTH_BUFFER_H__