//
//  camera_clear_flag.h
//  libkge
//
//  Created by kevin on 17-9-25.
//
//

#ifndef __KGE_GRAPHICS_CAMERA_CLEAR_FLAG_H__
#define __KGE_GRAPHICS_CAMERA_CLEAR_FLAG_H__

namespace kge
{
	enum class CameraClearFlags
	{
		Invalidate = 1,
		Color = 2,
		Depth = 3,
		Nothing = 4,
	};
}

#endif // __KGE_GRAPHICS_CAMERA_CLEAR_FLAG_H__