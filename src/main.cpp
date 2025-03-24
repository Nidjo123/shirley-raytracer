#include <chrono>

#include "raytracer.hpp"
#include "timer.hpp"
#include "util.h"

int main() {
  int nx = 1920;
  int ny = 1080;
  int ns = 256;

  std::cout << "P3\n" << nx << " " << ny << "\n255\n";

  hitable *list[5];
  list[0] = new sphere(vec3(0.0f, 0.0f, -1.0f), 0.5f,
                       new lambertian(vec3(0.8f, 0.3f, 0.3f)));
  list[1] = new sphere(vec3(0.0f, -100.5f, -1.0f), 100.0f,
                       new lambertian(vec3(0.1f, 0.8f, 0.0f)));
  list[2] = new sphere(vec3(1.0f, 0.0f, -1.0f), 0.5f,
                       new metal(vec3(0.8f, 0.6f, 0.2f), 0.3f));
  list[3] = new sphere(vec3(-1.0f, 0.0f, -1.0f), 0.55f, new dielectric(1.5f));
  list[4] = new sphere(vec3(-1.0f, 0.0f, -1.0f), 0.5f,
                       new metal(vec3(0.2f, 0.5f, 0.9f), 0.05f));

  hitable *world = random_scene();  // new hitable_list(list, 5);

  vec3 lookfrom = vec3(5, 1.f, 5.5f);
  vec3 lookat = vec3(1, 0.5f, 0);
  vec3 vup = vec3(0, 1, 0);
  float dist_to_focus = (lookfrom - lookat).length() * 2.f;
  float aperture = 0.08f;
  camera cam(lookfrom, lookat, vup, 20.f, float(nx) / float(ny), aperture,
             dist_to_focus);

  Timer timer;

  for (int j = ny - 1; j >= 0; j--)
    for (int i = 0; i < nx; i++) {
      vec3 col(0, 0, 0);
      for (int s = 0; s < ns; s++) {
        float u = float(i + rand01()) / float(nx);
        float v = float(j + rand01()) / float(ny);
        ray r = cam.get_ray(u, v);
        col += color(r, world, 0);
      }
      col /= float(ns);
      col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
      int ir = int(255.99 * col[0]);
      int ig = int(255.99 * col[1]);
      int ib = int(255.99 * col[2]);
      std::cout << ir << " " << ig << " " << ib << "\n";
    }

  return 0;
}
