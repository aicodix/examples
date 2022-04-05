/*
Fs/4 Complex down conversion

Copyright 2020 Ahmet Inan <inan@aicodix.de>
*/

#include <iostream>
#include <cmath>
#include "complex.hh"
#include "cdc.hh"
#include "wav.hh"

int main(int argc, char **argv)
{
	if (argc < 3 || argc > 4) {
		std::cerr << "usage: " << argv[0] << " OUTPUT INPUT [COMPLEX|POLAR]" << std::endl;
		return 1;
	}

	const char *out_name = argv[1];
	const char *inp_name = argv[2];

	bool polar = argc == 4 && strcmp("POLAR", argv[3]) == 0;

	typedef float value;
	typedef DSP::Complex<value> cmplx;

	DSP::ReadWAV<value> inp_file(inp_name);

	if (inp_file.channels() != 1) {
		std::cerr << "Need real signal." << std::endl;
		return 1;
	}

	if (inp_file.rate() & 1) {
		std::cerr << "Input rate not even." << std::endl;
		return 1;
	}

	DSP::WriteWAV<value> out_file(out_name, inp_file.rate() / 2, inp_file.bits(), 2);

	const int TAPS = 129;
	DSP::CDC<cmplx, TAPS> cdc;

	while (out_file.good() && inp_file.good()) {
		value input[2];
		inp_file.read(input, 2);
		cmplx output = cdc(input[0], input[1]);
		if (polar) {
			value mag = abs(output);
			value ang = arg(output) / DSP::Const<value>::Pi();
			output = cmplx(mag, ang);
		}
		out_file.write(reinterpret_cast<value *>(&output), 1);
	}

	return 0;
}

