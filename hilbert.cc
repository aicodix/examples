/*
Discrete Hilbert transform

Copyright 2020 Ahmet Inan <inan@aicodix.de>
*/

#include <iostream>
#include <cmath>
#include "complex.hh"
#include "hilbert.hh"
#include "fmd.hh"
#include "wav.hh"

int main(int argc, char **argv)
{
	if (argc < 3 || argc > 4) {
		std::cerr << "usage: " << argv[0] << " OUTPUT INPUT [COMPLEX|POLAR|FREQ]" << std::endl;
		return 1;
	}

	const char *out_name = argv[1];
	const char *inp_name = argv[2];

	int mode = 0;
	if (argc == 4) {
		if (strcmp("POLAR", argv[3]) == 0)
			mode = 1;
		if (strcmp("FREQ", argv[3]) == 0)
			mode = 2;
	}

	typedef float value;
	typedef DSP::Complex<value> cmplx;

	DSP::ReadWAV<value> inp_file(inp_name);

	if (inp_file.channels() != 1) {
		std::cerr << "Need real signal." << std::endl;
		return 1;
	}

	DSP::WriteWAV<value> out_file(out_name, inp_file.rate(), inp_file.bits(), 2);

	const int TAPS = 129;
	DSP::Hilbert<cmplx, TAPS> hilbert;
	DSP::FMD1<cmplx> demod;
	demod.bandwidth(1);

	while (out_file.good() && inp_file.good()) {
		value input;
		inp_file.read(&input, 1);
		cmplx output = hilbert(input);
		if (mode > 0) {
			value mag = abs(output);
			value ang;
			if (mode == 1)
				ang = arg(output) / DSP::Const<value>::Pi();
			else
				ang = demod(output);
			output = cmplx(mag, ang);
		}
		out_file.write(reinterpret_cast<value *>(&output), 1);
	}

	return 0;
}

