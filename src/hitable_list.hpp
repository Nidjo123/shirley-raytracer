#ifndef HITABLE_LIST_HPP
#define HITABLE_LIST_HPP

#include "hitable.hpp"

class hitable_list : public hitable {
public:
  hitable_list() = default;
  hitable_list(hitable **l, int n);
  virtual bool hit(const ray& r, float t_min, float t_max, hit_record& hitrec) const;

  hitable **list;
  int size;
};

hitable_list::hitable_list(hitable **l, int n)
  : list(l), size(n) {

}

bool hitable_list::hit(const ray& r, float t_min, float t_max, hit_record& hitrec) const {
  hit_record temp_rec;
  bool hit_anything = false;
  double closest_so_far = t_max;

  for (int i = 0; i < size; i++) {
    if (list[i]->hit(r, t_min, closest_so_far, temp_rec)) {
      hit_anything = true;
      closest_so_far = temp_rec.t;
      hitrec = temp_rec;
    }
  }

  return hit_anything;
}

#endif // HITABLE_LIST_HPP
