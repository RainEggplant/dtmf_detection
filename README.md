# DTMF Detection
This is a course project for _Digital Signal Processing_ of E.E., Tsinghua University.

See the [report](report.pdf).


## Contents

This project contains:

- A DSP library written in C++:
  - FFT
  - Utilities including:
    - `abs_vector` for `std::vector<std::complex<double>>`
    - `convert_to_complex` from two `vectors<double>` of real and imag part
    - `find_peaks`
- Goertzel Algorithm written in MATLAB

This project solves 3 problems:

1. Detect single telephone key using FFT
2. Detect single telephone key using Goertzel Algorithm
3. Detect continous telephone keys using FFT and Goertzel Algorithm

## Credits

[adamstark/AudioFile](https://github.com/adamstark/AudioFile) : Used for reading wav files.

[user lastchance on cplusplus.com](http://www.cplusplus.com/forum/beginner/219698/) : peak finding algorithm.



