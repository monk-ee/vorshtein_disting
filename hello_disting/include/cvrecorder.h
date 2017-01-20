#ifndef CVRECORDER_H
#define	CVRECORDER_H

#include "global.h"
#include "leds.h"

#define CVBUF_SIZE (6 * 4096)

static fix16 __cvbuf[CVBUF_SIZE];

static int __subsample_factor = 1;
static int __count = 0;

static int __head = 0;

void inline cvRecord(register fix16 x, register fix16 y) {
    register short* pWrite = __cvbuf + __head;

    *pWrite = x;
    *(pWrite + 1) = y;
}

void inline cvPlay(register fix32* x, register fix32* y) {
    register fix16* pw = __cvbuf + __head;

    *x = *pw << 8;
    *y = *(pw + 1) << 8;

    
    // TODO: linear interpolation
    
    register short next_x;
    register short next_y;

    if (__head < CVBUF_SIZE - 3) {
        next_x = *(pw + 2);
        next_y = *(pw + 3);
    } else {
        next_x = *__cvbuf;
        next_y = *(__cvbuf + 1);
    }
}

void doCvRecorder(int sub) {
    // setup
    DECLARATIONS();

    __subsample_factor = sub;
    for (;;) {
        // wait for new audio frame
        IDLE();

        fix16 x = inX >> 8;
        fix16 y = inY >> 8;

        // do the processing
        //doLeds(y);

        if (__count++ >= __subsample_factor) {
            __count = 0;

            if (pot > 512) {
                cvRecord(x, y);
            } else {

            }
            __head += 2;

            if (__head > CVBUF_SIZE - 2) {
                __head = 0;
            }
        }

        cvPlay(&outA, &outB);

        // loop end processing
        // (including reading the ADC channels)
        LOOP_END();

    }
}

#endif	/* BYPASS_H */

