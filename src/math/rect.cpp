#include "rect.h"

namespace kge
{
	bool Rect::operator ==(const Rect& r) const
	{
		return almostEqual(x, r.x) &&
			almostEqual(y, r.y) &&
			almostEqual(width, r.width) &&
			almostEqual(height, r.height);
	}

	bool Rect::operator !=(const Rect& r) const
	{
		return !(*this == r);
	}
}