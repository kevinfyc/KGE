//
//  render_queue.h
//  libkge
//
//  Created by kevin on 17-9-26.
//
//

#ifndef __KGE_GRAPHICS_RENDER_QUEUE_H__
#define __KGE_GRAPHICS_RENDER_QUEUE_H__

namespace kge
{
	enum class RenderQueue
	{
		Background = 1000,
		Geometry = 2000,
		AlphaTest = 2450,
		Transparent = 3000,
		Overlay = 4000,
	};
} // end of namespace kge

#endif // __KGE_GRAPHICS_RENDER_QUEUE_H__