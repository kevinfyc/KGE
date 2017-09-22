#ifndef _TRIANGLE_H
#define _TRIANGLE_H

#include "math_def.h"

namespace kge
{
    class Line;

    ///三角形
    class Triangle
    {
    public:
        Vector3 a, b, c;


        Triangle();
        Triangle(const Vector3 & A, const Vector3 & B, const Vector3 & C);

        Vector3 normal() const;

        Vector3 & operator [](int i);
        const Vector3 & operator [](int i) const;

        bool isPointIn(const Vector3 & p) const;

        bool isLineIntersect(const Line & line, Vector3 * cross = NULL) const;

        bool isSphereIntersect(const Vector3 & center, float radius, Vector3 * cross = NULL) const;

        void applyMatrix(const Matrix & mat);

        bool intersect(const Triangle & triangle, const Vector3 & offset) const;

        bool intersectRay(const Vector3 & start, const Vector3 & dir, float *pDist = NULL) const;
    };


    typedef std::vector<int> TriangleIndices;
    typedef std::vector<Triangle> TriangleSet;


}//namespace Math


#endif //_TRIANGLE_H