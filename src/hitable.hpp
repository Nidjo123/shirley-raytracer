#ifndef HITABLE_HPP
#define HITABLE_HPP

struct hit_record {
  float t;
  vec3 p;
  vec3 normal;
};

class hitable {
public:
  virtual bool hit(const ray& r, float t_min, float t_max, hit_record& hitrec) const = 0;
};

#endif // HITABLE_HPP
