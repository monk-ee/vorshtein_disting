/* 
 * File:   leds.h
 * Author: seb
 *
 * Created on May 20, 2016, 5:59 PM
 */

#ifndef LEDS_H
#define	LEDS_H

#include "global.h"

static unsigned int __leds_lock = 0xFFFFFFFF;

#define ledsLocked(key) (__leds_lock == key)

#define ledsLock(key) {__leds_lock = key; }



/*
    { BIT_3, 0 },
    { 0, BIT_15 },
    { 0, BIT_11 },
    { 0, BIT_7 },
    { 0, BIT_4 },
    { 0, BIT_12 },
    { 0, BIT_10 },
    { 0, BIT_6 },

 */


#define __LED0__ {BIT_3, 0}

#define __LED1__ {0, BIT_15}

#define __LED2__ {0, BIT_11}

#define __LED3__ {0, BIT_7}

#define __LED4__ {0, BIT_4}

#define __LED5__ {0, BIT_12}

#define __LED6__ {0, BIT_10}

#define __LED7__ {0, BIT_6}

/**
 *  4   0
 *  5   1
 *  6   2
 *  7   3
 **/
static unsigned int __leds[8][2] = {
    __LED0__,
    __LED1__,
    __LED2__,
    __LED3__,
    __LED4__,
    __LED5__,
    __LED6__,
    __LED7__
};



#define LED_OFF(index)  {PORTACLR = __leds[index][0]; PORTBCLR = __leds[index][1];}
#define LED_ON(index)   {PORTASET = __leds[index][0]; PORTBSET = __leds[index][1];}

/* 
 * pattern e.g 0x7643210
 * 
 * mask e.g 0b10101010
 * 
 * 
 */
void
setLeds(register unsigned int pattern,
        register unsigned char mask) {
    static unsigned int index;

    for (index = pattern; index > 0; index >>= 4, mask >>= 1) {
        if (mask & 0x1) {
            LED_ON(index & 0x7);
        } else {
            LED_OFF(index & 0x7)
        }
    }

}

inline char
onZeroCrossing(const fix32 x) {
    static fix32 prevX;
    static fix16 count;

    if ((prevX & 0x80000000) ^ (x & 0x80000000)) {
        if (++count & BIT_3) {
            count = 0;
            return 1;
        }
    }

    prevX = x;

    return 0;
}

static inline void ledsCycle(register unsigned int seqFull,
        register unsigned short slowDownFactor) {
    static unsigned int seq = 0;
    static unsigned char curr = 0;
    static unsigned char prev = 0;
    static unsigned short cycleCounter = 0;
    static unsigned char state = 0;

    if (++cycleCounter > slowDownFactor) {
        cycleCounter = 0;
    } else {
        return;
    }

    if (state == 0) {

        LED_OFF(curr)
        LED_ON(prev)
        state = 1;

    } else if (state == 1) {

        LED_OFF(prev)
        LED_ON(curr)
        state = 2;

    } else if (state == 2) {

        prev = curr;
        seq >>= 4;
        curr = seq & 0x7;

        if (curr == 0) {
            seq = seqFull;
            curr = seq & 0x7;
        }

        state = 0;
    }
}

static void ledsConditionalCycle(char doIt) {
    if (doIt) {
        ledsCycle(0x76543210, 1);
    }
}


#endif	/* LEDS_H */

