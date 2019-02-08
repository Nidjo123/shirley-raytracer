#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "ray.hpp"

class camera {
public:
  camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect);
  ray get_ray(float u, float v) const;

  vec3 origin;
  vec3 lower_left_corner;
  vec3 horizontal;
  vec3 vertical;
};

camera::camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect) {
    vec3 u, v, w;
  float theta = vfov * M_PI/180.f;
  float half_height = tan(theta/2);
  float half_width = aspect * half_height;
  origin = lookfrom;
  w = normalize(lookfrom-lookat);
  u = normalize(cross(vup, w));
  v = cross(w, u);
  // lower_left_corner = vec3(-half_width, -half_height, -1.0f);
  lower_left_corner = origin - half_width*u - half_height*v - w;
  horizontal = 2.f*half_width*u;
  vertical = 2.f*half_height*v;
}

ray camera::get_ray(float u, float v) const {
  return ray(origin,
	     lower_left_corner + u*horizontal + v*vertical - origin);
}

#endif // CAMERA_HPP
