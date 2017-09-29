#ifndef __KGE_MATH_FRUSTUM_H__
#define __KGE_MATH_FRUSTUM_H__

#include "math_def.h"

namespace kge
{
	enum class ContainsResult
	{
		In,
		Out,
		Cross
	};

	class Frustum
	{
	public:
		Frustum() { }

		//
		// 摘要:
		//     ///
		//     构建一个FrustumBounds, 投影类型为perspective.
		//     ///
		//
		// 参数:
		//   mat:
		//     wvp matrix, check pos in model space.
		//     vp matrix, check pos in world space.
		//     p matrix, check pos in view space.
		Frustum(const Matrix& mat);

		//
		// 摘要:
		//     ///
		//     构建一个FrustumBounds, 投影类型为orthographic, in view space.
		//     ///
		Frustum(float left, float right, float bottom, float top, float near, float far);

		ContainsResult ContainsPoint(const Vector3& point) const;
		ContainsResult ContainsSphere(const Vector3& center, float radius) const;
		ContainsResult ContainsBounds(const Vector3& min, const Vector3& max) const;
		ContainsResult ContainsPoints(const std::vector<Vector3>& points, const Matrix* matrix) const;
		float DistanceToPlane(const Vector3& point, int plane_index) const;

	private:
		void NormalizePlanes();

		Vector4 _planes[6];
	};
}


#endif //__KGE_MATH_FRUSTUM_H__