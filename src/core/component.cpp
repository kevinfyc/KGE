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

namespace kge
{
	DEFINE_COM_BASE(Component);

	/*static*/void Component::RegisterComponents()
	{
		Transform::RegisterComponent();
	}

	/*static*/void Component::Destroy(Ref<Component> com)
	{
		if (com)
			com->Delete();
	}

	Component::Component()
	{

	}

	Component::~Component()
	{
	}

	Ref<GameObject> Component::GetGameObject() const
	{
		return _gameObject.lock();
	}

	void Component::Delete()
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
