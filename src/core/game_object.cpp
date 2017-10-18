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

#include "graphics/renderer.h"
#include "graphics/layer.h"

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

	Ref<GameObject> GameObject::Instantiate(const Ref<GameObject>& source)
	{
		Ref<GameObject> clone = GameObject::Create(source->GetName());

		clone->DeepCopy(RefCast<Object>(source));

		return clone;
	}

	GameObject::GameObject(const std::string& name) :_deleted(false)
		, _layer((uint32)Layer::Default)
		, _active_in_hierarchy(true)
		, _active_self(true)
		, _static(false)
		, _in_world(false)
		, _in_world_update(false)
	{
		SetName(name);
	}

	GameObject::~GameObject()
	{

	}

	void GameObject::CopyComponent(const Ref<Component>& com)
	{
		auto transform = RefCast<Transform>(com);

		if (transform)
		{
			GetTransform()->DeepCopy(transform);
		}
		else
		{
			auto class_name = com->GetTypeName();
			auto* p_com = Component::Create(class_name);
			if (p_com != NULL)
			{
				AddComponent(Ref<Component>(p_com));
				p_com->DeepCopy(com);
			}
		}
	}

	void GameObject::DeepCopy(const Ref<Object>& source)
	{
		Object::DeepCopy(source);

		auto src = RefCast<GameObject>(source);

		for (const auto& i : src->_components)
		{
			CopyComponent(i);
		}

		for (const auto& i : src->_components_neo)
		{
			CopyComponent(i);
		}

		this->SetLayer(src->GetLayer());
		_active_in_hierarchy = src->_active_in_hierarchy;
		_active_self = src->_active_self;
		_deleted = src->_deleted;
		_static = src->_static;
	}

	void GameObject::SetLayer(uint32 layer)
	{
		if (_layer != layer)
		{
			_layer = layer;

			auto renderer = GetComponent<Renderer>();
			if (renderer)
			{
				Renderer::ClearPasses();
			}
		}
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
		if (_deleted)
			return Ref<Component>();

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

	std::vector<Ref<Component>> GameObject::GetComponentsInChildren(const std::string& name) const
	{
		std::vector<Ref<Component>> coms;

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
			std::vector<Ref<Component>> child_coms = child->GetGameObject()->GetComponentsInChildren(name);

			if (!child_coms.empty())
			{
				uint32 old_size = coms.size();
				coms.resize(old_size + child_coms.size());
				for (uint32 i = 0; i < child_coms.size(); ++i)
					coms[old_size + i] = child_coms[i];
			}
		}

		return coms;
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

	void GameObject::SetActive(bool active)
	{
		if (_active_self != active)
		{
			_active_self = active;

			Ref<Transform> t = _transform.lock();
			if (_active_in_hierarchy != active &&
				(t->IsRoot() || t->GetParent().lock()->GetGameObject()->IsActiveInHierarchy()))
			{
				SetActiveInHierarchy(active);

				if (!t->IsRoot())
					t->GetParent().lock()->NotifyParentHierarchyChange();
			}
		}
	}

	void GameObject::SetActiveInHierarchy(bool active)
	{
		if (_active_in_hierarchy != active)
		{
			_active_in_hierarchy = active;

			for (const auto& i : _components)
			{
				if (i->IsEnable())
				{
					if (_active_in_hierarchy)
					{
						i->OnEnable();
					}
					else
					{
						i->OnDisable();
					}
				}
			}

			auto transform = _transform.lock();
			int child_count = transform->GetChildCount();
			for (int i = 0; i < child_count; i++)
			{
				auto child = transform->GetChild(i);
				if (child->GetGameObject()->IsActiveSelf())
				{
					child->GetGameObject()->SetActiveInHierarchy(active);
				}
			}
		}
	}

	void GameObject::SetLayerRecursively(int layer)
	{
		SetLayer(layer);

		auto transform = _transform.lock();
		int child_count = transform->GetChildCount();
		for (int i = 0; i < child_count; i++)
		{
			auto child = transform->GetChild(i);
			child->GetGameObject()->SetLayerRecursively(layer);
		}
	}

	void GameObject::OnTransformChanged()
	{
		for (const auto& i : _components)
		{
			i->OnTransformChanged();
		}

		for (const auto& i : _components_neo)
		{
			i->OnTransformChanged();
		}
	}

	void GameObject::OnTransformHierarchyChanged()
	{
		for (const auto& i : _components)
		{
			i->OnTransformHierarchyChanged();
		}

		for (const auto& i : _components_neo)
		{
			i->OnTransformHierarchyChanged();
		}
	}

	void GameObject::Delete()
	{
		if (!_deleted)
			_deleted = true;

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
			if (!IsActiveInHierarchy() || _deleted)
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
