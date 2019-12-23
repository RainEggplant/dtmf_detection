#include <complex>
#include <iostream>
#include <string>
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
using std::cin;
using std::complex;
using std::cout;
using std::endl;
using std::getline;
using std::pair;
using std::string;
using std::vector;

char get_key_by_freq(double freq1, double freq2);
char get_key_from_file(string filename);

int main() {
  string filename;
  cout << "Input filename (empty line to exit): ";
  getline(cin, filename);
  while (!filename.empty()) {
    char key = get_key_from_file(filename);
    cout << "Key: " << key << endl;
    cout << "Input filename (empty line to exit): ";
    getline(cin, filename);
  }
}

char get_key_from_file(string filename) {
  AudioFile<double> audioFile;
  audioFile.load(filename);
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
  find_peaks(X_abs, &wide_peaks, &sharp_peaks, X_max * 0.6);

  // 若识别到少于 2 个峰，则失败
  size_t n_peaks = wide_peaks.size() + sharp_peaks.size();
  if (n_peaks < 2) {
    return 0;
  }

  // 转换成频率
  vector<double> peak_freq;
  for (auto it = wide_peaks.begin(); it != wide_peaks.end(); ++it) {
    double freq = ((*it).first + (*it).second) / 2.0 / n_fft * fs;
    peak_freq.push_back(freq);
  }

  for (auto it = sharp_peaks.begin(); it != sharp_peaks.end(); ++it) {
    double freq = (double)(*it) / n_fft * fs;
    peak_freq.push_back(freq);
  }

  // 按照峰值幅度降序排列
  std::sort(peak_freq.begin(), peak_freq.end(),
            [&X_abs, fs, n_fft](double freq1, double freq2) {
              double amp1 = X_abs[(int)(freq1 / fs * n_fft)];
              double amp2 = X_abs[(int)(freq2 / fs * n_fft)];
              return amp1 > amp2;
            });

  return get_key_by_freq(peak_freq[0], peak_freq[1]);
}

char get_key_by_freq(double freq1, double freq2) {
  static char table[4][4] = {{'1', '2', '3', 'A'},
                             {'4', '5', '6', 'B'},
                             {'7', '8', '9', 'C'},
                             {'*', '0', '#', 'D'}};
  size_t row;
  size_t col;
  if (freq1 > freq2) {
    std::swap(freq1, freq2);
  }

  if (freq1 < 733.5) {
    row = 0;
  } else if (freq1 < 811) {
    row = 1;
  } else if (freq1 < 896.5) {
    row = 2;
  } else {
    row = 3;
  }

  if (freq2 < 1272.5) {
    col = 0;
  } else if (freq2 < 1406.5) {
    col = 1;
  } else if (freq2 < 1555) {
    col = 2;
  } else {
    col = 3;
  }

  return table[row][col];
}
