/* 
 * File:   leds.h
 * Author: seb
 *
 * Created on May 20, 2016, 5:59 PM
 */

#ifndef LEDS_H
#define	LEDS_H

#include "global.h"

static char __leds_lock = 0xFF;

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

static void
ledsOff() {
    LED_OFF(0)
    LED_OFF(1)
    LED_OFF(2)
    LED_OFF(3)
    LED_OFF(4)
    LED_OFF(5)
    LED_OFF(6)
    LED_OFF(7)
}

static void
ledsR() {
    LED_OFF(4)
    LED_OFF(0)
    LED_OFF(5)
    LED_OFF(1)
    LED_ON(6)
    LED_ON(2)
    LED_ON(7)
    LED_ON(3)
}




void
setLeds(const char mode) {

    switch (mode) {
        case 'R':
            ledsR();
            break;
        case 0:
            ledsOff();
            break;
        default:
            break;
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

static inline void ledsCycle(unsigned short yo) {
    static char __state = 0;
    static char __curr_idx = 0;
    static char __prev_idx = 7;
    static unsigned short cycleCounter = 0;
    
    if (++cycleCounter >= yo) {
        cycleCounter = 0;
    } else {
        return;
    }
    
    if (__state == 0) {
        LED_OFF(__curr_idx)
        LED_ON(__prev_idx)

        __state = 1;
    } else if (__state == 1) {
        LED_OFF(__prev_idx)
        LED_ON(__curr_idx)

        __state = 2;
    } else if (__state == 2) {
        __prev_idx = __curr_idx;
        if (++__curr_idx > 7) {
            __curr_idx = 0;
        }

        __state = 0;
    }
}

static void ledsConditionalCycle(char doIt) {
    if (doIt) {
        ledsCycle(1);
    }
}


#endif	/* LEDS_H */

