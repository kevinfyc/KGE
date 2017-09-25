#ifndef __KGE_MATH_BOUNDS_H__
#define __KGE_MATH_BOUNDS_H__

#include "math_def.h"

namespace kge
{
	class Bounds
	{
	public:
		Bounds(const Vector3& min, const Vector3& max);
		const Vector3& Min() const { return _min; }
		const Vector3& Max() const { return _max; }
		bool Contains(const Vector3& point) const;

	private:
		Vector3 _min;
		Vector3 _max;
	};
}


#endif //__KGE_MATH_BOUNDS_H__