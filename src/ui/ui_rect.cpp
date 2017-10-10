//
//  ui_rect.cpp
//  libkge
//
//  Created by kevin on 17-10-10.
//
//
// 

#include "ui_rect.h"
#include "core/component.h"
#include "core/transform.h"
#include "core/game_object.h"

namespace kge
{
	UIRect::UIRect() :_anchor_min(0.5f, 0.5f)
		, _anchor_max(.5f, .5f)
		, _offset_min(-50.f, -50.f)
		, _offset_max(50.f, 50.f)
		, _pivot(.5f, .5f)
		, _dirty(true)
	{

	}

	void UIRect::SetAnchors(const Vector2& min, const Vector2& max)
	{
		if (_anchor_min != min || _anchor_max != max)
		{
			_anchor_max = max; _anchor_min = min;
			_dirty = true;
		}
	}

	void UIRect::SetOffsets(const Vector2& min, const Vector2& max)
	{
		if (_offset_min != min || _offset_max != max)
		{
			_offset_max = max; _offset_min = min;
			_dirty = true;
		}
	}

	void UIRect::SetPivot(const Vector2& pivot)
	{
		if (_pivot != pivot)
		{
			_pivot = pivot;
			_dirty = true;
		}
	}

	void UIRect::SetSize(const Vector2& size)
	{
		if (GetSize() != size)
		{
			Ref<UIRect> parent = GetParentRect();
			if (parent)
			{
				const Component* c = dynamic_cast<const Component*>(this);
				Vector3 pos = c->GetTransform()->GetLocalPosition();
				float min_x = pos.x - _pivot.x * size.x;
				float min_y = pos.y - _pivot.y * size.y;
				float max_x = pos.x + (1.f - _pivot.x) * size.x;
				float max_y = pos.y + (1.f - _pivot.y) * size.y;

				Vector2 parent_size = parent->GetSize();
				float parent_x = parent->_pivot.x * parent_size.x;
				float parent_y = parent->_pivot.y * parent_size.y;

				_offset_min.x = min_x + parent_x - _anchor_min.x * parent_size.x;
				_offset_min.y = min_y + parent_y - _anchor_min.y * parent_size.y;
				_offset_max.x = max_x + parent_x - _anchor_max.x * parent_size.x;
				_offset_max.y = max_y + parent_y - _anchor_max.y * parent_size.y;
			}
			else
			{
				_offset_min = Vector2(size.x * -_pivot.x, size.y * -_pivot.y);
				_offset_max = Vector2(size.x * (1.f - _pivot.x), size.y * (1.f - _pivot.y));
			}

			_dirty = true;
		}
	}

	Vector2 UIRect::GetSize() const
	{
		Vector2 size;
		Ref<UIRect> parent = GetParentRect();
		if (parent)
		{
			Vector2 parent_size = parent->GetSize();
			float min_x = _anchor_min.x * parent_size.x + _offset_min.x;
			float min_y = _anchor_min.y * parent_size.y + _offset_min.y;
			float max_x = _anchor_max.x * parent_size.x + _offset_max.x;
			float max_y = _anchor_max.y * parent_size.y + _offset_max.y;

			size = Vector2(max_x - min_x, max_y - min_y);
		}
		else
		{
			size = _offset_max - _offset_min;
		}

		return size;
	}

	Ref<UIRect> UIRect::GetParentRect() const
	{
		Ref<UIRect> rect;

		const Component* c = dynamic_cast<const Component*>(this);
		WeakRef<Transform> parent = c->GetTransform()->GetParent();
		if (!parent.expired())
		{
			Ref<UIRect> r = parent.lock()->GetGameObject()->GetComponent<UIRect>();
			if (r)
				rect = r;
		}

		return rect;
	}

	void UIRect::OnAnchor()
	{
		Ref<UIRect> parent = GetParentRect();
		if (parent)
		{
			Vector2 parent_size = parent->GetSize();
			float min_x = _anchor_min.x * parent_size.x + _offset_min.x;
			float min_y = _anchor_min.y * parent_size.y + _offset_min.y;
			float max_x = _anchor_max.x * parent_size.x + _offset_max.x;
			float max_y = _anchor_max.y * parent_size.y + _offset_max.y;

			float x = min_x + _pivot.x * (max_x - min_x);
			float y = min_y + _pivot.y * (max_y - min_y);
			float parent_x = parent_size.x * parent->_pivot.x;
			float parent_y = parent_size.y * parent->_pivot.y;

			Vector2 pos = Vector2(x - parent_x, y - parent_y);
			pos.x = floor(pos.x);
			pos.y = floor(pos.y);

			const Component* c = dynamic_cast<const Component*>(this);
			c->GetTransform()->SetLocalPosition(Vector3(pos.x, pos.y, 0));
		}
	}

} // end of namespace kge
