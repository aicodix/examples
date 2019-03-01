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

template <typename CMPLX>
CMPLX input()
{
	unsigned char inp[2];
	if (fread_unlocked(inp, 2, 1, stdin) != 1)
		exit(0);
	typename CMPLX::value_type
		real(inp[0]),
		imag(inp[1]),
		bias(127.5),
		scale(0.0078125);
	return scale * CMPLX(real - bias, imag - bias);
}

template <typename TYPE>
void output(TYPE out)
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
	if (argc != 5)
		return 1;
	int irate = atoi(argv[1]);
	int mrate = atoi(argv[2]);
	int orate = atoi(argv[3]);
	int knee = region(argv[4]);
	typedef float value;
	typedef DSP::Complex<value> cmplx;
	DSP::EMACascade<cmplx, value, 3> iflp;
	iflp.cutoff(75000, irate);
	DSP::NormalizeIQ<cmplx> normalize;
	normalize.samples(mrate / 10);
	DSP::FMD5<cmplx> demod;
	demod.bandwidth(value(150000) / value(mrate));
	DSP::Biquad<value, value> notch;
	notch.notch(19000, mrate, 24);
	DSP::BiquadCascade<value, value, 2> aalp;
	int fmfc = 15000;
	int aafc = (orate * 9) / 20;
	if (aafc > fmfc)
		aafc = fmfc;
	aalp.lowpass(aafc, mrate);
	DSP::EMA<value, value> deemp;
	deemp.cutoff(knee, orate);
	DSP::EMA<value, value> bias;
	bias.cutoff(30, orate);

	for (int i = 0, m = 0;; m -= mrate) {
		value tmp(0);
		for (; m < mrate; m += orate, i -= irate) {
			cmplx iq(0);
			for (; i < irate; i += mrate) {
				iq = input<cmplx>();
				iq = iflp(iq);
			}
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

