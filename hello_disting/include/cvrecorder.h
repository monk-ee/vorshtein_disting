#ifndef CVRECORDER_H
#define	CVRECORDER_H

#include "global.h"
#include "leds.h"

#define CVBUF_SIZE (4096)

static fix32 __cvbuf[CVBUF_SIZE];

static int __subsample_factor = 1;
static int __count = 0;

static int __head = 0;

void inline cvRecord(
        register fix32 x,
        register fix32 y) {

    register fix32* pBuffer = __cvbuf + __head;

    *pBuffer = x;
    *(pBuffer + 1) = y;
}

void inline cvPlay(
        register fix32* xOut,
        register fix32* yOut,
        register fix32 weight) {

    register fix32* pBuffer = __cvbuf + __head;

    register fix32 xCurr = *pBuffer;
    register fix32 yCurr = *(pBuffer + 1);

    register fix32 xNext;
    register fix32 yNext;

    if (__head < (CVBUF_SIZE - 3)) {
        xNext = *(pBuffer + 2);
        yNext = *(pBuffer + 3);
    } else {
        xNext = *__cvbuf;
        yNext = *(__cvbuf + 1);
    }

    *xOut = linterp(weight, xCurr, xNext);
    //*yOut = linterp_opt(weight, yCurr, yNext);
    *yOut = yCurr;

}

void doCvRecorder(register fix32 subSampleTicks) {
    // setup
    DECLARATIONS();
    static fix32 subSampleCount = 0;

    for (;;) {
        // wait for new audio frame
        IDLE();

        // do the processing
        //doLeds(y);

        if (++subSampleCount > subSampleTicks) {
            subSampleCount = 1;

            if (pot > 127) {
                cvRecord(inX, inY);
            }

            __head += 2;

            if (__head > CVBUF_SIZE - 2) {
                __head = 0;
            }
        } else {
            register fix32 weight = divfix32(subSampleCount, subSampleTicks);
            cvPlay(&outA, &outB, weight);
        }





        // loop end processing
        // (including reading the ADC channels)
        LOOP_END();

    }
}

#endif	/* BYPASS_H */

