//
//  game_object.cpp
//  libkge
//
//  Created by kevin on 17-9-22.
//
//

#include "game_object.h"

#include "component.h"

#include "world.h"

namespace kge
{
	Ref<GameObject> GameObject::Create(const std::string& name, bool add_to_world)
	{
		Ref<GameObject> obj = Ref<GameObject>(new GameObject(name));
		if (add_to_world)
		{
			obj->_in_world = true;
			World::AddGameObject(obj);
		}

		Ref<Transform> transform = Ref<Transform>((Transform*)Component::Create(Transform::ClassName()));
		transform->_gameObject = obj;
		obj->_transform = transform;
		obj->AddComponent(transform);

		return obj;
	}

	void GameObject::Destroy(Ref<GameObject> obj)
	{
		if (!obj)
			return;

		obj->GetTransform()->SetParent(WeakRef<Transform>());
		obj->Delete();
	}

	GameObject::GameObject(const std::string& name) :_deleted(false)
		, _in_world(false)
		, _in_world_update(false)
	{
		SetName(name);
	}

	GameObject::~GameObject()
	{

	}

	void GameObject::SetName(const std::string& name)
	{
		if (GetName() != name)
		{
			Object::SetName(name);

			for (const auto& com : _components)
				com->SetName(name);

			for (const auto& com : _components_neo)
				com->SetName(name);
		}
	}

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
			if (!com->_deleted && com->IsComponent(name))
				return com;
		}
		
		for (Ref<Component> com : _components_neo)
		{
			if (!com->_deleted && com->IsComponent(name))
				return com;
		}

		return Ref<Component>();
	}

	bool GameObject::GetComponentsInChildren(const std::string& name, std::vector<Ref<Component>>& coms) const
	{
		for (auto com : _components)
		{
			if (!com->_deleted && com->IsComponent(name))
				coms.push_back(com);
		}
		
		for (auto com : _components_neo)
		{
			if (!com->_deleted && com->IsComponent(name))
				coms.push_back(com);
		}

		auto transform = GetTransform();
		int child_count = transform->GetChildCount();
		for (int i = 0; i < child_count; i++)
		{
			auto child = transform->GetChild(i);
			std::vector<Ref<Component>> child_coms;
			child->GetGameObject()->GetComponentsInChildren(name, child_coms);

			if (!child_coms.empty())
			{
				uint32 old_size = coms.size();
				coms.resize(old_size + child_coms.size());
				for (uint32 i = 0; i < child_coms.size(); ++i)
					coms[old_size + i] = child_coms[i];
			}
		}

		return coms.size() > 0;
	}

	Ref<Component> GameObject::GetComponentRef(const Component* com) const
	{
		for (const auto& i : _components)
		{
			if (i.get() == com && !i->_deleted)
			{
				return i;
			}
		}
		
		for (const auto& i : _components_neo)
		{
			if (i.get() == com && !i->_deleted)
			{
				return i;
			}
		}

		return Ref<Component>();
	}

	void GameObject::AddComponent(const Ref<Component>& com)
	{
		_components_neo.push_back(com);

		com->_transform = _transform;
		com->_gameObject = _transform.lock()->_gameObject;
		com->SetName(GetName());
		com->Awake();

		if (_in_world)
			World::AddGameObject(GetTransform()->GetGameObject());
	}

	void GameObject::Delete()
	{
		if (!_deleted)
			_deleted = false;

		Ref<Transform> transform = GetTransform();
		uint32 child_count = transform->GetChildCount();
		for (uint32 i = 0; i < child_count; ++i)
			transform->GetChild(i)->GetGameObject()->Delete();

	}

	void GameObject::Start()
	{
		std::list<Ref<Component>> starts(_components);
		do
		{
			for (auto& i : starts)
			{
				if (_deleted)
					break;

				if (i->IsEnable() && !i->IsStarted())
				{
					i->_started = true;
					i->Start();
				}
			}
			starts.clear();

			starts = _components_neo;
			_components.insert(_components.end(), _components_neo.begin(), _components_neo.end());
			_components_neo.clear();
		} while (!starts.empty());
	}

	void GameObject::Update()
	{
		for (const auto& i : _components)
		{
			if (_deleted)
				break;

			if (i->IsEnable())
				i->Update();
		}
	}

	void GameObject::LateUpdate()
	{
		for (const auto& i : _components)
		{
			if (_deleted)
				break;

			if (i->IsEnable())
				i->LateUpdate();
		}

		//delete component
		auto it = _components.begin();
		while (it != _components.end())
		{
			if ((*it)->_deleted)
			{
				it = _components.erase(it);
			}
			else
			{
				it++;
			}
		}
	}

} // end namespace kge
