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

	Transform::Transform() :_delta(false)
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
			DeltaTransform();
		}
	}

	void Transform::SetLocalRotation(const Quaternion& rotation)
	{
		if (_local_rotation != rotation)
		{
			_local_rotation = rotation;
			DeltaTransform();
		}
	}

	void Transform::SetLocalScale(const Vector3& scale)
	{
		if (_local_scale != scale)
		{
			_local_scale = scale;
			DeltaTransform();
		}
	}

	void Transform::SetWorldPosition(const Vector3& pos)
	{
		if (!_delta && _local_position == pos)
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
		if (!_delta && _local_rotation == rotation)
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
		if (!_delta && _local_scale == scale)
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

	void Transform::DeltaTransform()
	{
		_delta = true;

		for (WeakRef<Transform> child : _children)
		{
			child.lock()->DeltaTransform();
		}
	}

	void Transform::ApplyDelta()
	{
		if (!_delta)
			return;

		_delta = false;

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

} // end namespace kge
