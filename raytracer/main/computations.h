#ifndef COMPUTATIONS_H
#define COMPUTATIONS_H

#include "sphere.h"
#include "point.h"
#include "vector.h"
#include "intersection.h"
#include "ray.h"
#include <memory>

struct Computations {
    double t{-1};
    std::shared_ptr<const Shape> object;
    Point point{0,0,0};
    Vector eyev{0,0,0};
    Vector normalv{0,0,0};
    Vector reflectv{0,0,0};
    Point over_point{point + normalv * EPSILON};
    bool inside{false};
};


inline Vector reflect(const Vector& direction, const Vector& normal) {
    return Vector{0,0,0};
}

inline Computations prepare_computations(const Intersection& i, const Ray& r) {
    Computations comps;

    comps.t = i.t;
    comps.object = i.object;
    comps.point = r.position(comps.t);
    comps.eyev = -r.direction();
    comps.normalv = comps.object->normal_at(comps.point);

    if (comps.normalv.dot(comps.eyev) < 0) {
        comps.inside = true;
        comps.normalv = -comps.normalv;
    } else {
        comps.inside = false;
    }

    comps.reflectv = r.direction().reflect(comps.normalv);

    comps.over_point = comps.point + comps.normalv * EPSILON;

    return comps;
}

#endif //COMPUTATIONS_H
