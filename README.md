Some examples of using our DSP code library

### [fmdemod.cc](fmdemod.cc)

[Frequency modulation](https://en.wikipedia.org/wiki/Frequency_modulation) [demodulation](https://en.wikipedia.org/wiki/Demodulation) of commercial [FM broadcasting](https://en.wikipedia.org/wiki/FM_broadcasting).

```
REGION=1
FREQ=106.4M
IRATE=2048000
MRATE=256000
ORATE=44100
rtl_sdr -f $FREQ -s $IRATE - | ./fmdemod $IRATE $MRATE $ORATE $REGION | aplay -t raw -c 1 -r $ORATE -f float
```

### [fmstereo.cc](fmstereo.cc)

[Frequency modulation](https://en.wikipedia.org/wiki/Frequency_modulation) [demodulation](https://en.wikipedia.org/wiki/Demodulation) of commercial [stereo](https://en.wikipedia.org/wiki/Stereophonic_sound) [FM broadcasting](https://en.wikipedia.org/wiki/FM_broadcasting).

```
REGION=1
FREQ=106.4M
IRATE=2400000
MRATE=480000
ORATE=48000
rtl_sdr -f $FREQ -s $IRATE - | ./fmstereo $IRATE $MRATE $ORATE $REGION | aplay -t raw -c 2 -r $ORATE -f float
```

For lower CPU usage and fewer underruns, choose lower rates and avoid PulseAudio:
```
REGION=1
FREQ=106.4M
IRATE=1024000
MRATE=256000
ORATE=32000
DEVICE=$(aplay -L | grep -m1 sysdefault)
rtl_sdr -f $FREQ -s $IRATE - | ./fmstereo $IRATE $MRATE $ORATE $REGION | aplay -t raw -c 2 -r $ORATE -f float -D $DEVICE
```

### [interpol.cc](interpol.cc)

Interpolation via frequency-domain zero padding.

Use gnuplot to look at the example:
```
plot "< ./interpol" u 1:2 w l, "< ./interpol" u 1:3 w l, "< ./interpol" u 1:4 w p, "< ./interpol" u 1:5 w p
```
![interpol.png](interpol.png)

