/* 
 * File:   leds.h
 * Author: seb
 *
 * Created on May 20, 2016, 5:59 PM
 */

#ifndef LEDS_H
#define	LEDS_H

#include "global.h"



/***
 * 0,BIT_4  (0)   BIT_3,0  (1)
 * 0,BIT_12 (2)   0,BIT_15 (3)
 * 0,BIT_10 (4)   0,BIT_11 (5)
 * 0,BIT_6  (6)   0,BIT_7  (7)
 * 
 */

static unsigned int __leds[8][2] = {
    {0, BIT_4}, // 0
    {BIT_3, 0}, // 1
    {0, BIT_15}, // 3
    {0, BIT_11}, // 5
    {0, BIT_7}, // 7
    {0, BIT_6}, // 6
    {0, BIT_10}, // 4
    {0, BIT_12} // 2
};

static unsigned int __state = 0;
static unsigned int __curr_idx = 0;
static unsigned int __prev_idx = 7;

static inline stepLeds()
{

    if (__state == 0) {
        PORTACLR = __leds[__curr_idx][0];
        PORTBCLR = __leds[__curr_idx][1];

        PORTASET = __leds[__prev_idx][0];
        PORTBSET = __leds[__prev_idx][1];

        __state = 1;
    }
    else if (__state == 1) {
        PORTACLR = __leds[__prev_idx][0];
        PORTBCLR = __leds[__prev_idx][1];

        PORTASET = __leds[__curr_idx][0];
        PORTBSET = __leds[__curr_idx][1];

        __state = 2;
    }
    else if (__state == 2) {
        __prev_idx = __curr_idx;
        if (++__curr_idx > 7) {
            __curr_idx = 0;
        }

        __state = 0;
    }

}


static inline doLeds(register const fix32 y)
{
    static fix32 prevY;
    static unsigned int __count;

    if ((prevY & 0x80000000) ^ (y & 0x80000000)) {

        if (++__count & BIT_3) {
            stepLeds();
            __count = 0;
        }

    }
    
    prevY = y;

}

/*
static inline doLeds()
{
    // setup
    DECLARATIONS();

    static int prevY;
    int y = 0;

    for (;;) {
        IDLE();

        prevY = y;
        y = inY;

        // sign difference
        if ((prevY & 0x80000000) ^ (y & 0x80000000)) {

            if (++__count & BIT_3) {
                stepLeds();
                __count = 0;
            }

        }

        outB = y;

        LOOP_END();
    }

    prevY = inY;
}
*/

#endif	/* LEDS_H */

