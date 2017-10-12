//
//  ui_event_handler.h
//  libkge
//
//  Created by kevin on 17-10-12.
//
//
// 

#ifndef __KGE_UI_EVENT_HANDLER_H__
#define __KGE_UI_EVENT_HANDLER_H__

#include "math/vector2.hpp"
#include <list>
#include <functional>
#include "core/object.h"

namespace kge
{
	class UICanvasRenderer;

	struct UIPointerEvent
	{
		Vector2 position;
	};

	class UIEventHandler
	{
	public:
		static void HandleUIEvent(const std::list<UICanvasRenderer*>& list);

	public:
		bool enable = false;
		std::function<void(const WeakRef<Object>& obj, UIPointerEvent& e)> on_pointer_down;
		std::function<void(const WeakRef<Object>& obj, UIPointerEvent& e)> on_pointer_up;
		std::function<void(const WeakRef<Object>& obj, UIPointerEvent& e)> on_pointer_click;
	};
}

#endif // __KGE_UI_EVENT_HANDLER_H__
