#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

class sphere : public hittable
{
public:

    point3 center;
    double radius;
    shared_ptr<material> mat_ptr;

    sphere(){}
    sphere(point3 cen, double r, shared_ptr<material> m) : center(cen), radius(r), mat_ptr(m){}

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record &rec) const override;
    virtual bool bounding_box(double time0, double time1, aabb &output_box) const override;

    static void get_sphere_uv(const point3 &p, double &u, double &v)
    {
        double theta = acos(-p.y());
        double phi = atan2(-p.z(), p.x()) + pi;
        u = phi / (2 * pi);
        v = theta / pi;
    }
};
bool sphere::hit(const ray &r, double t_min, double t_max, hit_record &rec) const
{
    vec3 oc = r.orig - center;
    double a = r.dir.length_squared();
    double half_b = dot(oc, r.dir);
    double c = oc.length_squared() - radius * radius;

    double discriminant = half_b * half_b - a * c;
    if (discriminant < 0)
    {
        return false;
    }
    double sqrtd = sqrt(discriminant);

    double root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root)
    {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
        {
            return false;
        }
    }
    rec.t = root;
    rec.p = r.at(rec.t);
    vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);

    rec.mat_ptr = mat_ptr;

    get_sphere_uv(outward_normal, rec.u, rec.v);

    return true;
}
bool sphere::bounding_box(double time0, double time1, aabb &output_box) const
{
    output_box = aabb(center - vec3(radius), center + vec3(radius));
    return true;
}
#endif