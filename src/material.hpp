#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "vec3.hpp"
#include "hitable.hpp"

vec3 random_in_unit_sphere();

class material {
public:
  virtual bool scatter(const ray& r_in, const hit_record &rec, vec3& attenuation, ray& scattered) const = 0;
};

// materials
class lambertian;
class metal;


class lambertian : public material {
public:
  lambertian(const vec3& a);
  virtual bool scatter(const ray& r_in, const hit_record &rec, vec3& attenuation, ray& scattered) const;

  vec3 albedo;
};



lambertian::lambertian(const vec3& a)
  : albedo(a) {

}

bool lambertian::scatter(const ray& r_in, const hit_record &rec, vec3& attenuation, ray& scattered) const {
  vec3 target = rec.p + rec.normal + random_in_unit_sphere();
  scattered = ray(rec.p, target-rec.p);
  attenuation = albedo;
  return true;
}

vec3 reflect(const vec3& v, const vec3& n) {
  return v - 2.0f*dot(v, n)*n;
}

class metal : public material {
public:
  metal(const vec3& a);
  virtual bool scatter(const ray& r_in, const hit_record &rec, vec3& attenuation, ray& scattered) const;

  vec3 albedo;
};

metal::metal(const vec3& a) : albedo(a) {

}

bool metal::scatter(const ray& r_in, const hit_record &rec, vec3& attenuation, ray& scattered) const {
  vec3 reflected = reflect(normalize(r_in.direction()), rec.normal);
  scattered = ray(rec.p, reflected);
  attenuation = albedo;
  return dot(scattered.direction(), rec.normal) > 0;
}

vec3 random_in_unit_sphere() {
  vec3 p;
  do {
    p = 2.0f*vec3(drand48(), drand48(), drand48()) - vec3(1, 1, 1);
  } while (p.sqr_length() >= 1.0f);
  return p;
}

#endif // MATERIAL_HPP
