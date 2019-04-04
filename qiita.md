
# 重み付きランダムサンプリングアルゴリズム

## はじめに

あらかじめ指定された重みに従って離散的な値を確率的に選択したい、ということがよくある。例えば`[1,4,5]`という配列が与えられたら、確率10%で0、40%で1、50%で2というインデックスを返すようにランダムサンプリングしたい。一番手軽な累積和の方法では、テーブル構築に$O(N)$、サンプリングに$O(\log(N))$の計算量となり、多くの場合これで用は足りる。さらに、テーブル構築は$O(N)$だが、サンプリングが$O(1)$となる[Walker's Alias法](https://qiita.com/kaityo256/items/1656597198cbfeb7328c)という強力アルゴリズムもある。

累積和の方法は組むのが簡単で探索が$O(\log(N)$と十分早いし、Walker's Alias法は探索が$O(1)$と強烈なので、「重み付きランダムサンプリングアルゴリズムは、もうこの２つのどちらかで十分じゃん？」という気持ちになるが、この２つのアルゴリズムでは、重みの部分書き換えができない、という弱点がある。つまり、どこか一つの配列の重みが変更された場合、テーブル構築にまた$O(N)$の手間がかかってしまう。

そこで本稿では二分木を用いたサンプリングアルゴリズムを紹介したい。これはテーブル構築に$O(N)$、探索が$O(\log(N)$だが、一度テーブルが構築されると、一つの重みを変更してもテーブルの更新が$O(\log(N))$でできる。

とりあえず本稿では、

* 累積和
* Walker's Alias
* 二分木探索

の３つの方法を紹介してみたい。

## 累積和の方法

まず、最も単純な累積和の方法を組んでみよう。以下、入力する重みの配列として`[1,2,3,4,5]`を考える。ここから1/15の確率でインデックス0を、2/15の確率でインデックス1を返す関数を作るのが重み付きサンプリングの目的である。

累積和の方法の原理は単純で、重みに比例した長さのバーを積み重ね、そこにダーツを投げれば、面積(つまり重み)に比例した確率で対応するインデックスに当たるでしょう、という思想である。

実装としては、まず事前準備として以下の図のように累積和を作っておく。これは$O(N)$の手間で構築できる。

そして、重みの総和(今回は15)に対する一様乱数を振る。例えば10.0という数字が得られたとしよう。これは、6と14の間なので、インデックスとしては3を返せば良い。この「得られた乱数がどことどこの間に挟まっているか」を二分探索で調べるので$O(\log(N)$の手間がかかる。

コンストラクタで重み配列を受け取り、$select$でインデックスを返すようなクラスはこんな感じに実装できるだろう。

```cpp
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
```

特に難しいことはないと思う。

もし、重み配列の一部が書き換わったら、それ以降の累積和を作り直しになるので、この手間は$O(N)$である。

## Walker's Alias

Walker's Alias法は、$N$個の要素に対して$N$個の箱を作り、その箱をたかだか２つに分解して色を塗り、それぞれの色の面積の合計が元の重みにすることで、$O(1)$の探索を可能とする方法である。

先程の例なら、以下のような5つの箱を作る。

それぞれの箱が、一種類か二種類の色で塗られており、それぞれの色の面積の合計が元の重みに対応していることがわかるであろう。この箱ができたら、

* まず$N$個の箱から一様に一つ選ぶ
* 選ばれた箱に２つの色があったら、どちらを選ぶか確率的に決める

の2ステップでインデックスを選べる。どんなに$N$が大きくても2ステップなので、$O(1)$の方法となっていることがわかるであろう。この箱の作り方がやや難しいのだが、詳細は[別記事](https://qiita.com/kaityo256/items/1656597198cbfeb7328c)を参照して欲しい。

箱を作るところから探索までを実装すると、例えばこんな感じになるだろう。

```cpp
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
```

詳しいアルゴリズムを知らないと、何をやっているか理解するのは難しいと思う。ここでは二回乱数を振るかわりに、一つの乱数の上位ビットと下位ビットを「箱の選択」「箱内の選択」に分けて使うことで、乱数生成回数を一度にしている。

この方法は箱の準備が$O(N)$、探索が$O(1)$だが、一部でも重みが変更されたら箱は全て作り直しなので、更新も$O(N)$である。

