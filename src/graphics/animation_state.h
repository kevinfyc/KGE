//
//  animation_state.h
//  libkge
//
//  Created by kevin on 17-10-16.
//
//

#ifndef __KGE_GRAPHICS_ANIMATION_STATE_H__
#define __KGE_GRAPHICS_ANIMATION_STATE_H__

#include <string>
#include "animation_wrap_mode.h"
#include "animation_clip.h"

namespace kge
{
	enum class AnimationBlendMode
	{
		Blend = 0,
		Additive = 1,
	};

	enum class AnimationFadeMode
	{
		None,
		In,
		Out,
	};

	struct AnimationFade
	{
		AnimationFadeMode mode;
		float length;
		float weight;

		AnimationFade() :
			mode(AnimationFadeMode::None),
			length(0),
			weight(0)
		{
		}

		void Clear()
		{
			mode = AnimationFadeMode::None;
			length = 0;
			weight = 0;
		}
	};

	struct AnimationState
	{
		AnimationState() { }
		AnimationState(Ref<AnimationClip>& clip) :
			name(clip->GetName()),
			clip(clip),
			blend_mode(AnimationBlendMode::Blend),
			enabled(false),
			layer(0),
			length(clip->length),
			normalized_speed(1 / clip->length),
			normalized_time(0),
			speed(1),
			time(0),
			weight(1),
			wrap_mode(AnimationWrapMode::Default),
			time_last(0),
			play_dir(1)
		{
		}

		std::string name;
		Ref<AnimationClip> clip;
		AnimationBlendMode blend_mode;
		bool enabled;
		int layer;
		float length;
		float normalized_speed;
		float normalized_time;
		float speed;
		float time;
		float weight;
		AnimationWrapMode wrap_mode;

		float time_last;
		int play_dir;
		AnimationFade fade;
	};
}

#endif // __KGE_GRAPHICS_ANIMATION_STATE_H__