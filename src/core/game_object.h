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

namespace kge
{
	class Component;

    class GameObject : public Object
    {
	public:
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

	private:
		void AddComponent(const Ref<Component>& com);

	private:
		bool _deleted;
		std::list<Ref<Component>> _components;
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

		//auto transform = GetTransform();
		//int child_count = transform->GetChildCount();
		//for (int i = 0; i < child_count; i++)
		//{
		//	auto child = transform->GetChild(i);
		//	auto child_coms = child->GetGameObject()->GetComponentsInChildren<T>();

		//	if (!child_coms.Empty())
		//	{
		//		coms.AddRange(&child_coms[0], child_coms.Size());
		//	}
		//}

		return coms.size() > 0;
	}

	template<class T> Ref<T> GameObject::GetComponentInParent() const
	{
		Ref<T> com;

		//auto parent = GetTransform()->GetParent().lock();

		//while (parent)
		//{
		//	com = parent->GetGameObject()->GetComponent<T>();

		//	if (com)
		//	{
		//		break;
		//	}
		//	else
		//	{
		//		parent = parent->GetParent().lock();
		//	}
		//}

		return com;
	}
}

#endif // __KGE_CORE_GAME_OBJECT_H__
