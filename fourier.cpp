#include "fourier.h"
#include "timestamp.h"
#include <cassert>
#include <cmath>
#include <complex>

// Initialse twiddle matrix
auto fourier_init() {

  _timestamp(__LINE__, __FILE__);

  // const size_t bins{fourier_bins};

  // Declare container for twiddle matrix
  std::vector<std::complex<double>> twiddle;
  twiddle.reserve(bins<size_t> * bins<size_t> / 2);

  // Populate twiddle matrix
  using namespace std::complex_literals;
  const auto two_i_times_pi = 2i * pi<double>;

  for (size_t k = 0; k < bins<size_t> / 2; ++k) {
    const double K          = k;
    const auto imag_times_k = two_i_times_pi * K;

    for (size_t n = 0; n < bins<size_t>; ++n) {

      const double N = n;
      twiddle.push_back(exp(imag_times_k * N / bins<double>));
    }
  }

  _timestamp(__LINE__, __FILE__);

  return twiddle;
}

std::vector<double> get_fourier(const std::vector<sample_t> &samples) {

  assert(samples.size() >= bins<size_t> &&
         "fourier bins greater than samples size");

  _timestamp(__LINE__, __FILE__);
  // Initialise twiddle matrix on first call
  static const auto &twiddle = fourier_init();

  // Initialise results container and reserve enough space for the bins
  // const size_t bins{fourier_bins};
  std::vector<double> fourier;
  fourier.reserve(bins<size_t>);

  // Calculate Fourier response
  for (size_t k = 0; k < bins<size_t> / 2; ++k) {

    // We want to calculate the sum of all responses
    std::complex<double> sum{};

    // Calculate response for each sample
    for (unsigned int n = 0; n < bins<size_t>; ++n)
      sum += twiddle[(k * bins<size_t>)+n] *
             std::complex<double>(samples.at(n), 0);

    // Store the absolute value of the complex sum
    fourier.push_back(abs(sum));
  }

  _timestamp(__LINE__, __FILE__);

  return fourier;
}
