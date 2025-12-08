#ifndef QUAD_H
#define QUAD_H

#include "hittable.h"

class quad : public hittable {
  public:
    quad(const point3& Q, const vec3& u, const vec3& v, shared_ptr<material> mat)
      : Q(Q), u(u), v(v), mat(mat)
    {
        setBoundingBox();
    }

    virtual void setBoundingBox() {
        auto bboxDiagonal1 = aabb(Q, Q + u + v);
        auto bboxDiagonal2 = aabb(Q + u, Q + v);
        bbox = aabb(bboxDiagonal1, bboxDiagonal2);
    }

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        return false;
    }

    aabb boundingBox() const override { return bbox; }

  private:
    point3 Q;
    vec3 u, v;
    shared_ptr<material> mat;
    aabb bbox;
};

#endif
