//
//  transform.cpp
//  libkge
//
//  Created by kevin on 17-9-22.
//
//

#include "transform.h"
#include "util/log.h"

namespace kge
{
	DEFINE_COM_CLASS(Transform);

	Transform::Transform() :_delty(false)
		,_local_position(0, 0, 0)
		,_local_rotation(0, 0, 0, 1)
		,_local_scale(1, 1, 1)
		,_world_position(_local_position)
		,_world_rotation(_local_rotation)
		,_world_scale(_local_scale)
	{

	}

	Transform::~Transform()
	{

	}

	Ref<Transform> Transform::GetChild(uint32 index)const
	{
		if (index > _children.size() - 1)
		{
			KGE_LOG_ERROR("array overflow!");
			return Ref<Transform>();
		}


		return _children[index].lock();
	}

	void Transform::SetLocalPosition(const Vector3& pos)
	{
		if (_local_position != pos)
		{
			_local_position = pos;
			DeltyTransform();
		}
	}

	void Transform::SetLocalRotation(const Quaternion& rotation)
	{
		if (_local_rotation != rotation)
		{
			_local_rotation = rotation;
			DeltyTransform();
		}
	}

	void Transform::SetLocalScale(const Vector3& scale)
	{
		if (_local_scale != scale)
		{
			_local_scale = scale;
			DeltyTransform();
		}
	}

	void Transform::SetWorldPosition(const Vector3& pos)
	{
		if (!_delty && _local_position == pos)
			return;

		if (IsRoot())
		{
			SetLocalPosition(pos);
		}
		else
		{
			Vector3 local;
			_parent.lock()->World2Local(pos, local);
			SetLocalPosition(local);
		}
	}

	void Transform::SetWorldRotation(const Quaternion& rotation)
	{
		if (!_delty && _local_rotation == rotation)
			return;

		if (IsRoot())
		{
			SetLocalRotation(rotation);
		}
		else
		{
			Quaternion local = Quaternion::inverse(_parent.lock()->GetWorldRotation()) * rotation;
			SetLocalRotation(local);
		}
	}

	void Transform::SetWorldScale(const Vector3& scale)
	{
		if (!_delty && _local_scale == scale)
			return;

		if (IsRoot())
		{
			SetLocalPosition(scale);
		}
		else
		{
			const Vector3& parent_scale = _parent.lock()->GetWorldScale();
			float x = scale.x / parent_scale.x;
			float y = scale.y / parent_scale.y;
			float z = scale.z / parent_scale.z;
			SetLocalScale(Vector3(x, y, z));
		}
	}

	const Vector3& Transform::GetWorldPosition()
	{
		ApplyDelta();

		return _world_position;
	}

	const Quaternion& Transform::GetWorldRotation()
	{
		ApplyDelta();

		return _world_rotation;
	}

	const Vector3& Transform::GetWorldScale()
	{
		ApplyDelta();

		return _world_scale;
	}

	void Transform::DeltyTransform()
	{
		_delty = true;

		for (WeakRef<Transform> child : _children)
		{
			child.lock()->DeltyTransform();
		}
	}

	void Transform::ApplyDelta()
	{
		if (!_delty)
			return;

		_delty = false;

		if (IsRoot())
		{
			_world_position = _local_position;
			_world_rotation = _local_rotation;
			_world_scale = _local_scale;
		}
		else
		{
			Ref<Transform> parent = _parent.lock();

			parent->Local2World(_local_position, _world_position);
			_world_rotation = parent->GetWorldRotation() * _local_rotation;

			_world_scale = parent->GetWorldScale() * _local_scale;
		}

		Matrix mat_trans = Matrix();
		mat_trans.translation(_world_position);
		Matrix mat_rot = Matrix();
		mat_rot.setRotate(_world_rotation);
		Matrix mat_scale = Matrix();
		mat_scale.setScale(_world_scale);

		_local_to_world_matrix.setIdentity();
		_local_to_world_matrix.preMultiply(mat_trans);
		_local_to_world_matrix.preMultiply(mat_rot);
		_local_to_world_matrix.preMultiply(mat_scale);
	}

	const Matrix& Transform::GetWorld2LocalMatrix()
	{
		_world_to_local_matrix = GetLocal2WorldMatrix();
		_world_to_local_matrix.invert();

		return _world_to_local_matrix;
	}

	const Matrix& Transform::GetLocal2WorldMatrix()
	{
		ApplyDelta();

		return _local_to_world_matrix;
	}

	void Transform::World2Local(const Vector3& world, Vector3& local)
	{
		local = GetWorld2LocalMatrix().applyPoint(world);
	}

	void Transform::Local2World(const Vector3& local, Vector3& world)
	{
		world = GetLocal2WorldMatrix().applyPoint(local);
	}

	Vector3 Transform::GetRight()
	{
		return GetWorldRotation() * Vector3(1, 0, 0);
	}

	Vector3 Transform::GetUp()
	{
		return GetWorldRotation() * Vector3(0, 1, 0);
	}

	Vector3 Transform::GetForward()
	{
		return GetWorldRotation() * Vector3(0, 0, 1);
	}

} // end namespace kge
