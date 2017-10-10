//
//  ui_canvas_renderer.h
//  libkge
//
//  Created by kevin on 17-10-10.
//
//
// 

#ifndef __KGE_UI_CANVAS_RENDERER_H__
#define __KGE_UI_CANVAS_RENDERER_H__

#include "math/vector2.hpp"
#include "math/rect.h"
#include "core/pre_define.h"
#include "graphics/renderer.h"
#include "ui_rect.h"
#include "graphics/mesh.h"
#include "ui_view.h"

namespace kge
{
	class UICanvasRenderer : public Renderer, public UIRect
	{
		DECLARE_COM_CLASS(UICanvasRenderer, Renderer);

	private:
		friend class UIEventHandler;

	public:
		enum class RenderType
		{
			BaseView,
			Sprite,
			Text
		};

		static void HandleUIEvent(const std::list<UICanvasRenderer*>& list);

		virtual ~UICanvasRenderer();
		virtual const VertexBuffer* GetVertexBuffer() const;
		virtual const IndexBuffer* GetIndexBuffer() const;
		virtual void GetIndexRange(uint32 material_index, uint32& start, uint32& count) const;
		void MarkDirty();
		const std::vector<Ref<UIView>>& GetViews() const { return _views; }

	protected:
		virtual void LateUpdate();
		virtual void OnTranformHierarchyChanged();

	private:
		UICanvasRenderer();
		void FindViews();
		void UpdateViews();

		RenderType _type;
		Ref<Mesh> _mesh;
		std::vector<Ref<UIView>> _views;
	};
} // end of namespace kge

#endif // __KGE_UI_CANVAS_RENDERER_H__