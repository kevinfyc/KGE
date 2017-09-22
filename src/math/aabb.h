#ifndef _MATH_AABB_H
#define _MATH_AABB_H

#include "math_def.h"

namespace kge
{
    class Triangle;

    class AABB
    {
    public:

        Vector3 min;
        Vector3 max;


        AABB();
        AABB(const Vector3 & min, const Vector3 & max);

        void reset();

        //正规化。aabb经过变换后，min可能要大于max。
        void normalization();

        Vector3 getCenter() const;
        void getCenter(Vector3 & center) const;

        Vector3 getDiameter() const;
        void getDiameter(Vector3 & diameter) const;

        ///包围盒求交集
        void sub(const AABB & aabb);

        ///包围盒求并集
        void add(const AABB & aabb);

        ///并入点
        void addPoint(const Vector3 & pt);

        ///并入三角形
        void addTriangle(const Triangle & tri);

        bool intersectsRay(const Vector3 & origin, const Vector3 & dir, float *pDist = nullptr) const;

        bool intersect(const AABB & aabb) const;

        bool clipLine(Vector3 & start, Vector3 & end) const;

        void applyMatrix(const Matrix & mat);
        void applyMatrix(AABB & out, const Matrix & mat) const;
    };




}//end namespace kge

#endif //_MATH_AABB_H