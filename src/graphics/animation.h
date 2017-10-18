//
//  animation.h
//  libkge
//
//  Created by kevin on 17-10-16.
//
//

#ifndef __KGE_GRAPHICS_ANIMATION_H__
#define __KGE_GRAPHICS_ANIMATION_H__

#include "core/component.h"
#include <list>
#include <map>
#include "animation_state.h"

namespace kge
{
	enum class PlayMode
	{
		StopSameLayer = 0,
		StopAll = 4,
	};

	class Animation : public Component
	{
		DECLARE_COM_CLASS(Animation, Component);
	public:
		Animation() { }
		virtual ~Animation() { }
		void SetAnimationStates(const std::map<std::string, AnimationState>& states) { m_states = states; }
		void FindBones();
		void Play(const std::string& clip, PlayMode mode = PlayMode::StopSameLayer);
		void Stop();
		void CrossFade(const std::string& clip, float fade_length = 0.3f, PlayMode mode = PlayMode::StopSameLayer);
		AnimationState GetAnimationState(const std::string& clip) const;
		void UpdateAnimationState(const std::string& clip, const AnimationState& state);

	private:
		struct Blend
		{
			AnimationState* state;
			float weight;

			bool operator <(const Blend& b) const
			{
				return state->layer < b.state->layer;
			}

			Blend() :
				state(0),
				weight(0)
			{
			}
		};

		enum class StateCmdType
		{
			Play,
			Stop,
			CrossFade,
			UpdateState
		};

		struct StateCmd
		{
			StateCmdType type;
			std::string clip;
			PlayMode mode;
			float fade_length;
			AnimationState state;
		};

		virtual void Start();
		virtual void Update();
		void UpdateAnimation();
		void UpdateBlend();
		void UpdateBones();
		void Play(AnimationState& state);
		void Stop(AnimationState& state);
		void ExecuteStateCommands();
		void PlayCmd(const std::string& clip, PlayMode mode);
		void StopCmd();
		void CrossFadeCmd(const std::string& clip, float fade_length, PlayMode mode);

		std::map<std::string, AnimationState> m_states;
		std::list<Blend> m_blends;
		std::map<std::string, WeakRef<Transform>> m_bones;
		std::list<StateCmd> m_state_cmds;
	};

}; // end of namespace kge

#endif // __KGE_GRAPHICS_ANIMATION_H__
