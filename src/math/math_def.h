#ifndef _MATH_DEF_H
#define _MATH_DEF_H

#include <vector>
#include <cmath>
#include <cassert>

#include "vector2.hpp"
#include "vector3.hpp"
#include "vector4.hpp"
#include "quat.hpp"
#include "matrix.hpp"

namespace kge
{
    
    const Vector3 Vector3Zero(0, 0, 0);
    const Vector3 Vector3Min(FloatMin, FloatMin, FloatMin);
    const Vector3 Vector3Max(FloatMax, FloatMax, FloatMax);


    typedef std::vector<size_t>     IndicesArray;

    enum SIDE
    {
        SIDE_ON = 0,
        SIDE_FRONT = 1,
        SIDE_BACK = 2,
        SIDE_BOTH = 3,
        SIDE_FORCE_DWORD = 0x7fffffff
    };

}// end namespace kge


#endif //_MATH_DEF_H