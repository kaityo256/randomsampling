#pragma once

#include <cstdint>
#include <numeric>
#include <random>
#include <string>
#include <vector>

class walker_alias {
private:
  unsigned int n;
  unsigned int nb;
  std::vector<int> index;
  std::vector<double> prob;
  std::vector<uint32_t> probi;
  uint32_t int_max = ~static_cast<uint32_t>(0);

public:
  const std::string name() {
    return "Walker's Alias";
  }
  walker_alias(std::vector<double> &a) {
    nb = 1;
    while ((unsigned int)(1 << nb) <= a.size()) {
      nb++;
    }
    n = (1 << nb);
    index.resize(n, 0.0);
    prob.resize(n, 0.0);
    probi.resize(n, 0);
    for (unsigned int i = 0; i < a.size(); i++) {
      prob[i] = a[i];
    }
    double ave = std::accumulate(prob.begin(), prob.end(), 0.0) / prob.size();
    for (auto &v : prob) {
      v /= ave;
    }
    std::vector<int> small, large;
    for (unsigned int i = 0; i < n; i++) {
      if (prob[i] < 1.0) {
        small.push_back(i);
      } else {
        large.push_back(i);
      }
      index[i] = i;
    }
    while (small.size() && large.size()) {
      const int j = small.back();
      small.pop_back();
      const int k = large.back();
      index[j] = k;
      prob[k] = prob[k] - 1.0 + prob[j];
      if (prob[k] < 1.0) {
        small.push_back(k);
        large.pop_back();
      }
    }
    const int imax = int_max >> nb;
    for (unsigned int i = 0; i < n; i++) {
      probi[i] = static_cast<uint32_t>(prob[i] * imax);
    }
  }
  int select(std::mt19937 &mt) {
    uint32_t r = mt();
    int k = r & (n - 1);
    r = r >> nb;
    if (r > probi[k]) {
      return index[k];
    } else {
      return k;
    }
  }
};
