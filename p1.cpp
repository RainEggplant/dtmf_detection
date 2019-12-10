#include <complex>
#include <iostream>
#include <utility>
#include <vector>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-compare"
#include "AudioFile.h"
#pragma GCC diagnostic pop
#include "dsp/fft.h"
#include "dsp/utils.h"

using dsp::abs_vector;
using dsp::fft_2_dit;
using dsp::find_peaks;
using std::complex;
using std::cout;
using std::endl;
using std::pair;
using std::vector;

int main() {
  AudioFile<double> audioFile;
  audioFile.load("./data1/data1081.wav");
  audioFile.printSummary();
  auto fs = audioFile.getSampleRate();

  // 计算音频的 DFT
  auto x = audioFile.samples[0];
  auto x_comp = vector<complex<double>>(x.begin(), x.end());
  size_t n_sample = x.size();
  int p = ceil(log2(n_sample));
  size_t n_fft = 1 << p;
  auto X = fft_2_dit(x_comp, p);

  // 只取 0 - pi 频域
  auto X_half = vector<complex<double>>(X.begin(), X.begin() + n_fft / 2);

  // 寻找峰值
  auto X_abs = abs_vector(X_half);
  double X_max = (*std::max_element(X_abs.begin(), X_abs.end()));
  vector<pair<size_t, size_t>> wide_peaks;
  vector<size_t> sharp_peaks;
  find_peaks(X_abs, &wide_peaks, &sharp_peaks, X_max * 0.8);

  // 转换成频率
  size_t n_peaks = wide_peaks.size() + sharp_peaks.size();
  vector<double> peak_freq;
  for (auto it = wide_peaks.begin(); it != wide_peaks.end(); ++it) {
    double freq = ((*it).first + (*it).second) / 2.0 / n_fft * fs;
    peak_freq.push_back(freq);
  }

  for (auto it = sharp_peaks.begin(); it != sharp_peaks.end(); ++it) {
    double freq = (double)(*it) / n_fft * fs;
    peak_freq.push_back(freq);
  }

  cout << "peak frequencies:" << endl;
  for (size_t i = 0; i < n_peaks; ++i) {
    cout << peak_freq[i] << endl;
  }

  getchar();
}
