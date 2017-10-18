//
//  animation_clip.h
//  libkge
//
//  Created by kevin on 17-10-16.
//
//

#ifndef __KGE_GRAPHICS_ANIMATION_CLIP_H__
#define __KGE_GRAPHICS_ANIMATION_CLIP_H__

#include "core/object.h"
#include <string>
#include "animation_wrap_mode.h"
#include "animation_curve.h"

namespace kge
{
	enum class CurveProperty
	{
		LocalPosX,
		LocalPosY,
		LocalPosZ,
		LocalRotX,
		LocalRotY,
		LocalRotZ,
		LocalRotW,
		LocalScaX,
		LocalScaY,
		LocalScaZ,

		Count
	};

	struct CurveBinding
	{
		std::string path;
		std::vector<AnimationCurve> curves;
	};

	class AnimationClip : public Object
	{
	public:
		float frame_rate;
		float length;
		AnimationWrapMode wrap_mode;
		std::map<std::string, CurveBinding> curves;
	};
}

#endif // __KGE_GRAPHICS_ANIMATION_CLIP_H__