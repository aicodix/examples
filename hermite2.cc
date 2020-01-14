/*
Interpolation in 2D using Cubic Hermite Spline

Copyright 2020 Ahmet Inan <inan@aicodix.de>
*/

#include <stdio.h>
#include <random>
#include <functional>
#include "spline.hh"
#include "complex.hh"
#include "utils.hh"

int main()
{
	const int SAMPLES = 16;
	const int FACT = 10;
	typedef float value;
	typedef DSP::Complex<value> cmplx;
	typedef DSP::CubicHermiteSpline<cmplx, value> hermite;
	value x[SAMPLES];
	cmplx y[SAMPLES];
	std::default_random_engine generator(42);
	typedef std::uniform_real_distribution<value> distribution;
	auto noise = std::bind(distribution(-1, 1), generator);
	for (int i = 0; i < SAMPLES; ++i) {
		y[i] = cmplx(noise(), noise());;
		x[i] = i ? x[i-1]+abs(y[i]-y[i-1]) : value(0);
	}
	for (int i = 0; i < SAMPLES; ++i)
		printf("%g %g %g NaN NaN\n", x[i], y[i].real(), y[i].imag());
	for (int k = 0; k < SAMPLES-1; ++k) {
		for (int i = 0; i < FACT; ++i) {
			value t = value(i) / value(FACT);
			cmplx tmp = hermite::eval(x, y, t, k, SAMPLES);
			printf("%g NaN NaN %g %g\n", DSP::lerp(t, x[k], x[k+1]), tmp.real(), tmp.imag());
		}
	}
	cmplx tmp = hermite::eval(x, y, value(1), SAMPLES-2, SAMPLES);
	printf("%g NaN NaN %g %g\n", x[SAMPLES-1], tmp.real(), tmp.imag());
	return 0;
}

