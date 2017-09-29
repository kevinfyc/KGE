//
//  component.h
//  libkge
//
//  Created by kevin on 17-9-22.
//
//

#ifndef __KGE_CORE_COMPONENT_H__
#define __KGE_CORE_COMPONENT_H__

#include "object.h"

namespace kge
{
	class GameObject;
	class Transform;

	class Component : public Object
	{
		DECLARE_COM_BASE(Component);
	private:
		friend GameObject;

	public:
		static void RegisterComponents();
		static void Destroy(Ref<Component> com);

		virtual ~Component();

		void SetName(const std::string& name);

		Ref<GameObject> GetGameObject() const;
		Ref<Transform> GetTransform() const;

		void Enable(bool enable);
		bool IsEnable() const { return _enable; }
		bool IsStarted() const { return _started; }
		bool IsComponent(const std::string& type) const;

	protected:
		Component();

		virtual void Awake() { }
		virtual void Start() { }
		virtual void Update() { }
		virtual void LateUpdate() { }
		virtual void OnEnable() { }
		virtual void OnDisable() { }
		virtual void OnTransformChanged() { }
		virtual void OnTransformHierarchyChanged() { }

	private:
		void Delete();

	protected:
		WeakRef<GameObject> _gameObject;
		WeakRef<Transform> _transform;

	private:
		bool _deleted;
		bool _started;
		bool _enable;
	};
}

#endif // __KGE_CORE_COMPONENT_H__
