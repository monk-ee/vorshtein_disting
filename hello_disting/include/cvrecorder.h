#ifndef CVRECORDER_H
#define	CVRECORDER_H

#include "global.h"
#include "leds.h"

#define CVBUF_LEN (4096)

typedef struct {
    fix32 x;
    fix32 y;
} frame_t;


static frame_t __cvBuffer[CVBUF_LEN];

static int __pHead = 0;

void inline cvMoveHead() {
    if (__pHead <= (CVBUF_LEN - 2)) {
        __pHead++;
    } else {
        __pHead = 0;
    }
}

void inline cvWriteBuffer(register frame_t* inFrame) {
    register frame_t* pWrite = __cvBuffer + __pHead;
    *pWrite = *inFrame;
}

void inline cvReadBuffer(register frame_t* outFrame, register fix32 weight) {

    frame_t currFrame;
    frame_t nextFrame;

    if (__pHead <= (CVBUF_LEN - 2)) {
        currFrame = *(__cvBuffer + __pHead);
        nextFrame = *(__cvBuffer + __pHead + 1);

    } else {
        currFrame = *(__cvBuffer);
        nextFrame = *(__cvBuffer + 1);
    }

    outFrame->x = linterp(weight, currFrame.x, nextFrame.x);
    outFrame->y = linterp(weight, currFrame.y, nextFrame.y);
}

void doCvRecorder(register fix32 subSampleTicks) {
    // setup
    DECLARATIONS();
    static fix32 subSampleCount = 0;

    for (;;) {
        // wait for new audio frame
        IDLE();

        if (++subSampleCount >= subSampleTicks) {
            subSampleCount = 0;

            frame_t inFrame;
            inFrame.x = inX;
            inFrame.y = inY;

            if (pot > 127) {
                cvWriteBuffer(&inFrame);
            }

            cvMoveHead();

        } else {
            register fix32 weight = divfix32(subSampleCount, subSampleTicks);
            frame_t outFrame;
            
            cvReadBuffer(&outFrame, weight);
            
            outA = outFrame.x;
            outB = outFrame.y;
        }

        // loop end processing
        // (including reading the ADC channels)
        LOOP_END();
    }
}

#endif	/* CVRECORDER_H */

