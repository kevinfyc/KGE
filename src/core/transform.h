//
//  transform.h
//  libkge
//
//  Created by kevin on 17-9-22.
//
//

#ifndef __KGE_CORE_TRANSFORM_H__
#define __KGE_CORE_TRANSFORM_H__

#include "component.h"

#include "math/vector3.hpp"
#include "math/quat.hpp"
#include "math/matrix.hpp"

namespace kge
{
	class Transform : public Component
	{
		DECLARE_COM_CLASS(Transform, Component);

	private:
		friend class GameObject;

	public:
		~Transform();

		WeakRef<Transform> GetParent()const { return _parent; }
		void SetParent(const WeakRef<Transform>& parent) { _parent = parent; }
		bool IsRoot()const { return _parent.expired(); }
		
		uint32 GetChildCount() { return _children.size(); }
		Ref<Transform> GetChild(uint32 index)const;

		void DeltaTransform();
		void SetLocalPosition(const Vector3& pos);
		void SetLocalRotation(const Quaternion& rotation);
		void SetLocalScale(const Vector3& scale);

		void SetWorldPosition(const Vector3& pos);
		void SetWorldRotation(const Quaternion& rotation);
		void SetWorldScale(const Vector3& scale);

		const Vector3& GetWorldPosition();
		const Quaternion& GetWorldRotation();
		const Vector3& GetWorldScale();

		const Vector3& GetLocalPosition() { return _local_position; }
		const Quaternion& GetLocalRotation() { return _local_rotation; };
		const Vector3& GetLocalScale() { return _local_scale; };

		void World2Local(const Vector3& world, Vector3& local);
		void Local2World(const Vector3& local, Vector3& world);

		const Matrix& GetWorld2LocalMatrix();
		const Matrix& GetLocal2WorldMatrix();

	private:
		Transform();

		void ApplyDelta();

	private:
		WeakRef<Transform> _parent;
		std::vector<WeakRef<Transform>> _children;

		Vector3 _local_position;
		Quaternion _local_rotation;
		Vector3 _local_scale;

		Vector3 _world_position;
		Quaternion _world_rotation;
		Vector3 _world_scale;

		Matrix _local_to_world_matrix;
		Matrix _world_to_local_matrix;

		bool _delta;
	};
}

#endif // __KGE_CORE_TRANSFORM_H__
