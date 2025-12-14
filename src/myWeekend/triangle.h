#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "hittable.h"
#include "vec3.h"

#include <cmath>

class triangle : public hittable {
  public:
    triangle(const point3& v0, const point3& v1, const point3& v2,
        shared_ptr<material> mat)
      : v0(v0), v1(v1), v2(v2), uv0(0, 0, 0), uv1(1, 0, 0), uv2(0, 1, 0), mat(mat)
    {
        setBoundingBox();
        setNormal();
    }

    triangle(const point3& v0, const point3& v1, const point3& v2, const vec3& uv0,
        const vec3& uv1, const vec3& uv2, shared_ptr<material> mat)
      : v0(v0), v1(v1), v2(v2), uv0(uv0), uv1(uv1), uv2(uv2), mat(mat)
    {
        setBoundingBox();
        setNormal();
    }

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        // Match the lecture implementation:
        //   n  = (b - a) x (c - a)
        //   t  = dot(n, (a - ray_origin)) / dot(n, ray_dir)
        //   p  = ray(t)
        //   na = (c - b) x (p - b)
        //   nb = (a - c) x (p - c)
        //   nc = (b - a) x (p - a)
        //   bary[i] = dot(n, n{i}) / dot(n, n)
        //   inside if all bary[i] >= 0

        const point3& a = v0;
        const point3& b = v1;
        const point3& c = v2;

        const vec3 ab = b - a;
        const vec3 ac = c - a;

        const vec3 n = cross(ab, ac);
        const double nn = dot(n, n);
        if (nn == 0.0)
            return false;

        const double denom = dot(n, r.direction());
        if (std::fabs(denom) < 1e-8)
            return false;

        const double t = dot(n, a - r.origin()) / denom;
        if (!ray_t.contains(t))
            return false;

        const point3 p = r.at(t);

        const vec3 na = cross(c - b, p - b);
        const vec3 nb = cross(a - c, p - c);
        const vec3 nc = cross(b - a, p - a);

        const double bary_a = dot(n, na) / nn;
        const double bary_b = dot(n, nb) / nn;
        const double bary_c = dot(n, nc) / nn;

        if (bary_a < 0.0 || bary_b < 0.0 || bary_c < 0.0)
            return false;

        rec.t = t;
        rec.p = p;
        rec.mat = mat;

        rec.setFaceNormal(r, normal);

        rec.u = bary_a * uv0.x() + bary_b * uv1.x() + bary_c * uv2.x();
        rec.v = bary_a * uv0.y() + bary_b * uv1.y() + bary_c * uv2.y();

        return true;
    }

    aabb boundingBox() const override { return bbox; }

  private:
    point3 v0, v1, v2;
    vec3 uv0, uv1, uv2;
    shared_ptr<material> mat;

    aabb bbox;
    vec3 normal;

    void setBoundingBox() {
        const auto min_x = std::fmin(v0.x(), std::fmin(v1.x(), v2.x()));
        const auto min_y = std::fmin(v0.y(), std::fmin(v1.y(), v2.y()));
        const auto min_z = std::fmin(v0.z(), std::fmin(v1.z(), v2.z()));

        const auto max_x = std::fmax(v0.x(), std::fmax(v1.x(), v2.x()));
        const auto max_y = std::fmax(v0.y(), std::fmax(v1.y(), v2.y()));
        const auto max_z = std::fmax(v0.z(), std::fmax(v1.z(), v2.z()));

        bbox = aabb(point3(min_x, min_y, min_z), point3(max_x, max_y, max_z));
    }

    void setNormal() {
        const vec3 e1 = v1 - v0;
        const vec3 e2 = v2 - v0;
        normal = convertToUnitVector(cross(e1, e2));
    }
};

#endif


