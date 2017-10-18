//
//  animation_curve.h
//  libkge
//
//  Created by kevin on 17-10-16.
//
//

#ifndef __KGE_GRAPHICS_ANIMATION_CURVE_H__
#define __KGE_GRAPHICS_ANIMATION_CURVE_H__

#include <vector>
#include "animation_wrap_mode.h"

namespace kge
{
	struct Keyframe
	{
		Keyframe()
		{
		}
		
		Keyframe(float time, float value) : in_tangent(0)
			, out_tangent(0)
			, tangent_mode(0)
			, time(time)
			, value(value)
		{
		}

		Keyframe(float time, float value, float in_tangent, float out_tangent) : in_tangent(in_tangent)
			, out_tangent(out_tangent)
			, tangent_mode(0)
			, time(time)
			, value(value)
		{
		}

		float in_tangent;
		float out_tangent;
		int tangent_mode;
		float time;
		float value;
	};

	struct AnimationCurve
	{
		static AnimationCurve DefaultLinear();
		float Evaluate(float time);

		std::vector<Keyframe> keys;
	};
}

#endif // __KGE_GRAPHICS_ANIMATION_CURVE_H__
