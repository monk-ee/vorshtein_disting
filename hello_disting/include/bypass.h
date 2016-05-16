#ifndef BYPASS_H
#define	BYPASS_H

#include "global.h"

void doBypass()
{
    // setup
    DECLARATIONS();

    for (;;) {
        // wait for new audio frame
        IDLE();

        int x = inX;
        int y = inY;

        // do the processing

        // write the outputs
        outA = x;
        outB = y;

        // loop end processing
        // (including reading the ADC channels)
        LOOP_END();

    }
}

#endif	/* BYPASS_H */

