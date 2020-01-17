/*
Interpolation in 2D using Cubic Hermite Spline

Copyright 2020 Ahmet Inan <inan@aicodix.de>
*/

#include <stdio.h>
#include <random>
#include <functional>
#include "spline.hh"
#include "complex.hh"

int main()
{
	const int SAMPLES = 16;
	typedef float value;
	const value STEP = 0.05;
	typedef DSP::Complex<value> cmplx;
	typedef DSP::CubicHermiteSpline<cmplx, value> hermite;
	value X[SAMPLES];
	cmplx Y[SAMPLES];
	std::default_random_engine generator(42);
	typedef std::uniform_real_distribution<value> distribution;
	auto noise = std::bind(distribution(-1, 1), generator);
	for (int i = 0; i < SAMPLES; ++i) {
		Y[i] = cmplx(noise(), noise());;
		X[i] = i ? X[i-1]+abs(Y[i]-Y[i-1]) : value(0);
	}
	for (int i = 0; i < SAMPLES; ++i)
		printf("%g %g %g NaN NaN\n", X[i], Y[i].real(), Y[i].imag());
	for (value x = X[0]; x <= X[SAMPLES-1]; x += STEP) {
		cmplx tmp = hermite::eval(X, Y, x, SAMPLES);
		printf("%g NaN NaN %g %g\n", x, tmp.real(), tmp.imag());
	}
	return 0;
}

