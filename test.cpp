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
  std::cout << s.name() << std::endl;
  const int N = 100000;
  double sum = std::accumulate(v.begin(), v.end(), 0);
  const double a = sum / N;
  for (int i = 0; i < N; i++) {
    int index = s.select(mt);
    r[index] += a;
  }
  for (size_t i = 0; i < v.size(); i++) {
    std::cout << i << ":" << r[i] << std::endl;
  }
  std::cout << std::endl;
}

template <class T>
void test_with_update(std::vector<double> &v, int ui, double uvalue) {
  std::vector<double> r(v.size(), 0.0);
  std::mt19937 mt;
  T s(v);
  std::cout << s.name() << std::endl;

  v[ui] = uvalue;
  s.update(ui, uvalue);
  const int N = 100000;
  double sum = std::accumulate(v.begin(), v.end(), 0);
  const double a = sum / N;
  for (int i = 0; i < N; i++) {
    int index = s.select(mt);
    r[index] += a;
  }
  for (size_t i = 0; i < v.size(); i++) {
    std::cout << i << ":" << r[i] << std::endl;
  }
}

void testall(std::vector<double> &v) {
  std::cout << "Input Data" << std::endl;
  for (size_t i = 0; i < v.size(); i++) {
    std::cout << i << ":" << v[i] << std::endl;
  }
  std::cout << std::endl;
  test<walker_alias>(v);
  test<cumulative_sum>(v);
  test<binary_tree>(v);

  int uindex = 2;
  double uvalue = 6.0;
  std::cout << "Partial Update" << std::endl;
  std::cout << uindex << ":" << v[uindex] << "->" << uvalue << std::endl;
  std::cout << std::endl;
  test_with_update<binary_tree>(v, uindex, uvalue);
}

int main() {
  std::vector<double> v = {1, 2, 3, 4, 5};
  testall(v);
}
