#ifndef CVRECORDER_H
#define	CVRECORDER_H

#include "global.h"
#include "leds.h"

#define __CVRECORDER_Z_TRESH__ (256)

#define __CVRECORDER_BUFFER_LENGTH__ (SAMPLE_RATE / 200)

#define cvWriteIndicator() (pot > __CVRECORDER_Z_TRESH__)

typedef struct {
    fix32 x;
    fix32 y;
} frame_t;

static frame_t  __cvBuffer[__CVRECORDER_BUFFER_LENGTH__];
static int      __pHead  = 0;
static int      __pStart = 0;
static int      __pEnd   = __CVRECORDER_BUFFER_LENGTH__;

void inline cvMoveHead() {
    if (__pHead < (__pEnd - 1)) {
        ++__pHead;
    } else {
        __pHead = __pStart;
    }
}

void inline 
cvWriteBuffer(const frame_t* inFrame) {
    
    *(__cvBuffer + __pHead) = *inFrame;
    
}

void inline 
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

            /**
            *  4   0
            *  5   1
            *  6   2
            *  7   3
            **/

#define LEDS_ALL (0x40516273)

#define LEDS_FW_CYCLE (0x45673210)

#define LEDS_ALL_ON (0b11111111)

#define LEDS_ALL_OFF (0b00000000)

void 
doCvRecorder(const fix16 subSampleTicks) {
    // setup
    DECLARATIONS();
    
    static fix16 subSampleCount = 0;

    ledsLock('P');
    
    for (;;) {
        // wait for new audio frame
        IDLE();
        
        if (cvWriteIndicator() && ledsLocked(LEDS_FW_CYCLE)) {
            //setLeds(LEDS_ALL, LEDS_ALL_ON);
            //ledsLock(LEDS_ALL);
        } else {

            
        }

        ledsCycle(LEDS_FW_CYCLE, __CVRECORDER_BUFFER_LENGTH__ * 4);
        ledsLock(LEDS_FW_CYCLE);
        
        if (++subSampleCount >= subSampleTicks) {
            subSampleCount = 0;

            frame_t inFrame;
            inFrame.x = inX;
            inFrame.y = inY;

            if (cvWriteIndicator()) {
                cvWriteBuffer(&inFrame);
                      
            }
            
            cvMoveHead();

        } else {
            frame_t outFrame;
            fix32 weight = divfix32(subSampleCount, subSampleTicks);
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

