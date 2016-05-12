Voltage controlled phase shifter firmware for the disting Eurorack module

Based on theory found in: 

https://ccrma.stanford.edu/~jos/pasp/Classic_Virtual_Analog_Phase.html 

Implemented using fixed point arithmetic borrowed from:

https://hackaday.io/project/5290-pic32-fixed-floating-point-performance


Usage:

Z input modifies the filter notch spectral distribution, variable between "very close" to roughly 1 notch/octave

X input controls the filter breaking frequency

Y input is audio source input

A and B output are currently both wet/dry signals mixed equally. 

Planning to let B output be "inverted dry/wet".

# vorshtein_disting
