//
//  input.cpp
//  libkge
//
//  Created by kevin on 17-10-10.
//
//

#include "input.h"
#include <vector>
#include <list>
#include "math/_math.hpp"

std::vector<kge::Touch> g_input_touches;
std::list<kge::Touch> g_input_touch_buffer;
bool g_key_down[(kge::uint32)kge::KeyCode::COUNT];
bool g_key[(kge::uint32)kge::KeyCode::COUNT];
bool g_key_up[(kge::uint32)kge::KeyCode::COUNT];
bool g_key_held[(kge::uint32)kge::KeyCode::COUNT];
bool g_mouse_button_down[3];
bool g_mouse_button_up[3];
kge::Vector3 g_mouse_position;
kge::Vector3 g_mouse_position_scale(1, 1, 1);
kge::Vector3 g_mouse_position_offset(0, 0, 0);
bool g_mouse_button_held[3];

namespace kge
{
	bool Input::s_multi_touch_enabled = false;

	bool Input::GetMouseButtonDown(uint32 index)
	{
		return g_mouse_button_down[index];
	}

	bool Input::GetMouseButton(uint32 index)
	{
		return g_mouse_button_held[index];
	}

	bool Input::GetMouseButtonUp(uint32 index)
	{
		return g_mouse_button_up[index];
	}

	Vector3 Input::GetMousePosition()
	{
		return Vector3(
			g_mouse_position.x * g_mouse_position_scale.x + g_mouse_position_offset.x,
			g_mouse_position.y * g_mouse_position_scale.y + g_mouse_position_offset.y,
			0);
	}

	void Input::SetMousePositionScaleOffset(const Vector3 &scale, const Vector3 &offset)
	{
		g_mouse_position_scale = scale;
		g_mouse_position_offset = offset;
	}

	int Input::GetTouchCount()
	{
		return g_input_touches.size();
	}

	const Touch *Input::GetTouch(uint32 index)
	{
		if (index >= 0 && index < g_input_touches.size())
		{
			return &g_input_touches[index];
		}

		return nullptr;
	}

	bool Input::GetKeyDown(KeyCode key)
	{
		return g_key_down[(uint32)key];
	}

	bool Input::GetKey(KeyCode key)
	{
		return g_key[(uint32)key];
	}

	bool Input::GetKeyUp(KeyCode key)
	{
		return g_key_up[(uint32)key];
	}

	void Input::Update()
	{
		g_input_touches.clear();
		if (!g_input_touch_buffer.empty())
		{
			g_input_touches.push_back(g_input_touch_buffer.front());
			g_input_touch_buffer.pop_front();
		}

		::memset(g_key_down, 0, sizeof(g_key_down));
		::memset(g_key_up, 0, sizeof(g_key_up));
		::memset(g_mouse_button_down, 0, sizeof(g_mouse_button_down));
		::memset(g_mouse_button_up, 0, sizeof(g_mouse_button_up));
	}

	void Input::EnableMultiTouch(bool value)
	{
	}

	void Input::ResetInputAxes()
	{
	}

} // end namespace kge
