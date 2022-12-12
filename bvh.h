#ifndef BVH_H
#define BVH_H

#include "core.h"
#include "hittable.h"
#include "hittable_list.h"

class bvh_node : public hittable
{
public:
    shared_ptr<hittable> left;
    shared_ptr<hittable> right;
    aabb box;
    bvh_node();
    bvh_node(const vector<shared_ptr<hittable>> &src_objects, size_t start, size_t end, double time0, double time1);
    bvh_node(const hittable_list &list, double time0, double time1) : bvh_node(list.objects, 0, list.objects.size(), time0, time1){}
    virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;
    virtual bool bounding_box(double time0, double time1, aabb &output_box) const override;
};
bool bvh_node::bounding_box(double time0, double time1, aabb &output_box) const
{
    output_box = box;
    return true;
}
#endif