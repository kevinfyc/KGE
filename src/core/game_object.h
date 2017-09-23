//
//  game_object.h
//  libkge
//
//  Created by kevin on 17-9-22.
//
//

#ifndef __KGE_CORE_GAME_OBJECT_H__
#define __KGE_CORE_GAME_OBJECT_H__

#include "pre_define.h"
#include "object.h"
#include "transform.h"

namespace kge
{
	class Component;

    class GameObject : public Object
    {
		friend class Transform;
		friend class World;

	public:

		static Ref<GameObject> Create(const std::string& name, bool add_to_world = true);
		static void Destroy(Ref<GameObject> obj);

		void SetName(const std::string& name) {}

		Ref<Component> AddComponent(const std::string& name);
		Ref<Component> GetComponent(const std::string& name) const;
		bool GetComponentsInChildren(const std::string& name, std::vector<Ref<Component>>& coms) const;

		template<class T> Ref<T> AddComponent();
		template<class T> Ref<T> GetComponent() const;
		template<class T> bool GetComponents(std::vector<Ref<T>>& coms) const;
		template<class T> bool GetComponentsInChildren(std::vector<Ref<T>>& coms) const;
		template<class T> Ref<T> GetComponentInParent() const;
		Ref<Component> GetComponentRef(const Component* com) const;


		Ref<Transform> GetTransform() const { return _transform.lock(); }

	private:
		GameObject(const std::string& name);
		void AddComponent(const Ref<Component>& com);

		void Delete();
		void Start();
		void Update();
		void LateUpdate();

	private:
		bool _deleted;
		std::list<Ref<Component>> _components;
		std::list<Ref<Component>> _components_neo;

		bool _in_world;
		bool _in_world_update;

		WeakRef<Transform> _transform;
    };

	template<class T> Ref<T> GameObject::AddComponent()
	{
		return RefCast<T>(AddComponent(T::ClassName()));
	}

	template<class T> Ref<T> GameObject::GetComponent() const
	{
		for (auto i : _components)
		{
			auto t = RefCast<T>(i);
			if (t && !i->_deleted)
			{
				return t;
			}
		}
		
		for (auto i : _components_neo)
		{
			auto t = RefCast<T>(i);
			if (t && !i->_deleted)
			{
				return t;
			}
		}

		return Ref<T>();
	}

	template<class T> bool GameObject::GetComponents(std::vector<Ref<T>>& coms) const
	{
		for (auto i : _components)
		{
			auto t = RefCast<T>(i);
			if (t && !i->_deleted)
			{
				coms.push_back(t);
			}
		}

		for (auto i : _components_neo)
		{
			auto t = RefCast<T>(i);
			if (t && !i->_deleted)
			{
				coms.push_back(t);
			}
		}

		return coms.size() > 0;
	}

	template<class T> bool GameObject::GetComponentsInChildren(std::vector<Ref<T>>& coms) const
	{
		for (auto i : _components)
		{
			auto t = RefCast<T>(i);
			if (t && !i->_deleted)
			{
				coms.push_back(t);
			}
		}
		
		for (auto i : _components_neo)
		{
			auto t = RefCast<T>(i);
			if (t && !i->_deleted)
			{
				coms.push_back(t);
			}
		}

		auto transform = GetTransform();
		int child_count = transform->GetChildCount();
		for (int i = 0; i < child_count; i++)
		{
			auto child = transform->GetChild(i);

			std::vector<Ref<Component>> child_coms;
			child->GetGameObject()->GetComponentsInChildren<T>(child_coms);

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

	template<class T> Ref<T> GameObject::GetComponentInParent() const
	{
		Ref<T> com;

		auto parent = GetTransform()->GetParent().lock();

		while (parent)
		{
			com = parent->GetGameObject()->GetComponent<T>();

			if (com)
			{
				break;
			}
			else
			{
				parent = parent->GetParent().lock();
			}
		}

		return com;
	}
}

#endif // __KGE_CORE_GAME_OBJECT_H__
