#include <iostream>
#include <cstdlib>
#include <cfloat>
#include "vec3.hpp"
#include "ray.hpp"
#include "sphere.hpp"
#include "hitable.hpp"
#include "hitable_list.hpp"
#include "camera.hpp"


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
  int ns = 100;

  std::cout << "P3\n" << nx << " " << ny << "\n255\n";

  hitable *list[2];
  list[0] = new sphere(vec3(0.0f, 0.0f, -1.0f), 0.5f);
  list[1] = new sphere(vec3(0.0f, -100.5f, -1.0f), 100.0f);
  hitable *world = new hitable_list(list, 2);

  camera cam;

  for (int j = ny-1; j >= 0; j--)
    for (int i = 0; i < nx; i++) {
      vec3 col(0, 0, 0);
      for (int s = 0; s < ns; s++) {
	float u = float(i + drand48()) / float(nx);
	float v = float(j + drand48()) / float(ny);
	ray r = cam.get_ray(u, v);
	col += color(r, world);
      }
      col /= float(ns);
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
