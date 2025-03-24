#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "ray.hpp"
#include "util.h"

class camera {
 public:
  camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect,
         float aperture, float focus_dist);
  ray get_ray(float u, float v) const;

  vec3 origin;
  vec3 lower_left_corner;
  vec3 horizontal;
  vec3 vertical;
  vec3 u, v, w;
  float lens_radius;
};

vec3 random_in_unit_disk() {
  vec3 p;
  do {
    p = 2.f * vec3(rand01(), rand01(), 0.f) - vec3(1.f, 1.f, 0.f);
  } while (dot(p, p) >= 1.f);
  return p;
}

camera::camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect,
               float aperture, float focus_dist) {
  lens_radius = aperture / 2.f;
  float theta = vfov * M_PI / 180.f;
  float half_height = tan(theta / 2);
  float half_width = aspect * half_height;
  origin = lookfrom;
  w = normalize(lookfrom - lookat);
  u = normalize(cross(vup, w));
  v = cross(w, u);
  // lower_left_corner = vec3(-half_width, -half_height, -1.0f);
  lower_left_corner = origin - half_width * focus_dist * u -
                      half_height * focus_dist * v - focus_dist * w;
  horizontal = 2.f * half_width * focus_dist * u;
  vertical = 2.f * half_height * focus_dist * v;
}

ray camera::get_ray(float s, float t) const {
  vec3 rd = lens_radius * random_in_unit_disk();
  vec3 offset = u * rd.x + v * rd.y;
  return ray(origin + offset,
             lower_left_corner + s * horizontal + t * vertical - offset);
}

#endif  // CAMERA_HPP
