//
//  game_object.cpp
//  libkge
//
//  Created by kevin on 17-9-22.
//
//

#include "game_object.h"

#include "component.h"

namespace kge
{
	Ref<Component> GameObject::AddComponent(const std::string& name)
	{
		Ref<Component> t = Ref<Component>(Component::Create(name));
		AddComponent(t);

		return t;
	}

	Ref<Component> GameObject::GetComponent(const std::string& name) const
	{
		for (Ref<Component> com : _components)
		{
			if (!com->_delete && com->IsComponent(name))
				return com;
		}

		return Ref<Component>();
	}

	bool GameObject::GetComponentsInChildren(const std::string& name, std::vector<Ref<Component>>& coms) const
	{
		for (auto com : _components)
		{
			if (!com->_delete && com->IsComponent(name))
				coms.push_back(com);
		}
		
		//auto transform = GetTransform();
		//int child_count = transform->GetChildCount();
		//for (int i = 0; i < child_count; i++)
		//{
		//	auto child = transform->GetChild(i);
		//	auto child_coms = child->GetGameObject()->GetComponentsInChildren(name);

		//	if (!child_coms.Empty())
		//	{
		//		coms.AddRange(&child_coms[0], child_coms.Size());
		//	}
		//}

		return coms.size() > 0;
	}

	Ref<Component> GameObject::GetComponentRef(const Component* com) const
	{
		for (const auto& i : _components)
		{
			if (i.get() == com && !i->_delete)
			{
				return i;
			}
		}

		return Ref<Component>();
	}

	void GameObject::AddComponent(const Ref<Component>& com)
	{
		_components.push_back(com);

		com->SetName(GetName());
	}
} // end namespace kge
