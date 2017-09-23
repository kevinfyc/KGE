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

namespace kge
{
	DEFINE_COM_BASE(Component);

	/*static*/void Component::RegisterComponents()
	{
		Transform::RegisterComponent();
		Camera::RegisterComponent();
	}

	/*static*/void Component::Destroy(Ref<Component> com)
	{
		if (com)
			com->Delete();
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
