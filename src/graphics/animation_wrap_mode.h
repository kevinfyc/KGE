//
//  animation_wrap_mode.h
//  libkge
//
//  Created by kevin on 17-10-16.
//
//

#ifndef __KGE_GRAPHICS_ANIMATION_WRAP_MODE_H__
#define __KGE_GRAPHICS_ANIMATION_WRAP_MODE_H__

namespace kge
{
	enum class AnimationWrapMode
	{
		Default = 0,
		Once = 1,
		Clamp = 1,
		Loop = 2,
		PingPong = 4,
		ClampForever = 8,
	};
}

#endif // __KGE_GRAPHICS_ANIMATION_WRAP_MODE_H__