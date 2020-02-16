/*
Interpolation via frequency-domain zero padding.

Copyright 2019 Ahmet Inan <inan@aicodix.de>
*/

#include <stdio.h>
#include <cmath>
#include <random>
#include <functional>
#include "fdzp.hh"
#include "complex.hh"

int main()
{
	const int INPUT = 15;
	const int OUTPUT = 150;
	typedef float value;
	typedef DSP::Complex<value> cmplx;
	DSP::FDZP<OUTPUT, INPUT, cmplx> fdzp;
	cmplx input[INPUT], output[OUTPUT];
	std::default_random_engine generator(42);
	std::uniform_real_distribution<value> distribution(-value(1), value(1));
	auto noise = std::bind(distribution, generator);
	for (int i = 0; i < INPUT; ++i)
		input[i] = cmplx(noise(), noise());
	fdzp(output, input);
	for (int i = 0; i < INPUT; ++i)
		printf("%g %g %g NaN NaN\n", value(i) / value(INPUT), input[i].real(), input[i].imag());
	for (int i = 0; i <= OUTPUT; ++i)
		printf("%g NaN NaN %g %g\n", value(i) / value(OUTPUT), output[i%OUTPUT].real(), output[i%OUTPUT].imag());
	return 0;
}

