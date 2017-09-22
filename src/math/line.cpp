#include "line.h"

namespace kge
{
    Line::Line()
    {}

    Line::Line(const Vector3 & s, const Vector3 & e)
        : start(s), end(e)
    {}

    float Line::length() const
    {
        return start.distance(end);
    }

    Vector3 Line::direction() const
    {
        Vector3 dir = end - start;
        dir.normalise();
        return dir;
    }

    //线性插值
    Vector3 Line::interpolation(float ratio) const
    {
        return end * ratio + start * (1.0f - ratio);
    }

    //线性插值
    Vector3 Line::interpolation(float ratioStart, float ratioEnd) const
    {
        float ratioTotal = ratioStart + ratioEnd;
        if (ratioTotal == 0.0f)
        {
            return start;
        }

        float ratioInverse = 1.0f / ratioTotal;
        return (end * ratioStart + start * ratioEnd) * ratioInverse;
    }

    Vector3 Line::project(const Vector3 & point)
    {
        Vector3 n = direction(); //直线方向
        Vector3 sp = point - start; //start -> point
        float d = sp.dotProduct(n); //边sp在直线上的投影长度
        Vector3 q = start + n * d; //投影点
        return q;
    }

    bool Line::intersect(Vector3 & cross, const Line & line)
    {
        Vector3 n1 = direction();
        Vector3 n2 = line.direction();
        Vector3 n1xn2 = n1.crossProduct(n2);
        if (almostZero(n1xn2)) //直线平行
        {
            return false;
        }

        float dn1xn2 = 1.0f / n1xn2.lengthSquared();
        Vector3 p1p2 = line.start - start;

        float d1 = p1p2.crossProduct(n2).dotProduct(n1xn2) * dn1xn2;
        float d2 = p1p2.crossProduct(n1).dotProduct(n1xn2) * dn1xn2;

        Vector3 q1 = start + n1 * d1;
        Vector3 q2 = line.start + n2 * d2;
        if (q1.distanceSquared(q2) != 0.0f)
        {
            return false;
        }

        cross = q1;
        return true;
    }

}