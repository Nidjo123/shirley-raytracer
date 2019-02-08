#ifndef RAY_HPP
#define RAY_HPP

#include "vec3.hpp"

class ray {
public:
  ray() = default;
  ray(const vec3& a, const vec3& b);
  vec3 origin() const;
  vec3 direction() const;
  vec3 at(float t) const;
  vec3 operator()(float t) const;

  vec3 A;
  vec3 B;
};

ray::ray(const vec3& a, const vec3& b)
  : A(a), B(b) {

}

vec3 ray::origin() const {
  return A;
}

vec3 ray::direction() const {
  return B;
}

vec3 ray::at(float t) const {
  return (*this)(t);
}

vec3 ray::operator()(float t) const {
  return A + t*B;
}

#endif // RAY_HPP
