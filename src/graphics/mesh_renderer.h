//
//  mesh_renderer.h
//  libkge
//
//  Created by kevin on 17-9-25.
//
//

#ifndef __KGE_GRAPHICS_MESH_RENDERER_H__
#define __KGE_GRAPHICS_MESH_RENDERER_H__

#include "renderer.h"

namespace kge
{
	class MeshRenderer : public Renderer
	{
		DECLARE_COM_CLASS(MeshRenderer, Renderer)
	};
}

#endif // __KGE_GRAPHICS_MESH_RENDERER_H__