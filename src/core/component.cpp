//
//  component.cpp
//  libkge
//
//  Created by kevin on 17-9-22.
//
//

#include "component.h"
#include "game_object.h"

#include "transform.h"
#include "graphics/camera.h"
#include "graphics/mesh_renderer.h"

#include "ui/ui_canvas_renderer.h"
#include "ui/ui_view.h"
#include "ui/ui_image.h"
#include "ui/ui_label.h"

#include "util/string_tool.h"

namespace kge
{
	DEFINE_COM_BASE(Component);

	/*static*/void Component::RegisterComponents()
	{
		Transform::RegisterComponent();
		Camera::RegisterComponent();
		MeshRenderer::RegisterComponent();
		UICanvasRenderer::RegisterComponent();
		UIView::RegisterComponent();
		UIImage::RegisterComponent();
		UILabel::RegisterComponent();
	}

	/*static*/void Component::Destroy(Ref<Component> com)
	{
		if (com)
			com->Delete();
	}

	Ref<Transform> Transform::Find(const std::string& path) const
	{
		Ref<Transform> find;

		std::string p = path;
		StringVector names;
		if (!StringSplit(p, "/", names))
			return find;

		for (auto& c : _children)
		{
			auto child = c.lock();
			auto name = names[0];

			if (child->GetName() == name)
			{
				if (names.size() > 1)
					find = child->Find(path.substr(name.size() + 1));
				else
					find = child;

				break;
			}
		}

		return find;
	}

	void Component::DeepCopy(const Ref<Object>& source)
	{
		Object::DeepCopy(source);

		auto src = RefCast<Component>(source);
		_deleted = src->_deleted;
		_enable = src->_enable;
		_started = false;
	}

	Component::Component():_deleted(false)
		,_started(false)
		,_enable(true)
	{

	}

	Component::~Component()
	{
	}

	Ref<GameObject> Component::GetGameObject() const
	{
		return _gameObject.lock();
	}

	Ref<Transform> Component::GetTransform() const
	{
		return _transform.lock();
	}

	Ref<Component> Component::GetRef() const
	{
		return GetGameObject()->GetComponentRef(this);
	}

	void Component::Delete()
	{
		if (!_deleted)
		{
			_deleted = true;
			Enable(false);
		}
	}

	void Component::Enable(bool enable)
	{

	}

	void Component::SetName(const std::string& name)
	{
		if (GetName() != name)
		{
			Object::SetName(name);
			GetGameObject()->SetName(name);
		}
	}

	bool Component::IsComponent(const std::string& type) const
	{
		auto& names = GetClassNames();

		for (auto& name : names)
		{
			if (name == type)
				return true;
		}

		return false;
	}

} // end namespace kge
