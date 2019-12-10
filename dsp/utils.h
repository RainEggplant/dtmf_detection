#pragma once
#include <algorithm>
#include <cmath>
#include <complex>
#include <iostream>
#include <utility>
#include <vector>

namespace dsp {
using std::complex;
using std::pair;
using std::vector;

// element-wise abs of a vector
vector<double> abs_vector(const vector<complex<double>> &X);

// create a vector<complex<double>> from two vectors<double> of real and imag
// part
vector<complex<double>> convert_to_complex(const vector<double> &x_real,
                                           const vector<double> &x_imag);

// 寻找一列 vector 中大于 threshold 的峰值
void find_peaks(const vector<double> &amp,
                vector<pair<size_t, size_t>> *wide_peaks,
                vector<size_t> *sharp_peaks, double threshold);
}  // namespace dsp
