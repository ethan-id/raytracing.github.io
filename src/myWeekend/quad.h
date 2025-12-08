#ifndef QUAD_H
#define QUAD_H

#include "hittable.h"
#include "myWeekend/vec3.h"

class quad : public hittable {
  public:
    quad(const point3& Q, const vec3& u, const vec3& v, shared_ptr<material> mat)
      : Q(Q), u(u), v(v), mat(mat)
    {
        auto n = cross(u, v);
        normal = convertToUnitVector(n);
        D = dot(normal, Q);
        w = n/dot(n,n);
        setBoundingBox();
    }

    virtual void setBoundingBox() {
        auto bboxDiagonal1 = aabb(Q, Q + u + v);
        auto bboxDiagonal2 = aabb(Q + u, Q + v);
        bbox = aabb(bboxDiagonal1, bboxDiagonal2);
    }

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        // If ray is parallel to plane  OR  hit point parameter t is outside the ray interval.
        //      No hit, return false
        auto denom = dot(normal, r.direction());
        auto t = (D - dot(normal, r.origin())) / denom;
        if (std::fabs(denom) < 1e-8 || !ray_t.contains(t))
            return false;

        auto intersection = r.at(t);

        rec.t = t;
        rec.p = intersection;
        rec.mat = mat;
        rec.setFaceNormal(r, normal);

        return true;
    }

    aabb boundingBox() const override { return bbox; }

  private:
    point3 Q;
    vec3 u, v;
    vec3 w;
    shared_ptr<material> mat;
    aabb bbox;
    vec3 normal;
    double D;
};

#endif
