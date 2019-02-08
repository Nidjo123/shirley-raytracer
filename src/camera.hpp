#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "ray.hpp"

class camera {
public:
  camera();
  ray get_ray(float u, float v) const;

  vec3 origin;
  vec3 lower_left_corner;
  vec3 horizontal;
  vec3 vertical;
};

camera::camera() {
  lower_left_corner = vec3(-2.0f, -1.0f, -1.0f);
  horizontal = vec3(4.0f, 0.0f, 0.0f);
  vertical = vec3(0.0f, 2.0f, 0.0f);
  origin = vec3(0.0f, 0.0f, 0.0f);
}

ray camera::get_ray(float u, float v) const {
  return ray(origin,
	     lower_left_corner + u*horizontal + v*vertical - origin);
}

#endif // CAMERA_HPP
