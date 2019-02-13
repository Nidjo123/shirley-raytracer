#ifndef HITABLE_HPP
#define HITABLE_HPP

#include "ray.hpp"
#include "vec3.hpp"

class material;

struct hit_record {
  float t;
  vec3 p;
  vec3 normal;
  material* mat;
};

class hitable {
 public:
  virtual bool hit(const ray& r, float t_min, float t_max,
                   hit_record& hitrec) const = 0;
};

#endif  // HITABLE_HPP
