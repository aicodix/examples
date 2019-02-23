
CXXFLAGS = -std=c++11 -W -Wall -Ofast -fno-exceptions -fno-rtti -march=native -I../dsp
CXX = clang++ -stdlib=libc++
#CXX = g++

fmdemod: fmdemod.cc
	$(CXX) $(CXXFLAGS) $< -o $@

.PHONY: clean

clean:
	rm -f fmdemod

