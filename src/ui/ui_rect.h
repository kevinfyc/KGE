//
//  ui_rect.h
//  libkge
//
//  Created by kevin on 17-10-10.
//
//
// 

#ifndef __KGE_UI_RECT_H__
#define __KGE_UI_RECT_H__

#include "math/vector2.hpp"
#include "math/rect.h"
#include "core/pre_define.h"

namespace kge
{
	class UIRect
	{
	public:
		virtual void SetAnchors(const Vector2& min, const Vector2& max);
		const Vector2& GetAnchorMin() const { return _anchor_min; }
		const Vector2& GetAnchorMax() const { return _anchor_max; }
		
		virtual void SetOffsets(const Vector2& min, const Vector2& max);
		const Vector2& GetOffsetMin() const { return _offset_min; }
		const Vector2& GetOffsetMax() const { return _offset_max; }

		virtual void SetPivot(const Vector2& pivot);
		const Vector2& GetPivot() const { return _pivot; }

		virtual void SetSize(const Vector2& size);
		Vector2 GetSize() const;

		void OnAnchor();

	protected:
		UIRect();
		Ref<UIRect> GetParentRect() const;

		Vector2 _anchor_min;
		Vector2 _anchor_max;
		Vector2 _offset_min;
		Vector2 _offset_max;
		Vector2 _pivot;
		bool _dirty;
	};
} // end of namespace kge

#endif // __KGE_UI_RECT_H__