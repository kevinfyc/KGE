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

	class Component : public Object
	{
		DECLARE_COM_BASE(Component);

	public:
		static void RegisterComponents();
		static void Destroy(Ref<Component> com);

		virtual ~Component();

		void SetName(const std::string& name);

		Ref<GameObject> GetGameObject() const;

	protected:
		Component();

	private:
		void Delete();

	protected:
		WeakRef<GameObject> _gameObject;
	};
}

#endif // __KGE_CORE_COMPONENT_H__
