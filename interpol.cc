/*
Stereo FM demodulation example

Copyright 2019 Ahmet Inan <inan@aicodix.de>
*/

#include <stdio.h>
#include <cmath>
#include <random>
#include "fdzp.hh"
#include "complex.hh"

int main()
{
	const int BINS = 15;
	const int FACT = 10;
	typedef float value;
	typedef DSP::Complex<value> cmplx;
	DSP::FDZP<BINS, FACT, cmplx> fdzp;
	cmplx input[BINS], output[BINS * FACT];
	std::default_random_engine generator(42);
	std::uniform_real_distribution<value> distribution(-value(1), value(1));
	auto noise = std::bind(distribution, generator);
	for (int i = 0; i < BINS; ++i)
		input[i] = cmplx(noise(), noise());
	fdzp(output, input);
	for (int i = 0; i < BINS; ++i)
		printf("%g # # %g %g\n", value(i) / value(BINS), input[i].real(), input[i].imag());
	for (int i = 0; i < BINS * FACT; ++i)
		printf("%g %g %g # #\n", value(i) / value(BINS * FACT), output[i].real(), output[i].imag());
	return 0;
}

