#include "bounds.h"

namespace kge
{
	Bounds::Bounds(const Vector3& min, const Vector3& max) :
		_min(min),
		_max(max)
	{
	}

	bool Bounds::Contains(const Vector3& point) const
	{
		return !(point.x < _min.x || point.y < _min.y || point.z < _min.z ||
			point.x > _max.x || point.y > _max.y || point.z > _max.z);
	}
}