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
	typedef DSP::CubicHermiteSpline<value, value> hermite;
	value y[SAMPLES];
	std::default_random_engine generator(42);
	typedef std::uniform_real_distribution<value> distribution;
	auto level = std::bind(distribution(-10, 10), generator);
	for (int i = 0; i < SAMPLES; ++i)
		y[i] = level();
	DSP::UniformNaturalCubicSpline<SAMPLES, value, value> natural(y);
	for (int i = 0; i < SAMPLES; ++i)
		printf("%g %g NaN NaN\n", value(i), y[i]);
	for (int k = -FACT; k <= SAMPLES * FACT; ++k) {
		value x = value(k) / value(FACT);
		printf("%g NaN %g %g\n", x, hermite::eval(y, x, SAMPLES), natural(x));
	}
	return 0;
}

