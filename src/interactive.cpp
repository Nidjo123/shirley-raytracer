#include <iostream>
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <cfloat>
#include <thread>
#include <mutex>
#include <vector>

#include <SFML/Graphics.hpp>

#include "vec3.hpp"
#include "ray.hpp"
#include "sphere.hpp"
#include "hitable.hpp"
#include "hitable_list.hpp"
#include "camera.hpp"

constexpr unsigned WIDTH = 512;
constexpr unsigned HEIGHT = 512;

constexpr unsigned N = 16u;
constexpr unsigned N_SAMPLES = 64;

constexpr unsigned W_CNT = (WIDTH+N-1)/N;
constexpr unsigned H_CNT = (HEIGHT+N-1)/N;

hitable *world;

vec3 lookfrom = vec3(5, 1.f, 5.5f);
vec3 lookat = vec3(1, 0.5f, 0);
vec3 vup = vec3(0, 1, 0);
float dist_to_focus = (lookfrom-lookat).length()*2.f;
float aperture = 0.08f;
camera cam = camera(lookfrom, lookat, vup, 20.f, float(WIDTH)/float(HEIGHT), aperture, dist_to_focus);

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

struct Pixels {
  Pixels(unsigned w, unsigned h) : width(w), height(h) {
    data = new float[width * height * 5]; // RGBA + sample counter
    pixels = new sf::Uint8[width * height * 4];

    std::memset(data, 0, width*height*5*sizeof(float));
    std::memset(pixels, 0, width*height*4*sizeof(sf::Uint8));
  }

  ~Pixels() {
    delete [] data;
    delete [] pixels;
  }

  void calculate_pixels() {
    for (int i = 0; i < HEIGHT; i++)
      for (int j = 0; j < WIDTH; j++) {
	const unsigned pos = (i * width + j) * 5;
	const float ns = data[pos + 4];
	const unsigned pos2 = ((HEIGHT - i - 1) * width + j) << 2;
	pixels[pos2 + 0] = sf::Uint8(255.99f*(sqrtf(data[pos + 0]/ns)));
	pixels[pos2 + 1] = sf::Uint8(255.99f*(sqrtf(data[pos + 1]/ns)));
	pixels[pos2 + 2] = sf::Uint8(255.99f*(sqrtf(data[pos + 2]/ns)));
	pixels[pos2 + 3] = 255u;
      }
  }

  inline void accumulate(unsigned x, unsigned y, float r, float g, float b) {
    const unsigned pos = (y * width + x) * 5;
    data[pos + 0] += r;
    data[pos + 1] += g;
    data[pos + 2] += b;
    data[pos + 3] += 255.f; // opaque
    data[pos + 4] += 1.f;
  }

  inline void accumulate(unsigned x, unsigned y, const vec3 &col) {
    accumulate(x, y, col.x, col.y, col.z);
  }

  float *data; // RGBA
  sf::Uint8 *pixels;
  unsigned width;
  unsigned height;
} pixels{WIDTH, HEIGHT};

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

struct Task;
bool get_next_task(Task &task);

struct Task {
  Task() = default;

  Task(int x, int y) : sx{x}, sy{y} {
  }

  void operator()() {
    do {
      if (!get_next_task(*this))
	return;

      for (unsigned s = 0; s < N_SAMPLES; s++)
	for (unsigned i = sy; i < sy+N; i++)
	  for (unsigned j = sx; j < sx+N; j++) {
	    if (i >= HEIGHT || j >= WIDTH)
	      continue;

	    const float u = float(j + drand48()) / float(WIDTH);
	    const float v = float(i + drand48()) / float(HEIGHT);
	    ray r = cam.get_ray(u, v);
	    const vec3 col = color(r, world, 0);

	    pixels.accumulate(j, i, col);
	  }
    } while (true);
  }

  int sx=-1, sy=-1;
};

bool get_next_task(Task &task) {
  static bool taken[H_CNT][W_CNT] = {};
  static std::mutex m;

  std::lock_guard<std::mutex> guard{m};

  for (int i = 0; i < H_CNT; i++)
    for (int j = 0; j < W_CNT; j++)
      if (!taken[i][j]) {
	task.sx = j * N;
	task.sy = i * N;
	taken[i][j] = true;
	return true;
      }

  return false;
}

int main() {
  sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Ray Tracing rules!");
  sf::Texture tex;
  sf::Sprite sprite;

  if (!tex.create(WIDTH, HEIGHT)) {
    std::cerr << "Couldn't create texture!" << std::endl;
    return 1;
  }

  tex.setSmooth(false);

  sprite.setTexture(tex);

  world = random_scene();

  const unsigned int n_threads = std::thread::hardware_concurrency();
  std::cout << "Detected " << n_threads << " supported threads." << std::endl;
  std::vector<std::thread> threads(n_threads);

  for (auto &t : threads)
    t = std::thread{Task{}};

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
	window.close();
    }

    pixels.calculate_pixels();
    tex.update(pixels.pixels);

    window.clear();
    window.draw(sprite);
    window.display();

    sf::sleep(sf::milliseconds(10));
  }

  for (auto &t : threads)
    t.join();

  delete world;

  return 0;
}
