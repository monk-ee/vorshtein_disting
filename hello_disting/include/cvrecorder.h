#ifndef CVRECORDER_H
#define	CVRECORDER_H

#include "global.h"
#include "leds.h"

#define __CVRECORDER_Z_TRESH__ (256)

#define __CVRECORDER_BUFFER_LENGTH__ (4096 + 1)
#define cvWriteIndicator() (pot > __CVRECORDER_Z_TRESH__)

typedef struct {
    fix32 x;
    fix32 y;
} frame_t;

static frame_t __cvBuffer[__CVRECORDER_BUFFER_LENGTH__];
static int __pHead = 0;
static int __pStart = 0;
static int __pEnd = __CVRECORDER_BUFFER_LENGTH__;

static inline char
cvStepLeds(register const fix16 subSampleTicks) {
    register fix32 timeDiv = (__pEnd - __pStart - 1) / subSampleTicks;
    register fix32 result = (__pHead - __pStart + 1) % timeDiv;
    return result == 0;
}

static inline void
cvMoveHead() {
    if (__pHead < (__pEnd - 1)) {
        ++__pHead;
    } else {
        __pHead = __pStart;
    }
}

static inline void
cvResetHead() {
    __pHead = __pStart;
}

static inline void
cvWriteBuffer(const frame_t* inFrame) {
    *(__cvBuffer + __pHead) = *inFrame;
}

static inline void
cvReadBuffer(frame_t* outFrame, const fix32 weight) {

    frame_t currFrame;
    frame_t nextFrame;

    if (__pHead >= __pStart && __pHead < (__pEnd - 1)) {
        currFrame = *(__cvBuffer + __pHead);
        nextFrame = *(__cvBuffer + __pHead + 1);
    } else {
        currFrame = *(__cvBuffer + __pStart);
        nextFrame = *(__cvBuffer + __pStart + 1);
    }

    outFrame->x = linterp(weight, currFrame.x, nextFrame.x);
    outFrame->y = linterp(weight, currFrame.y, nextFrame.y);
}

void
doCvRecorder(const fix16 subSampleTicks) {
    // setup
    DECLARATIONS();

    fix16 subSampleCount = 0;

    cvResetHead();
    ledsResetCycle();

    for (;;) {
        // wait for new audio frame
        IDLE();

        register char doStep = cvStepLeds(8);

        ledsConditionalCycle(LEDS_FW_CYCLE, doStep, subSampleTicks);

        if (++subSampleCount >= subSampleTicks) {
            subSampleCount = 0;

            frame_t inFrame;
            inFrame.x = inX;
            inFrame.y = inY;

            if (cvWriteIndicator()) {
                cvWriteBuffer(&inFrame);
            }

            cvMoveHead();

        }

        fix32 weight = divfix32(subSampleCount, subSampleTicks);

        frame_t outFrame;
        cvReadBuffer(&outFrame, weight);

        outA = outFrame.x;
        outB = outFrame.y;

        // loop end processing
        // (including reading the ADC channels)
        LOOP_END();
    }
}

#endif	/* CVRECORDER_H */

