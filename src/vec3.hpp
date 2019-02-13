#ifndef VEC3_HPP
#define VEC3_HPP

#include <cassert>
#include <cmath>
#include <iostream>

class vec3 {
 public:
  vec3() = default;
  vec3(float x, float y, float z);
  inline float r() const;
  inline float g() const;
  inline float b() const;

  inline const vec3& operator+() const;
  inline vec3 operator-() const;
  inline float operator[](int idx) const;
  inline float& operator[](int idx);

  inline vec3 operator+(const vec3& w) const;
  inline vec3 operator-(const vec3& w) const;
  inline vec3 operator*(const vec3& w) const;
  inline vec3 operator/(const vec3& w) const;
  inline vec3 operator*(float t) const;
  inline vec3 operator/(float t) const;
  inline vec3& operator+=(const vec3& v2);
  inline vec3& operator-=(const vec3& v2);
  inline vec3& operator*=(const vec3& v2);
  inline vec3& operator/=(const vec3& v2);
  inline vec3& operator*=(float t);
  inline vec3& operator/=(float t);

  inline float length() const;
  inline float sqr_length() const;

  inline void normalize();

  float x, y, z;
};

inline std::istream& operator>>(std::istream& is, vec3& v);
inline std::ostream& operator>>(std::ostream& os, const vec3& v);

vec3::vec3(float x, float y, float z) : x{x}, y{y}, z{z} {}

inline float vec3::r() const { return x; }

inline float vec3::g() const { return y; }

inline float vec3::b() const { return z; }

inline const vec3& vec3::operator+() const { return *this; }

inline vec3 vec3::operator-() const { return vec3(-x, -y, -z); }

inline float vec3::operator[](int idx) const {
  assert(idx >= 0 && idx < 3);
  switch (idx) {
    case 0:
      return x;
    case 1:
      return y;
    default:
      return z;
  }
}

inline float& vec3::operator[](int idx) {
  assert(idx >= 0 && idx < 3);
  switch (idx) {
    case 0:
      return x;
    case 1:
      return y;
    default:
      return z;
  }
}

inline vec3 vec3::operator+(const vec3& w) const {
  return vec3(x + w.x, y + w.y, z + w.z);
}

inline vec3 vec3::operator-(const vec3& w) const {
  return vec3(x - w.x, y - w.y, z - w.z);
}

inline vec3 vec3::operator*(const vec3& w) const {
  return vec3(x * w.x, y * w.y, z * w.z);
}

inline vec3 vec3::operator/(const vec3& w) const {
  assert(w.x != 0.f && w.y != 0.f && w.z != 0.f);
  return vec3(x / w.x, y / w.y, z / w.z);
}

inline vec3 vec3::operator*(float t) const { return vec3(x * t, y * t, z * t); }

inline vec3 vec3::operator/(float t) const {
  assert(t != 0.f);
  return vec3(x / t, y / t, z / t);
}

inline vec3& vec3::operator+=(const vec3& w) {
  x += w.x;
  y += w.y;
  z += w.z;
  return *this;
}

inline vec3& vec3::operator-=(const vec3& w) {
  x -= w.x;
  y -= w.y;
  z -= w.z;
  return *this;
}

inline vec3& vec3::operator*=(const vec3& w) {
  x *= w.x;
  y *= w.y;
  z *= w.z;
  return *this;
}

inline vec3& vec3::operator/=(const vec3& w) {
  x /= w.x;
  y /= w.y;
  z /= w.z;
  return *this;
}

inline vec3& vec3::operator*=(float t) {
  x *= t;
  y *= t;
  z *= t;
  return *this;
}

inline vec3& vec3::operator/=(float t) {
  x /= t;
  y /= t;
  z /= t;
  return *this;
}

inline float vec3::length() const { return sqrt(x * x + y * y + z * z); }

inline float vec3::sqr_length() const { return x * x + y * y + z * z; }

inline void vec3::normalize() {
  const float k = 1.0 / length();
  x *= k;
  y *= k;
  z *= k;
}

inline float dot(const vec3& v, const vec3& w) {
  return v.x * w.x + v.y * w.y + v.z * w.z;
}

inline vec3 cross(const vec3& v, const vec3& w) {
  return vec3(v.y * w.z - v.z * w.y, -(v.x * w.z - v.z * w.x),
              v.x * w.y - v.y * w.x);
}

inline vec3 normalize(vec3 v) { return v / v.length(); }

inline vec3 operator*(float t, const vec3& v) { return v * t; }

inline std::istream& operator>>(std::istream& is, vec3& v) {
  is >> v.x >> v.y >> v.z;
  return is;
}

inline std::ostream& operator>>(std::ostream& os, const vec3& v) {
  os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
  return os;
}

#endif  // VEC3_HPP
