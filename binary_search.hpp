#pragma once
#include <algorithm>
#include <random>
#include <vector>

struct binary_search {
  std::vector<double> tree;
  int root_index;
  size_t list_size;

  int twopow(int n) {
    if (n == 0) return 0;
    if ((n & (n - 1)) == 0) return n;
    return 1 << (32 - __builtin_clz(n));
  }

  binary_search(std::vector<double> &list) {
    int n = twopow(list.size());
    list_size = n;
    root_index = 2 * n - 2;
    tree.resize(2 * n - 1, 0.0);
    std::copy(list.begin(), list.end(), tree.begin());
    int s = 0;
    for (int j = 0; (1 << j) < n; j++) {
      for (int i = 0; i < (n >> (j + 1)); i++) {
        tree[s + (n >> j) + i] = tree[s + i * 2] + tree[s + i * 2 + 1];
      }
      s += n >> j;
    }
  }

  int select(std::mt19937 &mt) {
    std::uniform_real_distribution<double> ud(0.0, 1.0);
    int index = root_index;
    int base = root_index;
    for (int i = 0; index >= list_size; i++) {
      double v = ud(mt)*tree[index];
      int local_index = index - base;
      base -= (2 << i);
      int left = base + local_index * 2;
      int right = left + 1;
      if (v < tree[left]) {
        index = left;
      } else {
        index = right;
      }
    }
    return index;
  }
};