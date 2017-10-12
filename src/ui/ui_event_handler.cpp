//
//  ui_event_handler.cpp
//  libkge
//
//  Created by kevin on 17-10-12.
//
//
// 

#include "ui_event_handler.h"
#include "ui_canvas_renderer.h"
#include "ui_view.h"

#include "core/input.h"
#include "core/transform.h"

#include "graphics/graphics.h"
#include "graphics/display.h"


namespace kge
{
	static std::map<uint32, std::vector<WeakRef<UIView>>> g_hit_views;

	static std::vector<WeakRef<UIView>> hit_test(Vector2 position, const std::vector<Ref<UIView>>& views)
	{
		std::vector<WeakRef<UIView>> hit_views;

		Vector2 pos_world(position.x - Graphics::GetDisplay()->GetWidth() * 0.5f, position.y - Graphics::GetDisplay()->GetHeight() * 0.5f);

		for (const Ref<UIView>& view : views)
		{
			auto mat = view->GetRenderer().lock()->GetTransform()->GetLocal2WorldMatrix();
			auto vertices = view->GetBoundsVertices();

			for (uint32 i = 0; i < vertices.size(); ++i)
				vertices[i] = mat.applyPoint(vertices[i]);

			if (pos_world.x > vertices[0].x &&
				pos_world.x < vertices[1].x &&
				pos_world.y > vertices[1].y &&
				pos_world.y < vertices[2].y)
				hit_views.push_back(view);
		}

		return hit_views;
	}

	void UIEventHandler::HandleUIEvent(const std::list<UICanvasRenderer*>& list)
	{
		int touch_count = Input::GetTouchCount();
		if (touch_count == 0)
			return;

		std::vector<Ref<UIView>> views;
		for (auto i : list)
		{
			i->FindViews();
			auto& vs = i->GetViews();

			for (auto& j : vs)
				views.push_back(j);
		}

		for (int i = 0; i < touch_count; i++)
		{
			auto t = Input::GetTouch(i);

			UIPointerEvent e;
			e.position = t->position;

			if (t->phase == TouchPhase::Began)
			{
				if (g_hit_views.count(t->fingerID) <= 0)
					g_hit_views.insert(std::pair<uint32, std::vector<WeakRef<UIView>>>(t->fingerID, std::vector<WeakRef<UIView>>()));
				auto& pointer_views = g_hit_views[t->fingerID];

				auto hit_views = hit_test(t->position, views);
				for (int j = hit_views.size() - 1; j >= 0; j--)
				{
					auto event_handler = hit_views[j].lock()->event_handler;
					if (event_handler.enable)
					{
						// send down event to top view in down
						auto on_pointer_down = event_handler.on_pointer_down;
						if (on_pointer_down)
							on_pointer_down(hit_views[j], e);

						pointer_views.push_back(hit_views[j]);
					}
				}
			}
			else if (t->phase == TouchPhase::Moved)
			{
				auto& pointer_views = g_hit_views[t->fingerID];
				auto hit_views = hit_test(t->position, views);
				for (int j = hit_views.size() - 1; j >= 0; j--)
				{
					auto v = hit_views[j].lock();

					if (v->event_handler.enable && v->GetDrag())
					{
						// send drag begin to top view
						for (auto& k : pointer_views)
						{
							if (!k.expired() && k.lock() == v)
							{
								auto on_drag_begin = v->event_handler.on_drag_begin;
								if (on_drag_begin && !v->GetDraging())
								{
									on_drag_begin(hit_views[j], e);
									v->SetDraging(true);
								}
								break;
							}
						}

						// send drag to top view
						auto on_drag = v->event_handler.on_drag;
						if (on_drag && v->GetDraging())
							on_drag(hit_views[j], e);

						break;
					}
				}
			}
			else if (t->phase == TouchPhase::Ended || t->phase == TouchPhase::Canceled)
			{
				auto& pointer_views = g_hit_views[t->fingerID];

				auto hit_views = hit_test(t->position, views);
				for (int j = hit_views.size() - 1; j >= 0; j--)
				{
					auto v = hit_views[j].lock();

					if (v->event_handler.enable)
					{
						// send up event to top view in up
						auto on_pointer_up = v->event_handler.on_pointer_up;
						if (on_pointer_up)
							on_pointer_up(hit_views[j], e);

						// send drag end to top view
						auto on_drag_end = v->event_handler.on_drag_end;
						if (on_drag_end && v->GetDraging())
						{
							on_drag_end(hit_views[j], e);;
							v->SetDraging(false);
						}

						// send click event to top view in down and in up
						for (auto& k : pointer_views)
						{
							if (!k.expired() && k.lock() == v)
							{
								auto on_pointer_click = v->event_handler.on_pointer_click;
								if (on_pointer_click)
									on_pointer_click(hit_views[j], e);
								break;
							}
						}

						break;
					}
				}

				for (auto& j : pointer_views)
				{
					auto v = j.lock();
					bool not_hit = true;

					for (auto& k : hit_views)
					{
						if (v == k.lock())
						{
							not_hit = false;
							break;
						}
					}

					if (not_hit)
					{
						if (v->event_handler.enable)
						{
							// send up event to top view in down but not in up
							auto on_pointer_up = v->event_handler.on_pointer_up;
							if (on_pointer_up)
								on_pointer_up(j, e);

							// send drag end to top view
							auto on_drag_end = v->event_handler.on_drag_end;
							if (on_drag_end && v->GetDraging())
							{
								on_drag_end(j, e);;
								v->SetDraging(false);
							}

							break;
						}
					}
				}

				g_hit_views.erase(t->fingerID);
			}
		}
	}
}
