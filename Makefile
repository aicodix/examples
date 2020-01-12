
CXXFLAGS = -std=c++11 -W -Wall -Ofast -fno-exceptions -fno-rtti -march=native -I../dsp
CXX = clang++ -stdlib=libc++
#CXX = g++

.PHONY: all

all: fmdemod fmstereo interpol hermite uniform

fmdemod: fmdemod.cc
	$(CXX) $(CXXFLAGS) $< -o $@

fmstereo: fmstereo.cc
	$(CXX) $(CXXFLAGS) $< -o $@

interpol: interpol.cc
	$(CXX) $(CXXFLAGS) $< -o $@

hermite: hermite.cc
	$(CXX) $(CXXFLAGS) $< -o $@

uniform: uniform.cc
	$(CXX) $(CXXFLAGS) $< -o $@

.PHONY: clean

clean:
	rm -f fmdemod fmstereo interpol hermite uniform

