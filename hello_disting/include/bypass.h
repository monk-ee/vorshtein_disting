#ifndef BYPASS_H
#define	BYPASS_H

#include "global.h"

static void doBypass()
{
    // setup
    DECLARATIONS();

    for (;;) {
        // wait for new audio frame
        IDLE();

        int x = inL;
        int y = inR;

        // do the processing

        // write the outputs
        outL = x;
        outR = y;

        // loop end processing
        // (including reading the ADC channels)
        LOOP_END();

    }
}

#endif	/* BYPASS_H */

