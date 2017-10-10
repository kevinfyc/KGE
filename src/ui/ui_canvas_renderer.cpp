//
//  ui_canvas_renderer.cpp
//  libkge
//
//  Created by kevin on 17-10-10.
//
//
// 

#include "ui_canvas_renderer.h"
#include "core/game_object.h"
#include "graphics/material.h"

namespace kge
{
	DEFINE_COM_CLASS(UICanvasRenderer);

	UICanvasRenderer::UICanvasRenderer() :
		_type(RenderType::BaseView)
	{
	}

	UICanvasRenderer::~UICanvasRenderer()
	{
	}

	void UICanvasRenderer::DeepCopy(const Ref<Object>& source)
	{
		Renderer::DeepCopy(source);

		auto src = RefCast<UICanvasRenderer>(source);
		SetSortingOrder(src->GetSortingOrder());
	}

	void UICanvasRenderer::HandleUIEvent(const std::list<UICanvasRenderer*>& list)
	{
	}

	void UICanvasRenderer::LateUpdate()
	{
		UpdateViews();
	}

	void UICanvasRenderer::OnTranformHierarchyChanged()
	{
		MarkDirty();
	}

	void UICanvasRenderer::MarkDirty()
	{
		_dirty = true;
	}

	void UICanvasRenderer::FindViews()
	{
		_views.clear();
		std::list<Ref<Transform>> to_find;
		to_find.push_front(GetTransform());

		while (!to_find.empty())
		{
			auto t = to_find.front();
			to_find.pop_front();

			auto view = t->GetGameObject()->GetComponent<UIView>();
			if (view &&
				view->IsEnable() &&
				t->GetGameObject()->IsActiveSelf())
			{
				auto type_name = view->GetTypeName();
				if (type_name == "UIImage")
				{
					if (_type == RenderType::BaseView || _type == RenderType::Sprite)
					{
						_type = RenderType::Sprite;
						_views.push_back(view);
					}
				}
				else if (type_name == "UILabel")
				{
					if (_type == RenderType::BaseView || _type == RenderType::Text)
					{
						_type = RenderType::Text;
						_views.push_back(view);
					}
				}
				else if (type_name == "UIView")
				{
					// do not render empty view
				}
				else
				{
					assert(!"unknown view type");
				}
			}

			uint32 child_count = t->GetChildCount();
			for (uint32 i = child_count - 1; i >= 0; i--)
			{
				auto child = t->GetChild(i);
				auto canvas = child->GetGameObject()->GetComponent<UICanvasRenderer>();
				if (!canvas && child->GetGameObject()->IsActiveSelf())
				{
					to_find.push_front(child);
				}
			}
		}

		for (uint32 i = 0; i < _views.size(); i++)
		{
			_views[i]->SetRenderer(RefCast<UICanvasRenderer>(GetRef()));
		}
	}

	void UICanvasRenderer::UpdateViews()
	{
		if (!_dirty)
		{
			return;
		}

		_dirty = false;

		FindViews();

		if (!_views.empty())
		{
			auto mat = GetSharedMaterial();
			if (!mat)
			{
				if (_type == RenderType::BaseView || _type == RenderType::Sprite)
				{
					mat = Material::Create("UI/Sprite");
					SetSharedMaterial(mat);
				}
				else if (_type == RenderType::Text)
				{
					mat = Material::Create("UI/Text");
					SetSharedMaterial(mat);
				}
			}

			std::vector<Vector3> vertices;
			std::vector<Vector2> uv;
			std::vector<Color> colors;
			std::vector<unsigned short> indices;

			for (uint32 i = 0; i < _views.size(); i++)
			{
				_views[i]->SetRenderer(RefCast<UICanvasRenderer>(GetRef()));
				_views[i]->FillVertices(vertices, uv, colors, indices);
				_views[i]->FillMaterial(mat);
			}

			if (!vertices.empty())
			{
				if (!_mesh)
				{
					_mesh = Mesh::Create(true);
				}
				_mesh->vertices = vertices;
				_mesh->uv = uv;
				_mesh->colors = colors;
				_mesh->triangles = indices;
				_mesh->Tick();
			}
		}
	}

	const VertexBuffer* UICanvasRenderer::GetVertexBuffer() const
	{
		if (_mesh)
		{
			return _mesh->GetVertexBuffer().get();
		}

		return nullptr;
	}

	const IndexBuffer* UICanvasRenderer::GetIndexBuffer() const
	{
		if (_mesh)
		{
			return _mesh->GetIndexBuffer().get();
		}

		return nullptr;
	}

	void UICanvasRenderer::GetIndexRange(uint32 material_index, uint32& start, uint32& count) const
	{
		if (_mesh)
		{
			_mesh->GetIndexRange(material_index, start, count);
		}
	}

} // end of namespace kge
