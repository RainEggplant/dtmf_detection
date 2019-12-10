#include "utils.h"

namespace dsp {
vector<double> abs_vector(const vector<complex<double>> &X) {
  vector<double> X_abs(X.size());
  std::transform(X.begin(), X.end(), X_abs.begin(),
                 [](complex<double> X_k) { return std::abs(X_k); });
  return X_abs;
}

vector<complex<double>> convert_to_complex(const vector<double> &x_real,
                                           const vector<double> &x_imag) {
  vector<std::complex<double>> x_comp(x_real.size());
  std::transform(x_real.begin(), x_real.end(), x_imag.begin(), x_comp.begin(),
                 [](double real, double imag) {
                   return std::complex<double>(real, imag);
                 });
  return x_comp;
}

void find_peaks(const vector<double> &amp,
                vector<pair<size_t, size_t>> *wide_peaks,
                vector<size_t> *sharp_peaks, double threshold) {
  size_t wideStart = 0;  // The start of any current wide peak

  int grad = -1;  // Sign of gradient (almost)
                  //    =  1 for increasing
                  //    =  0 for level AND PREVIOUSLY INCREASING (so potential
                  //         wide peak)
                  //    = -1 for decreasing OR level, but previously
                  //         decreasing
                  // A sharp peak is identified by grad=1 -> grad=-1
                  // A wide  peak is identified by grad=0 -> grad=-1

  for (size_t i = 0; i < amp.size() - 1; ++i) {
    if (amp[i + 1] < amp[i])  // Only possibility of a peak
    {
      if (grad == 1 && amp[i] > threshold) {
        (*sharp_peaks).push_back(i);
      } else if (grad == 0 && amp[i] > threshold) {
        (*wide_peaks).push_back(pair<size_t, size_t>(wideStart, i));
      }
      grad = -1;
    } else if (amp[i + 1] == amp[i])  // Check for start of a wide peak
    {
      if (grad == 1) {
        wideStart = i;
        grad = 0;
      }
    } else {
      grad = 1;
    }
  }
}
}  // namespace dsp
