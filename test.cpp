#include "binary_search.hpp"
#include "cumulative_sum.hpp"
#include "walker.hpp"
#include <iostream>
#include <random>
#include <vector>

template <class T>
void test() {
  std::vector<double> v = {4, 6, 4, 9, 2};
  std::vector<double> r(v.size(), 0.0);
  std::mt19937 mt;
  T s(v);
  const int N = 50000;
  double sum = std::accumulate(v.begin(), v.end(), 0);
  const double a = sum / N;
  for (int i = 0; i < N; i++) {
    int index = s.select(mt);
    r[index] += a;
  }
  for (size_t i = 0; i < v.size(); i++) {
    std::cout << i << " " << r[i] << std::endl;
  }
}

int main() {
  test<walker_alias>();
  test<cumulative_sum>();
  test<binary_search>();
}
