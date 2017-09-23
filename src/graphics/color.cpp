//
//  vertex_buffer.cpp
//  libkge
//
//  Created by kevin on 17-9-23.
//
//

#include "color.h"
#include "math/_math.hpp"

namespace kge
{
	Color::Color(float r, float g, float b, float a) :
		r(r), g(g), b(b), a(a)
	{
	}

	bool Color::operator ==(const Color& c) const
	{
		return
			almostEqual(r, c.r) &&
			almostEqual(g, c.g) &&
			almostEqual(b, c.b) &&
			almostEqual(a, c.a);
	}

	bool Color::operator !=(const Color& c) const
	{
		return !(*this == c);
	}

	Color Color::Lerp(const Color& from, const Color& to, float t, bool clamp_01)
	{
		if (clamp_01)
			t = clamp01(t);

		return Color(
			lerp(from.r, to.r, t),
			lerp(from.g, to.g, t),
			lerp(from.b, to.b, t),
			lerp(from.a, to.a, t));
	}

	Color Color::operator *(const Color& c) const
	{
		return Color(r * c.r, g * c.g, b * c.b, a * c.a);
	}

	Color& Color::operator *=(const Color& c)
	{
		r *= c.r;
		g *= c.g;
		b *= c.b;
		a *= c.a;
		return *this;
	}

	Color Color::operator *(float v) const
	{
		return Color(r * v, g * v, b * v, a * v);
	}

	Color Color::operator /(float v) const
	{
		return *this * (1 / v);
	}
}

