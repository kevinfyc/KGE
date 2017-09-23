//
//  filter_mode.h
//  libkge
//
//  Created by kevin on 17-9-23.
//
//

#ifndef __KGE_GRAPHICS_FILTER_MODE_H__
#define __KGE_GRAPHICS_FILTER_MODE_H__

namespace kge
{
	enum class FilterMode
	{
		Point = 0,
		Bilinear = 1,
		Trilinear = 2,
	};
}

#endif // __KGE_GRAPHICS_FILTER_MODE_H__