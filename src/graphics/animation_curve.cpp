//
//  animation_curve.cpp
//  libkge
//
//  Created by kevin on 17-10-16.
//
//

#include "animation_curve.h"
#include <math.h>
#include "math/vector3.hpp"

namespace kge
{
	AnimationCurve AnimationCurve::DefaultLinear()
	{
		AnimationCurve c;
		c.keys.push_back(Keyframe(0, 0, 1, 1));
		c.keys.push_back(Keyframe(1, 1, 1, 1));

		return c;
	}

	static float evaluate(float time, const Keyframe& k0, const Keyframe& k1)
	{
		float dt = k1.time - k0.time;
		if(almostZero(dt))
			return k0.value;

		float t = (time - k0.time) / dt;
		float t2 = t * t;
		float t3 = t2 * t;
		float _t = 1 - t;
		float _t2 = _t * _t;
		float _t3 = _t2 * _t;

		float c = 1 / 3.0f;
		float c0 = dt * c * k0.out_tangent + k0.value;
		float c1 = -dt * c * k1.in_tangent + k1.value;
		float value = k0.value * _t3 + 3 * c0 * t * _t2 + 3 * c1 * t2 * _t + k1.value * t3;

		return value;
	}

	float AnimationCurve::Evaluate(float time)
	{
		if (keys.empty())
			return 0;

		const auto& back = keys[keys.size() - 1];
		if (time >= back.time)
			return back.value;

		for (uint32 i = 0; i < keys.size(); i++)
		{
			const auto& frame = keys[i];

			if (time < frame.time)
				return i == 0 ? frame.value : evaluate(time, keys[i - 1], frame);
		}

		return 0;
	}
}

