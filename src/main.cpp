#include <iostream>
#include <cfloat>
#include "vec3.hpp"
#include "ray.hpp"
#include "sphere.hpp"
#include "hitable.hpp"
#include "hitable_list.hpp"


vec3 color(const ray& r, hitable *world) {
  hit_record rec;
  if (world->hit(r, 0.0f, MAXFLOAT, rec))
    return 0.5f*(rec.normal+vec3(1.0f, 1.0f, 1.0f));

  vec3 dir = normalize(r.direction());
  float t = 0.5f*(dir.y+1.0f);
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

  hitable *list[2];
  list[0] = new sphere(vec3(0.0f, 0.0f, -1.0f), 0.5f);
  list[1] = new sphere(vec3(0.0f, -100.5f, -1.0f), 100.0f);
  hitable *world = new hitable_list(list, 2);

  for (int j = ny-1; j >= 0; j--)
    for (int i = 0; i < nx; i++) {
      float u = float(i) / float(nx);
      float v = float(j) / float(ny);
      ray r(origin, lower_left_corner + u*horizontal + v*vertical);
      vec3 col = color(r, world);
      int ir = int(255.99*col[0]);
      int ig = int(255.99*col[1]);
      int ib = int(255.99*col[2]);
      std::cout << ir << " " << ig << " " << ib << "\n";
    }

  // be pedantic
  delete list[0];
  delete list[1];

  return 0;
}
