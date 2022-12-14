#ifndef AABB_H
#define AABB_H

#include "core.h"
#include "vec3.h"
#include "ray.h"

class aabb
{
public:
    point3 minimum;
    point3 maximum;

    aabb(){};
    aabb(const point3 &a, const point3 &b)
    {
        minimum = a;
        maximum = b;
    }

    inline bool hit(const ray &r, double t_min, double t_max) const;
};

inline bool aabb::hit(const ray &r, double t_min, double t_max) const
{
    for (size_t i = 0; i < 3; i++)
    {
        double invD = 1.0f / r.dir[i];
        double t0 = (minimum[i] - r.orig[i]) * invD;
        double t1 = (maximum[i] - r.orig[i]) * invD;
        if (invD < 0.0f)
        {
            swap(t0, t1);
        }
        t_min = t0 > t_min ? t0 : t_min;
        t_max = t1 < t_max ? t1 : t_max;
        if (t_max <= t_min)
        {
            return false;
        }
    }
    return true;
}

aabb surrounding_box(aabb box0, aabb box1)
{
    point3 small(fmin(box0.minimum.x(), box1.minimum.x()), fmin(box0.minimum.y(), box1.minimum.y()), fmin(box0.minimum.z(), box1.minimum.z()));
    point3 big(fmax(box0.maximum.x(), box1.maximum.x()), fmax(box0.maximum.y(), box1.maximum.y()), fmax(box0.maximum.z(), box1.maximum.z()));
    return aabb(small, big);
}
#endif