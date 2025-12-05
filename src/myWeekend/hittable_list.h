#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "aabb.h"
#include "hittable.h"

#include <vector>

// This class is used as the "world" or "scene" of the render
class hittable_list : public hittable {
    public:
        std::vector<shared_ptr<hittable>> objects;

        hittable_list() {}
        hittable_list(shared_ptr<hittable> object) { add(object); }

        void clear() { objects.clear(); }

        void add(shared_ptr<hittable> object) {
            objects.push_back(object);
            bbox = aabb(bbox, object->boundingBox());
        }

        // This method gets called by the camera with every outgoing ray to see if it 
        // collides with an object.
        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            hit_record temp_rec;
            bool hit_anything = false;
            auto closest = ray_t.max;

            for (const auto& object : objects) {
                if (object->hit(r, interval(ray_t.min, closest), temp_rec)) {
                    hit_anything = true;
                    closest = temp_rec.t;
                    rec = temp_rec;
                }
            }

            return hit_anything;
        }

        aabb boundingBox() const override { return bbox; }

    private:
        aabb bbox;
};

#endif
