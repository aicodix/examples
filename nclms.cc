/*
Channel estimation using the Normalized Complex Least Mean Squares filter

Copyright 2020 Ahmet Inan <inan@aicodix.de>
*/

#include <iostream>
#include <cmath>
#include <random>
#include <functional>
using std::min;
#include "complex.hh"
#include "lms.hh"
#include "ema.hh"
#include "decibel.hh"

int main()
{
	typedef float value;
	typedef DSP::Complex<value> cmplx;

	std::random_device rd;
	typedef std::default_random_engine generator;
	typedef std::normal_distribution<value> normal;
	auto noise = std::bind(normal(), generator(rd()));

	const int SIZE = 513;
	DSP::NCLMS<cmplx, SIZE> nlms;

	cmplx channel[SIZE];
	for (int i = 0; i < SIZE; ++i)
		channel[i] = cmplx(noise(), noise());

	DSP::BipBuffer<cmplx, SIZE> history;

	DSP::EMA<value, value> ema;
	ema.samples(10);

	for (int s = 0; s < 20000; ++s) {
		cmplx input(noise(), noise());
		const cmplx *hist = history(input);
		cmplx desired = 0;
		for (int i = 0; i < SIZE; ++i)
			desired += channel[i] * hist[i];
		cmplx error = nlms(input, desired);
		value avg = ema(abs(error));
		std::cout << DSP::decibel(avg) << std::endl;
	}

//	for (int i = 0; i < SIZE; ++i)
//		std::cout << abs(channel[i] - nlms[i]) << std::endl;
	return 0;
}

