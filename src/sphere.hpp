#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "hitable.hpp"

class sphere : public hitable {
public:
  sphere() = default;
  sphere(vec3 c, float r);
  virtual bool hit(const ray& r, float t_min, float t_max, hit_record& hitrec) const;

  vec3 center;
  float radius;
};

sphere::sphere(vec3 c, float r)
  : center(c), radius(r) {

}

bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& hitrec) const {
  const vec3 oc = r.origin() - center;
  const float a = dot(r.direction(), r.direction());
  const float b = 2.0f*dot(r.direction(), oc);
  const float c = dot(oc, oc) - radius*radius;
  const float discriminant = b*b - 4.0f*a*c;

  if (discriminant > 0.0f) {
    float tmp = (-b - sqrt(discriminant)) / (2.0f*a);
    if (tmp < t_max && tmp > t_min) {
      hitrec.t = tmp;
      hitrec.p = r(tmp);
      hitrec.normal = (hitrec.p - center) / radius;
      return true;
    }
    tmp = (-b + sqrt(discriminant)) / (2.0f*a);
    if (tmp < t_max && tmp > t_min) {
      hitrec.t = tmp;
      hitrec.p = r(tmp);
      hitrec.normal = (hitrec.p - center) / radius;
      return true;
    }
  }
  return false;
}

#endif // SPHERE_HPP
