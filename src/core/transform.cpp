//
//  transform.cpp
//  libkge
//
//  Created by kevin on 17-9-22.
//
//

#include "transform.h"
#include "util/log.h"
#include "game_object.h"

namespace kge
{
	DEFINE_COM_CLASS(Transform);

	Transform::Transform() :_delty(true)
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

	void Transform::DeepCopy(const Ref<Object>& source)
	{
		Component::DeepCopy(source);

		auto src = RefCast<Transform>(source);
		_delty = true;
		_world_position = src->GetWorldPosition();
		_world_rotation = src->GetWorldRotation();
		_world_scale = src->GetWorldScale();
		SetLocalPosition(_world_position);
		SetLocalRotation(_world_rotation);
		SetLocalScale(_world_scale);

		for (uint32 i = 0; i < src->_children.size(); i++)
		{
			auto src_child = src->_children[i].lock();
			auto child = GameObject::Instantiate(src_child->GetGameObject());

			child->GetTransform()->SetParent(RefCast<Transform>(GetRef()));
		}
	}

	void Transform::AddChild(WeakRef<Transform>& child)
	{
		_children.push_back(child);
	}

	void Transform::RemoveChild(WeakRef<Transform>& child)
	{
		for (uint32 i = 0; i < _children.size(); ++i)
		{
			if (_children[i].lock() == child.lock())
			{
				_children.erase(_children.begin() + i);
				break;
			}
		}
	}

	void Transform::SetParent(const WeakRef<Transform>& parent)
	{
		auto obj = GetGameObject();
		ApplyDelta();

		if (!_parent.expired())
		{
			auto p = _parent.lock();
			p->RemoveChild(_transform);
			p->NotifyParentHierarchyChange();
			_parent.reset();

			if (parent.expired())
			{
				_local_position = _world_position;
				_local_rotation = _world_rotation;
				_local_scale = _world_scale;
				DeltyTransform();
				NotifyChildHierarchyChange();

				obj->SetActiveInHierarchy(obj->IsActiveSelf());
			}
		}

		_parent = parent;

		if (!_parent.expired())
		{
			auto p = _parent.lock();
			p->AddChild(_transform);
			p->NotifyParentHierarchyChange();

			{
				p->World2Local(_world_position, _local_position);
				_local_rotation = Quaternion::inverse(p->GetWorldRotation()) * _world_rotation;
				const Vector3& parent_scale = p->GetWorldScale();
				float x = _world_scale.x / parent_scale.x;
				float y = _world_scale.x / parent_scale.y;
				float z = _world_scale.x / parent_scale.z;
				_local_scale = Vector3(x, y, z);
				DeltyTransform();;
				NotifyChildHierarchyChange();

				obj->SetActiveInHierarchy(p->GetGameObject()->IsActiveInHierarchy() && obj->IsActiveSelf());
			}
		}
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
			NotifyDelty();
		}
	}

	void Transform::SetLocalRotation(const Quaternion& rotation)
	{
		Quaternion r = rotation;
		r.normalise();

		if (_local_rotation != r)
		{
			_local_rotation = r;
			DeltyTransform();
			NotifyDelty();
		}
	}

	void Transform::SetLocalScale(const Vector3& scale)
	{
		if (_local_scale != scale)
		{
			_local_scale = scale;
			DeltyTransform();
			NotifyDelty();
		}
	}

	void Transform::SetWorldPosition(const Vector3& pos)
	{
		if (!_delty && _world_position == pos)
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

		
		_local_to_world_matrix = Matrix::TRS(_world_position, _world_rotation, _world_scale);
	}

	void Transform::NotifyDelty()
	{
		_delty_notifying = true;
		GetGameObject()->OnTransformChanged();

		for (auto& child : _children)
			child.lock()->NotifyDelty();

		_delty_notifying = false;
	}

	void Transform::NotifyParentHierarchyChange()
	{
		_delty_notifying = true;

		GetGameObject()->OnTransformHierarchyChanged();

		auto p = _parent;
		while (!p.expired())
		{
			auto parent = p.lock();
			parent->NotifyParentHierarchyChange();
			p = parent->GetParent();
		}

		_delty_notifying = false;
	}

	void Transform::NotifyChildHierarchyChange()
	{
		_delty_notifying = true;

		GetGameObject()->OnTransformHierarchyChanged();

		for (auto& child : _children)
			child.lock()->NotifyChildHierarchyChange();

		_delty_notifying = false;
	}

	const Matrix& Transform::GetWorld2LocalMatrix()
	{
		Matrix tmp = GetLocal2WorldMatrix();
		tmp.invert();
		_world_to_local_matrix = tmp;

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
