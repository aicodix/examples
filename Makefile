
CXXFLAGS = -std=c++11 -W -Wall -Ofast -fno-exceptions -fno-rtti -march=native -I../dsp
CXX = clang++ -stdlib=libc++
#CXX = g++

.PHONY: all

all: fmdemod fmstereo fdzp hermite hermite2 uniform

fmdemod: fmdemod.cc
	$(CXX) $(CXXFLAGS) $< -o $@

fmstereo: fmstereo.cc
	$(CXX) $(CXXFLAGS) $< -o $@

fdzp: fdzp.cc
	$(CXX) $(CXXFLAGS) $< -o $@

hermite: hermite.cc
	$(CXX) $(CXXFLAGS) $< -o $@

hermite2: hermite2.cc
	$(CXX) $(CXXFLAGS) $< -o $@

uniform: uniform.cc
	$(CXX) $(CXXFLAGS) $< -o $@

.PHONY: clean

clean:
	rm -f fmdemod fmstereo fdzp hermite hermite2 uniform

