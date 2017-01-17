#ifndef CVRECORDER_H
#define	CVRECORDER_H

#include "global.h"
#include "leds.h"

#define CVBUF_SIZE (6 * 4096)
#define CVBUF_END (CVBUF_SIZE - 1)

static short __cvbuf[CVBUF_SIZE];

void inline cvRecord(int x, int y) {
    static int head = 0;
    
    *(__cvbuf + head++) = (short) (x >> 8);

    if (head > CVBUF_END) {
        head = 0;
    }
    
    *(__cvbuf + head) = (short) (y >> 8);
}

void inline cvPlay(int* x, int* y) {
    static int head = 0;
    
    *x = *(__cvbuf + head++) << 8;
    
    if (head > CVBUF_END) {
        head = 0;
    }
    
    *y = *(__cvbuf + head) << 8;
}


void doCvRecorder()
{
    // setup
    DECLARATIONS();

    for (;;) {
        // wait for new audio frame
        IDLE();

        int x = inX;
        int y = inY;
        
        // do the processing
        //doLeds(y);

        if (pot > 512) {
            setLed(0, 1);
            setLed(1, 1);
            setLed(2, 1);
            setLed(3, 1);
            setLed(4, 1);
            setLed(5, 1);
            setLed(6, 1);
            setLed(7, 1);
            cvRecord(x, y);

        } else { 
            doLeds(outA);
        }
        
        cvPlay(&outA, &outB);

        // loop end processing
        // (including reading the ADC channels)
        LOOP_END();

    }
}

#endif	/* BYPASS_H */

