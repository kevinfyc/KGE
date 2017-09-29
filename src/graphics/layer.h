//
//  layer.h
//  libkge
//
//  Created by kevin on 17-9-29.
//
//

#ifndef __KGE_GRAPHICS_LAYER_H__
#define __KGE_GRAPHICS_LAYER_H__

namespace kge
{
	enum class Layer
	{
		Default = 0,
		TransparentFX = 1,
		IgnoreRaycast = 2,
		Highlighting = 3,
		Water = 4,
		UI = 5,
		Scene = 6,
		Terrain = 7,
		Character = 8,
		Editor = 31
	};
}

#endif // __KGE_GRAPHICS_LAYER_H__