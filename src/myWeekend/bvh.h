#ifndef BVH_H
#define BVH_H

#include "aabb.h"
#include "hittable.h"
#include "hittable_list.h"

#include <algorithm>

class bvh_node : public hittable {
    public:
        bvh_node(hittable_list list) : bvh_node(list.objects, 0, list.objects.size()) {
        }

        bvh_node(std::vector<shared_ptr<hittable>>& objs, size_t start, size_t end) {
            size_t objSpan = end - start;
            int axis = randomInt(0,2);
            auto sortFunc = (axis == 0) ? boxCompareX
                          : (axis == 1) ? boxCompareY
                                        : boxCompareZ;

            if (objSpan == 1) {
                left = right = objs[start];
            } else if (objSpan == 2) {
                left = objs[start];
                right = objs[start+1];
            } else {
                std::sort(std::begin(objs) + start, std::begin(objs) + end, sortFunc);
                auto mid = start + objSpan/2;
                left = make_shared<bvh_node>(objs, start, mid);
                right = make_shared<bvh_node>(objs, mid, end);
        }

        bbox = aabb(left->boundingBox(), right->boundingBox());        }

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            if (!bbox.hit(r, ray_t))
                return false;

            bool hitLeft = left->hit(r, ray_t, rec);
            bool hitRight = right->hit(r, interval(ray_t.min, hitLeft ? rec.t : ray_t.max), rec);

            return hitLeft || hitRight;
        }

        aabb boundingBox() const override { return bbox; }

    private:
        shared_ptr<hittable> left;
        shared_ptr<hittable> right;
        aabb bbox;

        static bool boxCompare(const shared_ptr<hittable> a, const shared_ptr<hittable> b, int axisIndex) {
            return a->boundingBox().getAxisInterval(axisIndex).min < b->boundingBox().getAxisInterval(axisIndex).min;
        }

        static bool boxCompareX(const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
            return boxCompare(a, b, 0);
        }

        static bool boxCompareY(const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
            return boxCompare(a, b, 1);
        }

        static bool boxCompareZ(const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
            return boxCompare(a, b, 2);
        }
};

#endif
