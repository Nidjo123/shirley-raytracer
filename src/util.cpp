#include "util.h"
#include <random>

std::random_device rd;
std::mt19937 generator(rd());

double rand01() {
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(generator);
}

