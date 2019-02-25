/*
FM demodulation example

Copyright 2019 Ahmet Inan <inan@aicodix.de>
*/

#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include "complex.hh"
#include "ema.hh"
#include "fmd.hh"
#include "biquad.hh"
#include "normalize.hh"

typedef float value;
typedef DSP::Complex<value> cmplx;

cmplx input()
{
	unsigned char inp[2];
	if (fread_unlocked(inp, 2, 1, stdin) != 1)
		exit(0);
	value real(inp[0]);
	value imag(inp[1]);
	value bias(-127.5);
	value scale(0.0078125);
	return scale * cmplx(real + bias, imag + bias);
}

void output(value out)
{
	if (fwrite_unlocked(&out, sizeof(out), 1, stdout) != 1)
		exit(0);
}

int region(const char *str)
{
	switch (atoi(str)) {
	case 1:
		// Europe and Asia: 50µs or 3183Hz
		return 3183;
	case 2:
		// The Americas: 75µs or 2122Hz
		return 2122;
	}
	return 15000;
}

int main(int argc, char **argv)
{
	if (argc != 4)
		return 1;
	int irate = atoi(argv[1]);
	int orate = atoi(argv[2]);
	int knee = region(argv[3]);
	DSP::EMACascade<cmplx, value, 3> iflp;
	iflp.cutoff(75000, irate);
	DSP::NormalizeIQ<cmplx> normalize;
	normalize.samples(irate / 10);
	DSP::FMD5<cmplx> demod;
	demod.bandwidth(value(150000) / value(irate));
	DSP::Biquad<value, value> notch;
	notch.notch(19000, irate, 300);
	DSP::BiquadCascade<value, value, 2> aalp;
	int fmfc = 15000;
	int aafc = (orate * 9) / 20;
	if (aafc > fmfc)
		aafc = fmfc;
	aalp.lowpass(aafc, irate);
	DSP::EMA<value, value> deemp;
	deemp.cutoff(knee, orate);
	DSP::EMA<value, value> bias;
	bias.cutoff(30, orate);

	for (int n = 0;; n -= irate) {
		value tmp(0);
		for (; n < irate; n += orate) {
			cmplx iq = input();
			iq = iflp(iq);
			iq = normalize(iq);
			tmp = demod(iq);
			tmp = notch(tmp);
			tmp = aalp(tmp);
		}
		tmp = deemp(tmp);
		tmp -= bias(tmp);
		output(tmp);
	}
	return 0;
}

