#ifndef CVRECORDER_H
#define	CVRECORDER_H

#include "global.h"
#include "leds.h"

#define CVBUF_SIZE (6 * 4096)
#define CVBUF_END (CVBUF_SIZE - 1)

static short __cvbuf[CVBUF_SIZE];

static int __subsample_factor = 1;

static int __head = 0;
static int __count = 0;

void inline cvRecord(int x, int y) {
    *(__cvbuf + __head) = (short) (x >> 8);
    *(__cvbuf + __head + 1) = (short) (y >> 8);
}

void inline cvPlay(int* x, int* y) {
    *x = *(__cvbuf + __head) << 8;
    *y = *(__cvbuf + __head + 1) << 8;
}

void doCvRecorder(int sub) {
    // setup
    DECLARATIONS();

    __subsample_factor = sub;
    for (;;) {
        // wait for new audio frame
        IDLE();

        int x = inX;
        int y = inY;

        // do the processing
        //doLeds(y);

        if (__count++ >= __subsample_factor) {
            __count = 0;

            if (pot > 512) {
                cvRecord(x, y);
            } else {

            }
            cvPlay(&outA, &outB);
            __head += 2;

            if (__head >= CVBUF_END) {
                __head = 0;
            }

        }

        // loop end processing
        // (including reading the ADC channels)
        LOOP_END();

    }
}

#endif	/* BYPASS_H */

