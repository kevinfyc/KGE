//
//  component.cpp
//  libkge
//
//  Created by kevin on 17-9-22.
//
//

#include "component.h"
#include "game_object.h"

namespace kge
{
	DEFINE_COM_BASE(Component);

	/*static*/void Component::RegisterComponents()
	{

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

} // end namespace ora
