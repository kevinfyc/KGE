//
//  input.h
//  libkge
//
//  Created by kevin on 17-10-10.
//
//

#ifndef __KGE_CORE_INPUT_H__
#define __KGE_CORE_INPUT_H__

#include "math/vector2.hpp"
#include "math/vector3.hpp"
#include "key_code.h"

namespace kge
{

	enum class TouchPhase
	{
		Began,          //A finger touched the screen.
		Moved,          //A finger moved on the screen.
		Stationary,     //A finger is touching the screen but hasn't moved.
		Ended,          //A finger was lifted from the screen. This is the final phase of a touch.
		Canceled,       //The system cancelled tracking for the touch.
	};
	
	struct Touch
	{
		Vector2 deltaPosition;
		float time;
		float deltaTime;
		uint32 fingerID;
		TouchPhase phase;
		Vector2 position;
		uint32 tapCount;

		Touch() { }
	};

	class Input
	{
	public:
		static bool IsMultiTouchEnabled() { return s_multi_touch_enabled; }
		static void EnableMultiTouch(bool value);
		static int GetTouchCount();
		static const Touch *GetTouch(uint32 index);
		static void Update();
		static bool GetKeyDown(KeyCode key);
		static bool GetKey(KeyCode key);
		static bool GetKeyUp(KeyCode key);
		static bool GetMouseButtonDown(uint32 index);
		static bool GetMouseButton(uint32 index);
		static bool GetMouseButtonUp(uint32 index);
		static Vector3 GetMousePosition();
		static void SetMousePositionScaleOffset(const Vector3 &scale, const Vector3 &offset);
		static void ResetInputAxes();

	private:
		static bool s_multi_touch_enabled;
	};
}

#endif // __KGE_CORE_INPUT_H__
