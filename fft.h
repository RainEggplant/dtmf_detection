#pragma once
#include <cmath>
#include <complex>
#include <iostream>
#include <vector>

#define M_PI 3.14159265358979323846

namespace fft {
using std::complex;
using std::vector;
// const double M_PI = 3.14159265358979323846;

// 计算 x 的 n 点 DFT
vector<complex<double>> dft(const complex<double> x[], const size_t len,
                            const size_t n);

// 使用基 2 DIT-FFT 计算 x 的 2^p 点 DFT
vector<complex<double>> fft_2_dit(const complex<double> x[], const size_t len,
                                  const int p);

// 使用基 2 DFT-FFT 计算 x 的 2^p 点 DFT
vector<complex<double>> fft_2_dft(const complex<double> x[], const size_t len,
                                  const int p);

// 求倒序编号
void get_reversed_index(size_t index[], const size_t n);
}  // namespace fft
