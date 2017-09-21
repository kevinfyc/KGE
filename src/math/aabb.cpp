
#include "aabb.h"
#include "triangle.h"

namespace ora
{

    AABB::AABB()
    {

    }

    AABB::AABB(const Vector3 & min_, const Vector3 & max_)
        : min(min_)
        , max(max_)
    {

    }

    void AABB::reset()
    {
        min = Vector3Max;
        max = Vector3Min;
    }

    bool AABB::intersectsRay(const Vector3 & origin, const Vector3 & dir, float *pDist/*=nullptr*/) const
    {
        if(pDist) *pDist = FloatMax;

        Vector3 base(0.0f, 0.0f, 0.0f);
        int couter = 0;

        for (int i = 0; i < 3; ++i)
        {
            if (origin[i] <= min[i])
                base[i] = min[i];
            else if (origin[i] >= max[i])
                base[i] = max[i];
            else
            {
                //该方向在包围盒里
                couter++;
                float offset_min_v = origin[i] - min[i];
                float offset_max_v = max[i] - origin[i];

                if (offset_min_v < offset_max_v)
                    base[i] = min[i];
                else
                    base[i] = max[i];
            }
        }

        if (couter == 3) //起点在包围盒里
        {
            if(pDist) *pDist = -1.0f;
            return true;
        }

        Vector3 ptOnPlane;
        for (int axis = 0; axis < 3; axis++)
        {
            if (dir[axis] != 0.f)
            {
                float t = (base[axis] - origin[axis]) / dir[axis];

                if (t > 0.f)
                {
                    ptOnPlane = origin + t * dir;

                    int nextAxis = (axis + 1) % 3;
                    int prevAxis = (axis + 2) % 3;

                    if (min[nextAxis] < ptOnPlane[nextAxis] && ptOnPlane[nextAxis] < max[nextAxis] &&
                            min[prevAxis] < ptOnPlane[prevAxis] && ptOnPlane[prevAxis] < max[prevAxis])
                    {
                        if(pDist) *pDist = t;
                        return true;
                    }
                }
            }
        }


        return false;
    }

    bool AABB::clipLine(Vector3 & start, Vector3 & end) const
    {
        float sMin = 0.0f;
        float eMax = 1.0f;

        Vector3 delta = end - start;

        for (int i = 0; i < 3; ++i)
        {
            if (delta[i] != 0)
            {
                float dMin = (min[i] - start[i]) / delta[i];
                float dMax = (max[i] - start[i]) / delta[i];

                if (delta[i] > 0)//射线方向从min指向max
                {
                    sMin = max2(sMin, dMin);
                    eMax = min2(eMax, dMax);
                }
                else//射线方向从max指向min，dMax应该和dMin交换
                {
                    sMin = max2(sMin, dMax);
                    eMax = min2(eMax, dMin);
                }
            }
            else if (start[i] < min[i] || start[i] > max[i]) //起点不在包围盒范围。
                return false;
        }

        end = start + eMax * delta;
        start = start + sMin * delta;

        return true;
    }

    bool AABB::intersect(const AABB & aabb) const
    {
        for (int i = 0; i < 3; ++i)
        {
            if (max[i] < aabb.min[i] || min[i] > aabb.max[i])
            {
                return false;
            }
        }
        return true;
    }

    void AABB::normalization()
    {
        for (int i = 0; i < 3; ++i)
        {
            if (min[i] > max[i]) std::swap(min[i], max[i]);
        }
    }

    void AABB::applyMatrix(const Matrix & mat)
    {
        Vector3 corners[8];
        // Near face, specified counter-clockwise
        // Left-top-front.
        corners[0].set(min.x, max.y, max.z);
        // Left-bottom-front.
        corners[1].set(min.x, min.y, max.z);
        // Right-bottom-front.
        corners[2].set(max.x, min.y, max.z);
        // Right-top-front.
        corners[3].set(max.x, max.y, max.z);

        // Far face, specified clockwise
        // Right-top-back.
        corners[4].set(max.x, max.y, min.z);
        // Right-bottom-back.
        corners[5].set(max.x, min.y, min.z);
        // Left-bottom-back.
        corners[6].set(min.x, min.y, min.z);
        // Left-top-back.
        corners[7].set(min.x, max.y, min.z);

        reset();
        for(int i = 0; i < 8; ++i)
        {
            corners[i] = mat.applyPoint(corners[i]);
            addPoint(corners[i]);
        }
    }

    void AABB::applyMatrix(AABB & out, const Matrix & matrix) const
    {
        out = *this;
        out.applyMatrix(matrix);
    }

    void AABB::add(const AABB & aabb)
    {
        min.x = min2(min.x, aabb.min.x);
        min.y = min2(min.y, aabb.min.y);
        min.z = min2(min.z, aabb.min.z);

        max.x = max2(max.x, aabb.max.x);
        max.y = max2(max.y, aabb.max.y);
        max.z = max2(max.z, aabb.max.z);
    }

    void AABB::addPoint(const Vector3 & pt)
    {
        min.x = min2(min.x, pt.x);
        min.y = min2(min.y, pt.y);
        min.z = min2(min.z, pt.z);

        max.x = max2(max.x, pt.x);
        max.y = max2(max.y, pt.y);
        max.z = max2(max.z, pt.z);
    }

    void AABB::addTriangle(const Triangle & tri)
    {
        addPoint(tri.a);
        addPoint(tri.b);
        addPoint(tri.c);
    }

    void AABB::sub(const AABB & aabb)
    {
        min.x = max2(min.x, aabb.min.x);
        min.y = max2(min.y, aabb.min.y);
        min.z = max2(min.z, aabb.min.z);

        max.x = min2(max.x, aabb.max.x);
        max.y = min2(max.y, aabb.max.y);
        max.z = min2(max.z, aabb.max.z);
    }

    Vector3 AABB::getCenter() const
    {
        Vector3 v;
        getCenter(v);
        return v;
    }

    void AABB::getCenter(Vector3 & center) const
    {
        center = (max + min) * 0.5f;
    }

    Vector3 AABB::getDiameter() const
    {
        Vector3 v;
        getDiameter(v);
        return v;
    }

    void AABB::getDiameter(Vector3 & diameter) const
    {
        diameter = max - min;
    }


}//end namespace ora
