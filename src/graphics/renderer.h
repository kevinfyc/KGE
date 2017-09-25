//
//  renderer.h
//  libkge
//
//  Created by kevin on 17-9-25.
//
//

#ifndef __KGE_GRAPHICS_RENDERER_H__
#define __KGE_GRAPHICS_RENDERER_H__

#include "core/component.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "math/bounds.h"

namespace kge
{
	class Renderer : public Component
	{
		DECLARE_COM_CLASS_ABSTRACT(Renderer, Component)
	};
}

#endif // __KGE_GRAPHICS_RENDERER_H__