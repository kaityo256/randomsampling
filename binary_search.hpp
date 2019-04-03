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
      //std::cout << (n >> (j + 1)) << std::endl;
      for (int i = 0; i < (n >> (j + 1)); i++) {
        //std::cout << " " << (s + i * 2) << " " << (s + i * 2 + 1) << " " << (s + (n >> j)) << std::endl;
        tree[s + (n >> j) + i] = tree[s + i * 2] + tree[s + i * 2 + 1];
      }
      s += n >> j;
    }
    for (size_t i = 0; i < tree.size(); i++) {
      //std::cout << i << " " << tree[i] << std::endl;
    }
  }

  int select(std::mt19937 &mt) {
    int index = root_index;
    int base = root_index;
    for (int i = 0; index >= list_size; i++) {
      std::uniform_real_distribution<double> ud(0.0, tree[index]);
      //double v = ud(mt) * tree[index];
      double v = ud(mt);
      int local_index = index - base;
      base -= (2 << i);
      int left = base + local_index * 2;
      int right = left + 1;
      //std::cout << index << " " << left << " " << right << std::endl;
      if (v < tree[left]) {
        index = left;
      } else {
        index = right;
      }
    }
    return index;
  }
};