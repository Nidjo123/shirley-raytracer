#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>

class Timer {
 public:
  Timer() : start{std::chrono::high_resolution_clock::now()} {}

  unsigned get_millis() const {
    const auto elapsed = std::chrono::high_resolution_clock::now() - start;
    return std::chrono::duration_cast<std::chrono::milliseconds>(elapsed)
        .count();
  }

  unsigned get_seconds() const {
    const auto elapsed = std::chrono::high_resolution_clock::now() - start;
    return std::chrono::duration_cast<std::chrono::seconds>(elapsed).count();
  }

 private:
  std::chrono::high_resolution_clock::time_point start;
};

#endif  // TIMER_HPP
