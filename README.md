Some examples of using our DSP code library

### [fmdemod.cc](fmdemod.cc)

[Frequency modulation](https://en.wikipedia.org/wiki/Frequency_modulation) [demodulation](https://en.wikipedia.org/wiki/Demodulation) of commercial [FM broadcasting](https://en.wikipedia.org/wiki/FM_broadcasting).

```
REGION=1
FREQ=106.4M
IRATE=2048000
ORATE=44100
rtl_sdr -f $FREQ -s $IRATE - | ./fmdemod $IRATE $ORATE $REGION | aplay -t raw -c 1 -r $ORATE -f float

```

