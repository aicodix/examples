/*
Stereo FM demodulation example

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
#include "calculus.hh"

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
	if (argc != 4)
		return 1;
	int irate = atoi(argv[1]);
	int orate = atoi(argv[2]);
	int knee = region(argv[3]);
	typedef float value;
	typedef DSP::Complex<value> cmplx;
	DSP::EMACascade<cmplx, value, 3> iflp;
	iflp.cutoff(75000, irate);
	DSP::NormalizeIQ<cmplx> normalize;
	normalize.samples(irate / 10);
	DSP::FMD5<cmplx> demod;
	demod.bandwidth(value(150000) / value(irate));
	DSP::Biquad<value, value> notch, bandpass;
	notch.notch(19000, irate, 24);
	bandpass.bandpass(19000, irate, 24);
	DSP::Differentiator<value> diff;
	DSP::BiquadCascade<cmplx, value, 2> aalp;
	int fmfc = 15000;
	int aafc = (orate * 9) / 20;
	if (aafc > fmfc)
		aafc = fmfc;
	aalp.lowpass(aafc, irate);
	DSP::EMA<cmplx, value> deemp;
	deemp.cutoff(knee, orate);
	DSP::EMA<cmplx, value> bias;
	bias.cutoff(30, orate);

	for (int n = 0;; n -= irate) {
		cmplx tmp(0);
		for (; n < irate; n += orate) {
			cmplx iq = input<cmplx>();
			iq = iflp(iq);
			iq = normalize(iq);
			value phase = demod(iq);
			value pilot = bandpass(phase);
			value level = notch(phase);
			value twice = pilot * diff(pilot);
			value l(0), r(0);
			if (twice >= value(0))
				l = value(2) * level;
			else
				r = value(2) * level;
			tmp = cmplx(l, r);
			tmp = aalp(tmp);
		}
		tmp = deemp(tmp);
		tmp -= bias(tmp);
		output(tmp);
	}
	return 0;
}

