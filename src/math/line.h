#ifndef _LINE_H
#define _LINE_H

#include "math_def.h"

namespace kge
{
    ///直线
    class Line
    {
    public:
        Vector3 start;
        Vector3 end;

        Line();
        Line(const Vector3 & s, const Vector3 & e);

        float length() const;

        Vector3 direction() const;

        //线性插值
        Vector3 interpolation(float ratio) const;

        //线性插值
        Vector3 interpolation(float ratioStart, float ratioEnd) const;

        //点在直线上的投影
        Vector3 project(const Vector3 & point);

        bool intersect(Vector3 & cross, const Line & line);

    };
}


#endif //_LINE_H