#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"
#include <cmath>

class sphere : public hittable {
    public:
        sphere(const point3& static_center, double radius, shared_ptr<material> mat) 
          : center(static_center, vec3(0,0,0)), radius(std::fmax(0, radius)), mat(mat) {}

        sphere(const point3& center1, const point3& center2, double radius, shared_ptr<material> mat) 
          : center(center1, center2 - center1), radius(std::fmax(0, radius)), mat(mat) {}

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            point3 currentCenter = center.at(r.time());
            vec3 oc = currentCenter - r.origin();
            auto a = r.direction().length_squared();
            auto h = dot(r.direction(), oc);
            auto c = oc.length_squared() - radius*radius;

            auto discriminant = h*h - a*c;
            // if the discriminant in the quadratic is negative it has no solutions, aka the ray
            // "r", does not intersect this sphere at any point
            if (discriminant < 0) 
                return false;

            auto sqrtd = std::sqrt(discriminant);

            // Find nearest root that lies in acceptable range
            auto root = (h - sqrtd) / a;
            if (!ray_t.surrounds(root)) {
                root = (h+sqrtd) / a;
                if (!ray_t.surrounds(root))
                    return false;
            }

            rec.t = root;
            rec.p = r.at(rec.t);
            vec3 outward_normal = (rec.p - currentCenter) / radius;
            rec.setFaceNormal(r, outward_normal);
            rec.mat = mat;

            return true;
        }

    private:
        ray center;
        double radius;
        shared_ptr<material> mat;
};

#endif
