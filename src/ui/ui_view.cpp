//
//  ui_view.cpp
//  libkge
//
//  Created by kevin on 17-10-10.
//
//
// 

#include "ui_view.h"
#include "graphics/color.h"
#include "core/transform.h"
#include "graphics/material.h"
#include "ui_canvas_renderer.h"
#include "ui_event_handler.h"

namespace kge
{
	DEFINE_COM_CLASS(UIView);

	UIView::UIView() :
		_color(1, 1, 1, 1)
	{
	}

	void UIView::DeepCopy(const Ref<Object>& source)
	{
		Component::DeepCopy(source);

		auto src = RefCast<UIView>(source);
		_color = src->_color;
		_anchor_min = src->_anchor_min;
		_anchor_max = src->_anchor_max;
		_offset_min = src->_offset_min;
		_offset_max = src->_offset_max;
		_pivot = src->_pivot;
		_dirty = true;
	}

	void UIView::SetAnchors(const Vector2& min, const Vector2& max)
	{
		UIRect::SetAnchors(min, max);

		if (_dirty)
			MarkRendererDirty();
	}

	void UIView::SetOffsets(const Vector2& min, const Vector2& max)
	{
		UIRect::SetOffsets(min, max);

		if (_dirty)
			MarkRendererDirty();
	}

	void UIView::SetPivot(const Vector2& pivot)
	{
		UIRect::SetPivot(pivot);

		if (_dirty)
			MarkRendererDirty();
	}

	void UIView::SetColor(const Color& color)
	{
		if (_color != color)
		{
			_color = color;
			_dirty = true;
			MarkRendererDirty();
		}
	}

	void UIView::OnTranformChanged()
	{
		_dirty = true;
		MarkRendererDirty();
	}

	void UIView::SetRenderer(const Ref<UICanvasRenderer>& renderer)
	{
		_renderer = renderer;
	}

	void UIView::MarkRendererDirty()
	{
		if (!_renderer.expired())
		{
			_renderer.lock()->MarkDirty();
			_renderer.reset();
		}
	}

	Matrix UIView::GetVertexMatrix()
	{
		auto local_position = GetTransform()->GetLocalPosition();
		auto local_rotation = GetTransform()->GetLocalRotation();
		auto local_scale = GetTransform()->GetLocalScale();

		auto mat_local = Matrix::TRS(local_position, local_rotation, local_scale);
		auto mat_parent_to_world = GetTransform()->GetParent().lock()->GetLocal2WorldMatrix();
		auto mat_world_to_canvas = _renderer.lock()->GetTransform()->GetWorld2LocalMatrix();

		Matrix tmp = Matrix::identity;
		tmp.postMultiply(mat_world_to_canvas);
		tmp.postMultiply(mat_parent_to_world);
		tmp.postMultiply(mat_local);

		return tmp;
	}

	std::vector<Vector3> UIView::GetBoundsVertices()
	{
		std::vector<Vector3> vertices;

		Vector2 size = this->GetSize();
		Vector2 min = Vector2(-_pivot.x * size.x, -_pivot.y * size.y);
		Vector2 max = Vector2((1 - _pivot.x) * size.x, (1 - _pivot.y) * size.y);
		vertices.push_back(Vector3(min.x, min.y, 0));
		vertices.push_back(Vector3(max.x, min.y, 0));
		vertices.push_back(Vector3(max.x, max.y, 0));
		vertices.push_back(Vector3(min.x, max.y, 0));

		auto mat = GetVertexMatrix();
		for (int i = 0; i < 4; i++)
		{
			auto v = vertices[i];
			v.x = floor(v.x);
			v.y = floor(v.y);

			vertices[i] = mat.applyPoint(v);
		}

		return vertices;
	}

	void UIView::FillVertices(std::vector<Vector3>& vertices, std::vector<Vector2>& uv, std::vector<Color>& colors, std::vector<uint16>& indices)
	{
		auto vertex_array = GetBoundsVertices();
		auto old_size = vertices.size();
		vertices.resize(old_size + 4);

		for (int i = 0; i < 4; i++)
		{
			vertices[old_size + i] = vertex_array[i];
		}

		uv.push_back(Vector2(0, 1));
		uv.push_back(Vector2(1, 1));
		uv.push_back(Vector2(1, 0));
		uv.push_back(Vector2(0, 0));

		colors.push_back(_color);
		colors.push_back(_color);
		colors.push_back(_color);
		colors.push_back(_color);

		uint32 index_begin = vertices.size() - 4;
		indices.push_back(index_begin + 0);
		indices.push_back(index_begin + 1);
		indices.push_back(index_begin + 2);
		indices.push_back(index_begin + 0);
		indices.push_back(index_begin + 2);
		indices.push_back(index_begin + 3);
	}

	void UIView::FillMaterial(Ref<Material>& mat)
	{
	}

} // end of namespace kge
