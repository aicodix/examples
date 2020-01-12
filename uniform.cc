/*
Comparing interpolation between Uniform Cubic Hermite Spline and Uniform Natural Cubic Spline

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
	typedef DSP::CubicHermiteSpline<value> hermite;
	value y[SAMPLES];
	std::default_random_engine generator(42);
	typedef std::uniform_real_distribution<value> distribution;
	auto level = std::bind(distribution(-10, 10), generator);
	for (int i = 0; i < SAMPLES; ++i)
		y[i] = level();
	DSP::UniformNaturalCubicSpline<SAMPLES, value, value> natural(y);
	for (int i = 0; i < SAMPLES; ++i)
		printf("%g %g NaN NaN\n", value(i), y[i]);
	for (int k = 0; k < SAMPLES-1; ++k) {
		for (int i = 0; i < FACT; ++i) {
			value t = value(i) / value(FACT);
			value x = value(k) + t;
			printf("%g NaN %g %g\n", x, hermite::eval(y, t, k, SAMPLES), natural(x));
		}
	}
	printf("%g NaN %g %g\n", value(SAMPLES-1), hermite::eval(y, value(1), SAMPLES-2, SAMPLES), natural(value(SAMPLES-1)));
	return 0;
}

