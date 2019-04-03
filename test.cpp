#include <iostream>
#include <random>
#include <vector>

struct CSSearcher {
  std::vector<double> c_sum;
  std::uniform_real_distribution<double> ud;
  CSSearcher(std::vector<double> &list) {
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
      //std::cout << imin << " " << index << " " << imax << std::endl;
    }
    return index;
  }
  int select(std::mt19937 &mt) {
    double v = ud(mt);
    return search(v);
  }
};

int main() {
  std::vector<double> v = {1, 2, 3, 4, 5};
  std::vector<double> r(v.size(), 0.0);
  std::mt19937 mt;
  CSSearcher cs(v);
  const int N = 50000;
  double sum = std::accumulate(v.begin(), v.end(), 0);
  const double a = sum / N;
  for (int i = 0; i < N; i++) {
    int index = cs.select(mt);
    r[index] += a;
  }
  for (size_t i = 0; i < v.size(); i++) {
    std::cout << i << " " << r[i] << std::endl;
  }
}
