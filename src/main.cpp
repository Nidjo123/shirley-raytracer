#include <iostream>
#include "vec3.hpp"
#include "ray.hpp"

float hit_sphere(const vec3& center, float radius, const ray& r) {
  const float a = dot(r.direction(), r.direction());
  const float b = 2.0f*dot(r.direction(), r.origin()-center);
  const float c = dot(r.origin()-center, r.origin()-center) - radius*radius;
  const float discriminant = b*b - 4.0f*a*c;

  if (discriminant < 0.0f)
    return -1.0f;
  return (-b - sqrt(discriminant)) / (2.0f*a);
}

vec3 color(const ray& r) {
  vec3 dir = normalize(r.direction());
  float t = 0.5f*(dir.y + 1.0f);
  vec3 center = vec3(0.0f, 0.0f, -2.0f);
  float hit = hit_sphere(center, 1.0f, r);
  if (hit >= 0.0f)
    return 0.5f*(normalize(r(hit)-center)+vec3(1.0f, 1.0f, 1.0f));
  return (1.0f-t)*vec3(1.0f, 1.0f, 1.0f) + t*vec3(0.5f, 0.7f, 1.0f);
}

int main() {
  int nx = 200;
  int ny = 100;

  std::cout << "P3\n" << nx << " " << ny << "\n255\n";

  vec3 lower_left_corner(-2.0f, -1.0f, -1.0f);
  vec3 horizontal(4.0f, 0.0f, 0.0f);
  vec3 vertical(0.0f, 2.0f, 0.0f);
  vec3 origin(0.0f, 0.0f, 0.0f);

  for (int j = ny-1; j >= 0; j--)
    for (int i = 0; i < nx; i++) {
      float u = float(i) / float(nx);
      float v = float(j) / float(ny);
      ray r(origin, lower_left_corner + u*horizontal + v*vertical);
      vec3 col = color(r);
      int ir = int(255.99*col[0]);
      int ig = int(255.99*col[1]);
      int ib = int(255.99*col[2]);
      std::cout << ir << " " << ig << " " << ib << "\n";
    }

  return 0;
}
