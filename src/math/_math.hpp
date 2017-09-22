//
//  _math.hpp
//  liborange
//
//  Created by zhoubao on 14-6-4.
//
//

#ifndef liborange__math_h
#define liborange__math_h


#include <iostream>
#include <cmath>
#include <cassert>

// NOMINMAX makes sure that windef.h doesn't add macros min and max
#ifdef WIN32
#define NOMINMAX
#endif

#ifdef CODE_INLINE
#   define INLINE inline
#else
#   define INLINE
#endif

#define MF_ASSERT_DEBUG assert

#define MATH_FLOAT_SMALL            1.0e-37f
#define MATH_TOLERANCE              2e-37f
#define MATH_EPSILON                0.000001f

#define MATH_PI 3.14159265f

#ifndef PI_FULL
#define PI_FULL MATH_PI
#endif // PI_FULL

#ifndef PI_HALF
#define PI_HALF PI_FULL * 0.5f
#endif // PI_HALF

#ifndef PI_QUAR
#define PI_QUAR PI_HALF * 0.5f
#endif // PI_QUAR

namespace kge
{

    typedef signed char     int8;
    typedef unsigned char   uint8;
    typedef short           int16;
    typedef unsigned short  uint16;
    typedef int             int32;
    typedef unsigned int    uint32;

    typedef uint32          uint;


    const float FloatMax = 1e30f;
    const float FloatMin = -FloatMax;
    const float DefaultEpsilon = 0.000001f;


    ///×ø±êÖáË÷Òý
    enum AXIS_INDEX
    {
        AXIS_X = 0,
        AXIS_Y = 1,
        AXIS_Z = 2
    };

    template<typename T>
    inline const T & min2(const T & a, const T & b)
    {
        return a < b ? a : b;
    }

    template<typename T>
    inline const T & min3(const T & a, const T & b, const T & c)
    {
        return (a < b) ? min2(a, c) : min2(b, c);
    }

    template<typename T>
    inline const T & max2(const T & a, const T & b)
    {
        return a > b ? a : b;
    }

    template<typename T>
    inline const T & max3(const T & a, const T & b, const T & c)
    {
        return (a > b) ? max2(a, c) : max2(b, c);
    }

    inline bool almostZero(float d, float epsilon = DefaultEpsilon)
    {
        return d > -epsilon && d < epsilon;
    }

    inline bool almostEqual(float a, float b, float epsilon = DefaultEpsilon)
    {
        return fabs(a - b) < epsilon;
    }

    template<typename T>
    const T & clamp(const T & low, const T & v, const T & hig)
    {
        if (v < low) return low;
        else if (v > hig) return hig;

        return v;
    }

    template<typename T>
    void limit_in_range(T & v, const T & low, const T & high)
    {
        if (v < low) v = low;
        else if (v > high) v = high;
    }

    template<typename T>
    void limit_in_order(T & a, T & b)
    {
        if (a > b) std::swap(a, b);
    }
}

#endif
