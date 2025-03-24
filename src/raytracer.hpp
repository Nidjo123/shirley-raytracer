#ifndef RAYTRACER_HPP
#define RAYTRACER_HPP

#include <cfloat>
#include <cstdlib>
#include <iostream>
#include "camera.hpp"
#include "hitable.hpp"
#include "hitable_list.hpp"
#include "ray.hpp"
#include "sphere.hpp"
#include "vec3.hpp"
#include "util.h"

vec3 color(const ray &r, hitable *world, int depth) {
  hit_record rec;
  if (world->hit(r, 0.001f, MAXFLOAT, rec)) {
    ray scattered;
    vec3 attenuation;
    if (depth < 50 && rec.mat->scatter(r, rec, attenuation, scattered))
      return attenuation * color(scattered, world, depth + 1);
    else
      return vec3(0, 0, 0);
  }

  vec3 dir = normalize(r.direction());
  float t = 0.5f * (dir.y + 1.0f);
  return (1.0f - t) * vec3(1.0f, 1.0f, 1.0f) + t * vec3(0.5f, 0.7f, 1.0f);
}

hitable *random_scene() {
  const int n = 500;
  hitable **list = new hitable *[n + 1];
  list[0] = new sphere(vec3(0, -1000, 0), 1000,
                       new lambertian(vec3(0.5f, 0.5f, 0.5f)));
  int i = 1;
  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      float choose_mat = rand01();
      vec3 center(a + 0.9f * rand01(), 0.2f, b + 0.9f * rand01());
      if ((center - vec3(4.f, 0.2f, 0.f)).length() > 0.9) {
        if (choose_mat < 0.7) {  // diffuse
          list[i++] = new sphere(
              center, 0.2f,
              new lambertian(vec3(rand01() * rand01(), rand01() * rand01(),
                                  rand01() * rand01())));
        } else if (choose_mat < 0.9) {  // metal
          list[i++] = new sphere(
              center, 0.2f,
              new metal(vec3(0.5f * (1.f + rand01()), 0.5f * (1.f + rand01()),
                             0.5f * (1.f + rand01())),
                        rand01() * rand01() * rand01()));
        } else {  // dielectric
          list[i++] =
              new sphere(center, 0.2f, new dielectric(1.f + 3.f * rand01()));
        }
      }
    }
  }

  list[i++] = new sphere(vec3(0.f, 1.f, 0.f), 1.f, new dielectric(1.5f));
  list[i++] = new sphere(vec3(-4.f, 1.f, 0.f), 1.f,
                         new lambertian(vec3(0.4f, 0.2f, 0.1f)));
  list[i++] = new sphere(vec3(4.f, 1.f, 0.f), 1.f,
                         new metal(vec3(0.7f, 0.6f, 0.5f), 0.f));

  return new hitable_list(list, i);
}

#endif  // RAYTRACER_HPP
