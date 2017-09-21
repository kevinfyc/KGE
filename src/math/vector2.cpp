
#include "vector2.hpp"

#ifndef CODE_INLINE
    #include "vector2.ipp"
#endif

#include <iomanip>

namespace ora
{

    Vector2 Vector2::s_zero(0.f, 0.f);

    /**
     *	This function implements the output streaming operator for Vector2.
     *
     *	@relates Vector2
     */
    std::ostream& operator <<(std::ostream& o, const Vector2& t)
    {
        const int fieldWidth = 8;

        o.put('(');
        o.width(fieldWidth);
        o << t[0];
        o.put(',');
        o.width(fieldWidth);
        o << t[1];
        o.put(')');

        return o;
    }

    /**
     *	This function implements the input streaming operator for Vector2.
     *
     *	@relates Vector2
     */
    std::istream& operator >>(std::istream& i, Vector2& t)
    {
        char dummy;
        i >> dummy >> t[0] >> dummy >> t[1] >> dummy;

        return i;
    }

}// end namespace ora

// vector2.cpp
