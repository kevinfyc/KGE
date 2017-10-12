//
//  ui_view.h
//  libkge
//
//  Created by kevin on 17-10-10.
//
//
// 

#ifndef __KGE_UI_VIEW_H__
#define __KGE_UI_VIEW_H__

#include "core/component.h"
#include "ui_rect.h"
#include "graphics/color.h"
#include "graphics/material.h"
#include "ui_event_handler.h"

namespace kge
{
	class UICanvasRenderer;

	class UIView:public Component, public UIRect
	{
		friend class UIEventHandler;
		DECLARE_COM_CLASS(UIView, Component);

	public:
		virtual void SetAnchors(const Vector2& min, const Vector2& max);
		virtual void SetOffsets(const Vector2& min, const Vector2& max);
		virtual void SetPivot(const Vector2& pivot);
		virtual void FillVertices(std::vector<Vector3>& vertices, std::vector<Vector2>& uv, std::vector<Color>& colors, std::vector<uint16>& indices);
		virtual void FillMaterial(Ref<Material>& mat);
		void SetColor(const Color& color);
		const Color& GetColor() const { return _color; }
		void SetRenderer(const Ref<UICanvasRenderer>& renderer);
		const WeakRef<UICanvasRenderer>& GetRenderer() const { return _renderer; }
		std::vector<Vector3> GetBoundsVertices();

		bool GetDrag()const { return _drag; }
		void SetDrag(bool drag) { _drag = drag; }

	protected:
		UIView();
		void MarkRendererDirty();
		Matrix GetVertexMatrix();
		virtual void OnTranformChanged();

	private:
		bool GetDraging()const { return _draging; }
		void SetDraging(bool draging) { _draging = draging; }

		bool GetPointerIn()const { return _pointer_in; }
		void setPointerIn(bool pointer_in) { _pointer_in = pointer_in; }

	public:
		UIEventHandler event_handler;

	protected:
		Color _color;
		WeakRef<UICanvasRenderer> _renderer;
		bool _drag;
		bool _draging;
		bool _pointer_in;
	};
} // end of namespace kge

#endif // __KGE_UI_VIEW_H__