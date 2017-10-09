﻿#include "frustum.h"

namespace kge
{
	Frustum::Frustum(const Matrix& mat)
	{
		Vector4 left_plane;
		left_plane.x = mat.m30 + mat.m00;
		left_plane.y = mat.m31 + mat.m01;
		left_plane.z = mat.m32 + mat.m02;
		left_plane.w = mat.m33 + mat.m03;

		Vector4 right_plane;
		right_plane.x = mat.m30 - mat.m00;
		right_plane.y = mat.m31 - mat.m01;
		right_plane.z = mat.m32 - mat.m02;
		right_plane.w = mat.m33 - mat.m03;

		Vector4 bottom_plane;
		bottom_plane.x = mat.m30 + mat.m10;
		bottom_plane.y = mat.m31 + mat.m11;
		bottom_plane.z = mat.m32 + mat.m12;
		bottom_plane.w = mat.m33 + mat.m13;

		Vector4 top_plane;
		top_plane.x = mat.m30 - mat.m10;
		top_plane.y = mat.m31 - mat.m11;
		top_plane.z = mat.m32 - mat.m12;
		top_plane.w = mat.m33 - mat.m13;

		Vector4 near_plane;
		near_plane.x = mat.m30 + mat.m20;
		near_plane.y = mat.m31 + mat.m21;
		near_plane.z = mat.m32 + mat.m22;
		near_plane.w = mat.m33 + mat.m23;

		Vector4 far_plane;
		far_plane.x = mat.m30 - mat.m20;
		far_plane.y = mat.m31 - mat.m21;
		far_plane.z = mat.m32 - mat.m22;
		far_plane.w = mat.m33 - mat.m23;

		_planes[0] = left_plane;
		_planes[1] = right_plane;
		_planes[2] = bottom_plane;
		_planes[3] = top_plane;
		_planes[4] = near_plane;
		_planes[5] = far_plane;

		NormalizePlanes();
	}

	Frustum::Frustum(float left, float right, float bottom, float top, float near, float far)
	{
		float center_x = (left + right) * 0.5f;
		float center_y = (bottom + top) * 0.5f;
		float center_z = (near + far) * 0.5f;

		Vector4 left_plane;
		left_plane.x = 1;
		left_plane.y = 0;
		left_plane.z = 0;
		left_plane.w = -Vector3(left, center_y, center_z).dotProduct(Vector3(1, 0, 0));

		Vector4 right_plane;
		right_plane.x = -1;
		right_plane.y = 0;
		right_plane.z = 0;
		right_plane.w = -Vector3(right, center_y, center_z).dotProduct(Vector3(-1, 0, 0));

		Vector4 bottom_plane;
		bottom_plane.x = 0;
		bottom_plane.y = 1;
		bottom_plane.z = 0;
		bottom_plane.w = -Vector3(center_x, bottom, center_z).dotProduct(Vector3(0, 1, 0));

		Vector4 top_plane;
		top_plane.x = 0;
		top_plane.y = -1;
		top_plane.z = 0;
		top_plane.w = -Vector3(center_x, top, center_z).dotProduct(Vector3(0, -1, 0));

		Vector4 near_plane;
		near_plane.x = 0;
		near_plane.y = 0;
		near_plane.z = 1;
		near_plane.w = -Vector3(center_x, center_y, near).dotProduct(Vector3(0, 0, 1));

		Vector4 far_plane;
		far_plane.x = 0;
		far_plane.y = 0;
		far_plane.z = -1;
		far_plane.w = -Vector3(center_x, center_y, far).dotProduct(Vector3(0, 0, -1));

		_planes[0] = left_plane;
		_planes[1] = right_plane;
		_planes[2] = bottom_plane;
		_planes[3] = top_plane;
		_planes[4] = near_plane;
		_planes[5] = far_plane;

		NormalizePlanes();
	}

	void Frustum::NormalizePlanes()
	{
		for (int i = 0; i < 6; i++)
		{
			Vector4 &plane = _planes[i];

			float div = 1.0f / Vector3(plane.x, plane.y, plane.z).length();
			plane *= div;
		}
	}

	ContainsResult Frustum::ContainsPoint(const Vector3& point) const
	{
		for (int i = 0; i < 6; i++)
		{
			float dis = DistanceToPlane(point, i);
			if (dis < 0)
			{
				return ContainsResult::Out;
			}
		}

		return ContainsResult::In;
	}

	ContainsResult Frustum::ContainsSphere(const Vector3& center, float radius) const
	{
		bool all_in = true;

		for (int i = 0; i < 6; i++)
		{
			float dis = DistanceToPlane(center, i);
			if (dis < -radius)
			{
				return ContainsResult::Out;
			}

			if (fabs(dis) < radius)
			{
				all_in = false;
			}
		}

		if (!all_in)
		{
			return ContainsResult::Cross;
		}

		return ContainsResult::In;
	}

	ContainsResult Frustum::ContainsBounds(const Vector3& min, const Vector3& max) const
	{
		std::vector<Vector3> corners(8);
		corners[0] = max;
		corners[1] = Vector3(min.x, max.y, max.z);
		corners[2] = Vector3(max.x, min.y, max.z);
		corners[3] = Vector3(max.x, max.y, min.z);
		corners[4] = Vector3(min.x, min.y, max.z);
		corners[5] = Vector3(min.x, max.y, min.z);
		corners[6] = Vector3(max.x, min.y, min.z);
		corners[7] = min;

		return ContainsPoints(corners, NULL);
	}

	ContainsResult Frustum::ContainsPoints(const std::vector<Vector3>& points, const Matrix* matrix) const
	{
		std::vector<Vector3> ps(points.size());
		for (uint32 i = 0; i < points.size(); i++)
		{
			if (matrix != NULL)
			{
				ps[i] = matrix->applyPoint(points[i]);
			}
			else
			{
				ps[i] = points[i];
			}
		}

		uint32 in_plane_count = 0;

		for (uint32 i = 0; i < 6; i++)
		{
			uint32 in_count = 0;

			for (uint32 j = 0; j < ps.size(); j++)
			{
				float dis = DistanceToPlane(ps[j], i);
				if (dis >= 0)
				{
					in_count++;
				}
			}

			// all points in same side to one plane
			if (in_count == 0)
			{
				return ContainsResult::Out;
			}
			else if (in_count == ps.size())
			{
				in_plane_count++;
			}
		}

		if (in_plane_count == 6)
		{
			return ContainsResult::In;
		}

		return ContainsResult::Cross;
	}

	float Frustum::DistanceToPlane(const Vector3& point, int plane_index) const
	{
		Vector4 p(point);
		p.w = 1;

		return _planes[plane_index].dotProduct(p);
	}
}
