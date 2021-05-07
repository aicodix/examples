/*
Interpolation using Cubic Hermite Spline

Copyright 2020 Ahmet Inan <inan@aicodix.de>
*/

#include <stdio.h>
#include <random>
#include <functional>
#include "spline.hh"
#include "utils.hh"

int main()
{
	const int SAMPLES = 16;
	const int FACT = 10;
	typedef float value;
	typedef DSP::CubicHermiteSpline<value, value> hermite;
	value x[SAMPLES], y[SAMPLES];
	std::default_random_engine generator(42);
	typedef std::uniform_real_distribution<value> distribution;
	auto step = std::bind(distribution(1, 10), generator);
	auto level = std::bind(distribution(-10, 10), generator);
	for (int i = 0; i < SAMPLES; ++i) {
		x[i] = (i ? x[i-1] : 0) + step();
		y[i] = level();
	}
	for (int i = 0; i < SAMPLES; ++i)
		printf("%g %g NaN\n", x[i], y[i]);
	for (int k = 0; k < SAMPLES-1; ++k) {
		for (int i = 0; i < FACT; ++i) {
			value t = value(i) / value(FACT);
			printf("%g NaN %g\n", DSP::lerp(x[k], x[k+1], t), hermite::eval(x, y, t, k, SAMPLES));
		}
	}
	printf("%g NaN %g\n", x[SAMPLES-1], hermite::eval(x, y, value(1), SAMPLES-2, SAMPLES));
	return 0;
}

