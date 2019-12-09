#include "fft.h"

namespace fft {
vector<complex<double>> dft(const complex<double> x[], const size_t len,
                            const size_t n) {
  complex<double> W;
  W = exp(complex<double>(0, -2 * M_PI / n));
  vector<complex<double>> X(n);

  for (size_t i = 0; i < n; ++i) {
    for (size_t j = 0; j < n; ++j) {
      X[i] += x[j] * pow(W, i * j);
    }
  }

  return X;
}

vector<complex<double>> fft_2_dit(const complex<double> x[], size_t len,
                                  const int p) {
  size_t n = 1 << p;

  // 求倒序编号
  size_t index[n];  // valid since C98
  index[0] = 0;
  get_reversed_index(index, n);

  // 将输入序列长度取为 n 并排序
  vector<complex<double>> xs(n);

  for (size_t i = 0; i < n; ++i) {
    if (index[i] < len) {
      xs[index[i]] = x[i];
    }
  }

  // 旋转因子
  complex<double> w0;
  w0 = exp(complex<double>(0, -2 * M_PI / n));

  // 执行变换
  size_t step = 1;
  for (int i = 0; i < p; ++i) {                   // p 级
    for (int j = 0; j < 1 << (p - 1 - i); ++j) {  // 2^(p-1-i) 个序号组
      for (int k = 0; k < 1 << i; ++k) {  // 每组有 2^i 个连续序号
        size_t id1 = (1 << (i + 1)) * j + k;
        size_t id2 = id1 + step;
        complex<double> x1 = xs[id1];
        complex<double> x2 = xs[id2];
        complex<double> w = pow(w0, k * (1 << (p - 1 - i)));
        xs[id1] = x1 + w * x2;
        xs[id2] = x1 - w * x2;
      }
    }
    step <<= 1;
  }

  return xs;
}

vector<complex<double>> fft_2_dft(const complex<double> x[], const size_t len,
                                  const int p) {
  size_t n = 1 << p;

  // 将输入序列长度设为 n
  vector<complex<double>> xs(x, x + len);
  xs.resize(n);

  // 旋转因子
  complex<double> w0;
  w0 = exp(complex<double>(0, -2 * M_PI / n));

  int step = 1 << (p - 1);
  for (int i = 0; i < p; ++i) {         // p 级
    for (int j = 0; j < 1 << i; ++j) {  // 2^i 个序号组
      for (int k = 0; k < 1 << (p - 1 - i); ++k) {
        // 每组有 2^(p-1-i) 个连续序号
        int id1 = (1 << (p - i)) * j + k;
        int id2 = id1 + step;
        complex<double> x1 = xs[id1];
        complex<double> x2 = xs[id2];
        complex<double> w = pow(w0, k * (1 << i));
        xs[id1] = x1 + x2;
        xs[id2] = (x1 - x2) * w;
      }
    }
    step >>= 1;
  }

  // 求倒序编号
  size_t index[n];  // valid since C98
  index[0] = 0;
  get_reversed_index(index, n);

  // 排序得到结果
  vector<complex<double>> result(n);
  for (size_t i = 0; i < n; ++i) {
    result[index[i]] = xs[i];
  }

  return result;
}

void get_reversed_index(size_t index[], const size_t n) {
  index[0] = 0;

  for (size_t i = 1; i < n; ++i) {
    size_t k = n / 2;
    index[i] = index[i - 1];
    while (k > 0) {
      if (index[i] < k) {  // 如果最高位为 0
        index[i] += k;     // 下一个倒码序号就是将最高位变成 1
        break;
      } else {
        index[i] -= k;  // 否则先将最高位变成 0 （进位）
        k /= 2;         // 然后 k /= 2 以比较次高位
      }
    }
  }
}
}  // namespace fft