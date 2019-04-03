#pragma once
#include <algorithm>
#include <random>
#include <vector>

struct cumulative_sum {
  std::vector<double> c_sum;
  std::uniform_real_distribution<double> ud;
  const std::string name() {
    return "Cumulative Sum";
  }

  cumulative_sum(std::vector<double> &list) {
    double sum = 0.0;
    for (auto v : list) {
      c_sum.push_back(sum);
      sum += v;
    }
    ud.param(std::uniform_real_distribution<double>::param_type(0, sum));
  }
  int search(double v) {
    int imin = 0;
    int imax = c_sum.size();
    int index = (imin + imax) / 2;
    while (imax != index && imin != index) {
      if (c_sum[index] < v) {
        imin = index;
      } else {
        imax = index;
      }
      index = (imin + imax) / 2;
    }
    return index;
  }
  int select(std::mt19937 &mt) {
    double v = ud(mt);
    return search(v);
  }
};