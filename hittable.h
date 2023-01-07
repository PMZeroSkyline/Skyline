#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "vec3.h"
#include "aabb.h"

class material;

struct hit_record
{
    point3 p;
    vec3 normal;
    double t;
    
    bool front_face;

    shared_ptr<material> mat_ptr;

    double u;
    double v;

    inline void set_face_normal(const ray& r, const vec3 &outward_normal)
    {
        front_face = dot(r.dir, outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable
{
public:
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record &rec) const = 0;
    virtual bool bounding_box(double time0, double time1, aabb &output_box) const = 0;
};

class translate : public hittable
{
public:
    shared_ptr<hittable> ptr;
    vec3 offset;

    translate(shared_ptr<hittable> p, const vec3 &displacement) : ptr(p), offset(displacement){}

    virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;
    virtual bool bounding_box(double time0, double time1, aabb &output_box) const override;
};
bool translate::hit(const ray &r, double t_min, double t_max, hit_record &rec) const
{
    ray moved_r(r.orig - offset, r.dir, r.tm);
    if (!ptr->hit(moved_r, t_min, t_max, rec))
    {
        return false;
    }
    rec.p += offset;
    rec.set_face_normal(moved_r, rec.normal);
    return true;
}
bool translate::bounding_box(double time0, double time1, aabb &output_box) const
{
    if (!ptr->bounding_box(time0, time1, output_box))
    {
        return false;
    }
    output_box = aabb(output_box.minimum + offset, output_box.maximum + offset);
    return true;
}
//class rotate_y : public hittable
//{
//public:
//    shared_ptr<hittable> ptr;
//    double sin_theta;
//    double cos_theta;
//    bool hasbox;
//    aabb bbox;
//
//    rotate_y(shared_ptr<hittable> p, double angle);
//    virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;
//    virtual bool bounding_box(double time0, double time1, aabb &output_box) const override
//    {
//        output_box = bbox;
//        return hasbox;
//    }
//};
//rotate_y::rotate_y(shared_ptr<hittable> p, double angle) : ptr(p)
//{
//    auto radians = degrees_to_radians(angle);
//    sin_theta = sin(radians);
//    cos_theta = cos(radians);
//    hasbox = ptr->bounding_box(0, 1, bbox);
//}
#endif