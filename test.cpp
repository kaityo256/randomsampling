#include "binary_tree.hpp"
#include "cumulative_sum.hpp"
#include "walker_alias.hpp"
#include <iostream>
#include <random>
#include <vector>

template <class T>
void test(std::vector<double> &v) {
  std::vector<double> r(v.size(), 0.0);
  std::mt19937 mt;
  T s(v);
  const int N = 100000;
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
  std::vector<double> v = {4, 6, 4, 9, 2};
  std::cout << "Input" << std::endl;
  for (size_t i = 0; i < v.size(); i++) {
    std::cout << i << " " << v[i] << std::endl;
  }
  std::cout << "Walker's Alias" << std::endl;
  test<walker_alias>(v);
  std::cout << "Cumulative Sum" << std::endl;
  test<cumulative_sum>(v);
  std::cout << "Binary Tree" << std::endl;
  test<binary_tree>(v);
}
