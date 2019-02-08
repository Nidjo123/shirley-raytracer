#include <iostream>
#include <cstdlib>
#include <cfloat>
#include "vec3.hpp"
#include "ray.hpp"
#include "sphere.hpp"
#include "hitable.hpp"
#include "hitable_list.hpp"
#include "camera.hpp"

vec3 color(const ray& r, hitable *world, int depth) {
  hit_record rec;
  if (world->hit(r, 0.001f, MAXFLOAT, rec)) {
    ray scattered;
    vec3 attenuation;
    if (depth < 50 && rec.mat->scatter(r, rec, attenuation, scattered))
      return attenuation * color(scattered, world, depth+1);
    else
      return vec3(0, 0, 0);
  }

  vec3 dir = normalize(r.direction());
  float t = 0.5f*(dir.y+1.0f);
  return (1.0f-t)*vec3(1.0f, 1.0f, 1.0f) + t*vec3(0.5f, 0.7f, 1.0f);
}

hitable *random_scene() {
  const int n = 500;
  hitable **list = new hitable*[n+1];
  list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5f, 0.5f, 0.5f)));
  int i = 1;
  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      float choose_mat = drand48();
      vec3 center(a+0.9f*drand48(), 0.2f, b+0.9f*drand48());
      if ((center-vec3(4.f, 0.2f, 0.f)).length() > 0.9) {
	if (choose_mat < 0.7) { // diffuse
	  list[i++] = new sphere(center, 0.2f, new lambertian(vec3(drand48()*drand48(), drand48()*drand48(), drand48()*drand48())));
	} else if (choose_mat < 0.9) { // metal
	  list[i++] = new sphere(center, 0.2f, new metal(vec3(0.5f*(1.f+drand48()), 0.5f*(1.f+drand48()), 0.5f*(1.f+drand48())), drand48()*drand48()*drand48()));
	} else { // dielectric
	  list[i++] = new sphere(center, 0.2f, new dielectric(1.f+3.f*drand48()));
	}
      }
    }
  }

  list[i++] = new sphere(vec3(0.f, 1.f, 0.f), 1.f, new dielectric(1.5f));
  list[i++] = new sphere(vec3(-4.f, 1.f, 0.f), 1.f, new lambertian(vec3(0.4f, 0.2f, 0.1f)));
  list[i++] = new sphere(vec3(4.f, 1.f, 0.f), 1.f, new metal(vec3(0.7f, 0.6f, 0.5f), 0.f));

  return new hitable_list(list, i);
}

int main() {
  int nx = 1920;
  int ny = 1080;
  int ns = 256;

  std::cout << "P3\n" << nx << " " << ny << "\n255\n";

  hitable *list[5];
  list[0] = new sphere(vec3(0.0f, 0.0f, -1.0f), 0.5f, new lambertian(vec3(0.8f, 0.3f, 0.3f)));
  list[1] = new sphere(vec3(0.0f, -100.5f, -1.0f), 100.0f, new lambertian(vec3(0.1f, 0.8f, 0.0f)));
  list[2] = new sphere(vec3(1.0f, 0.0f, -1.0f), 0.5f, new metal(vec3(0.8f, 0.6f, 0.2f), 0.3f));
  list[3] = new sphere(vec3(-1.0f, 0.0f, -1.0f), 0.55f, new dielectric(1.5f));
  list[4] = new sphere(vec3(-1.0f, 0.0f, -1.0f), 0.5f, new metal(vec3(0.2f, 0.5f, 0.9f), 0.05f));

  hitable *world =  random_scene();//new hitable_list(list, 5);

  vec3 lookfrom = vec3(5, 1.f, 5.5f);
  vec3 lookat = vec3(1, 0.5f, 0);
  vec3 vup = vec3(0, 1, 0);
  float dist_to_focus = (lookfrom-lookat).length()*2.f;
  float aperture = 0.08f;
  camera cam(lookfrom, lookat, vup, 20.f, float(nx)/float(ny), aperture, dist_to_focus);

  for (int j = ny-1; j >= 0; j--)
    for (int i = 0; i < nx; i++) {
      vec3 col(0, 0, 0);
      for (int s = 0; s < ns; s++) {
	float u = float(i + drand48()) / float(nx);
	float v = float(j + drand48()) / float(ny);
	ray r = cam.get_ray(u, v);
	col += color(r, world, 0);
      }
      col /= float(ns);
      col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
      int ir = int(255.99*col[0]);
      int ig = int(255.99*col[1]);
      int ib = int(255.99*col[2]);
      std::cout << ir << " " << ig << " " << ib << "\n";
    }

  return 0;
}
