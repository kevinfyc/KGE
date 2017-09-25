#ifndef __KGE_MATH_RECT_H__
#define __KGE_MATH_RECT_H__

#include "math_def.h"

namespace kge
{
    class Rect
    {
	public:
		explicit Rect(float x = 0, float y = 0, float width = 0, float height = 0) :
			x(x),
			y(y),
			width(width),
			height(height)
		{
		}

		void Set(float x, float y, float width, float height)
		{
			this->x = x;
			this->y = y;
			this->width = width;
			this->height = height;
		}

		bool operator ==(const Rect &r) const;
		bool operator !=(const Rect &r) const;

		float x;
		float y;
		float width;
		float height;

    };
}


#endif //__KGE_MATH_RECT_H__