Voltage controlled phase shifter firmware for the disting Eurorack module

Based on theory found in: 

https://ccrma.stanford.edu/~jos/pasp/Classic_Virtual_Analog_Phase.html 

Implemented using fixed point arithmetic borrowed from:

https://hackaday.io/project/5290-pic32-fixed-floating-point-performance


Usage:

Z input controls the spread of filter notches. Ranges between a very small value (notches become very closely arranged) and a value corresponding roughly to 1 octave/notch.
X input controls phaser sweep
Y input is audio source input
A and B are mix output

# vorshtein_disting
