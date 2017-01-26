/* 
 * File:   leds.h
 * Author: seb
 *
 * Created on May 20, 2016, 5:59 PM
 */

#ifndef LEDS_H
#define	LEDS_H

#include "global.h"



#define __LED0__ {0, BIT_4}
#define __LED1__ {BIT_3, 0}
#define __LED2__ {0, BIT_15}
#define __LED3__ {0, BIT_11}
#define __LED4__ {0, BIT_7}
#define __LED5__ {0, BIT_6}
#define __LED6__ {0, BIT_10}
#define __LED7__ {0, BIT_12}

/**
    0   1
    7   2
    6   3
    5   4
 
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

static unsigned int __state = 0;
static unsigned int __curr_idx = 0;
static unsigned int __prev_idx = 7;

#define LED_OFF(index)  PORTACLR = __leds[index][0]; PORTBCLR = __leds[index][1];
#define LED_ON(index)   PORTASET = __leds[index][0]; PORTBSET = __leds[index][1];

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
ledsP() {
    LED_ON(0)
    LED_ON(1)
    LED_ON(7)
    LED_ON(2)
    LED_OFF(6)
    LED_OFF(3)
    LED_OFF(5)
    LED_ON(4)
}

static void
ledsR() {
    LED_OFF(0)
    LED_OFF(1)
    LED_OFF(7)
    LED_OFF(2)
    LED_ON(6)
    LED_ON(3)
    LED_ON(5)
    LED_ON(4)
}

static void 
ledsC() {
    LED_ON(0)
    LED_ON(1)
    LED_OFF(2)
    LED_OFF(3)
    LED_ON(4)
    LED_ON(5)
    LED_ON(6)
    LED_ON(7)
}


void 
setLeds(char mode) {
    static char lastMode;
    if (mode == lastMode) {
        return;
    }
    
    lastMode = mode;
    
    switch (mode) {
        case 'P':
            ledsP();
            break;
        case 'C':
            ledsC();
            break;
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

static inline void ledStep() {

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

static void ledConditionalStep(char doIt) {
    if (doIt) {
        ledStep();
    }
}


#endif	/* LEDS_H */

