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
		virtual ~GameObject();
		static Ref<GameObject> Create(const std::string& name, bool add_to_world = true);
		static void Destroy(Ref<GameObject> obj);

		void SetName(const std::string& name);

		Ref<Component> AddComponent(const std::string& name);
		Ref<Component> GetComponent(const std::string& name) const;
		std::vector<Ref<Component>> GetComponentsInChildren(const std::string& name) const;

		template<class T> Ref<T> AddComponent();
		template<class T> Ref<T> GetComponent() const;
		template<class T> bool GetComponents(std::vector<Ref<T>>& coms) const;
		template<class T> std::vector<Ref<T>> GetComponentsInChildren() const;
		template<class T> Ref<T> GetComponentInParent() const;
		Ref<Component> GetComponentRef(const Component* com) const;

		Ref<Transform> GetTransform() const { return _transform.lock(); }

		bool IsActiveInHierarchy() const { return _active_in_hierarchy; }
		bool IsActiveSelf() const { return _active_self; }

		void SetActive(bool active);
		void SetLayerRecursively(int layer);

		bool IsStatic() const { return _static; }
		void SetStatic(bool value) { _static = value; }

		void SetLayer(uint32 layer);
		uint32 GetLayer() const { return _layer; }

	private:
		GameObject(const std::string& name);
		void AddComponent(const Ref<Component>& com);

		void Delete();
		void Start();
		void Update();
		void LateUpdate();
		void SetActiveInHierarchy(bool active);
		void OnTransformChanged();
		void OnTransformHierarchyChanged();

	private:
		bool _deleted;
		std::list<Ref<Component>> _components;
		std::list<Ref<Component>> _components_neo;

		uint32 _layer;

		bool _in_world;
		bool _in_world_update;

		WeakRef<Transform> _transform;

		bool _active_in_hierarchy;
		bool _active_self;
		bool _static;
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

	template<class T> std::vector<Ref<T>> GameObject::GetComponentsInChildren() const
	{
		std::vector<Ref<T>> coms;

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
			auto child_coms = child->GetGameObject()->GetComponentsInChildren<T>();

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
