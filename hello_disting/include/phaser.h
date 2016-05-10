#ifndef ALLPASS_H_INCLUDED
#define ALLPASS_H_INCLUDED

#include "global.h"
#include "table.h"

#define PHASER_NUM_NOTCHES (7)
#define PHASER_NUM_FILTERS (14)

//  4,194,304

//#define DISTING_ADC_MAX_HALF ((1 << (DISTING_ADC_RESOLUTION - 2)) - 1)

//#define TWO_ADC_MAX (2 * DISTING_ADC_MAX_HALF)
// 157..934
#define POTMIN (157)
#define POTMAX (934)

#define PHASER_FACTOR_0250 (268435456)
#define PHASER_FEEDBACK_0000 (0)
#define PHASER_FEEDBACK_1000 (1073741824)
#define PHASER_FEEDBACK_2000 (2147483647)
#define PHASER_FEEDBACK_4000 (2147483647)

typedef struct
{
    fix32 x_prev;
    fix32 y;
} allpass_filter_t;

static allpass_filter_t __filter[PHASER_NUM_FILTERS];
static fix32 __coeff[PHASER_NUM_NOTCHES];

static inline void UpdateLookupIndices()
{
#define __PHASER_IN_MIN__ (1 << (PHASER_NUM_NOTCHES + 1))
#define __DISTING_ADC_MAX_HALF__ (4194303)
#define __DISTING_ADC_MAX__ (8388606)

    register fix32 in = inL += __DISTING_ADC_MAX_HALF__;
    
    if (in > __DISTING_ADC_MAX__) {
        in = __DISTING_ADC_MAX__;
    }
    else if (in < __PHASER_IN_MIN__) {
        in = __PHASER_IN_MIN__;
    }

    register fix32 __pot = (pot + POTMAX) << 19;
    //pot <<= (31 - 10 - 2);

    register fix32* p_coeff; // = __coeff;
    char i;
    for (i = 0, p_coeff = __coeff; i < PHASER_NUM_NOTCHES; ++i, ++p_coeff) {
        in = multfix32(in, __pot);
        *p_coeff = in;
    }
}

static fix32 ComputeLookupValue(const char coeff_index)
{
#define __LOOKUP_SHIFT_OPERAND__ ((DISTING_ADC_RESOLUTION - AP_LOOKUP_EXP - 1))
    
    register const fix32 pos_index = *(__coeff + coeff_index);

    //register const fix32 base_index = pos_index >> (DISTING_ADC_RESOLUTION - AP_LOOKUP_EXP - 1);
    register const fix32 base_index = pos_index >> __LOOKUP_SHIFT_OPERAND__;

    // difference between pos_index and base_index shifted up to the original magnitude
    //register const fix32 d = (pos_index - (base_index << (DISTING_ADC_RESOLUTION - AP_LOOKUP_EXP - 1))) << (31 - (DISTING_ADC_RESOLUTION - AP_LOOKUP_EXP + 0));
    register const fix32 d = (pos_index - (base_index << __LOOKUP_SHIFT_OPERAND__)) << (31 - (__LOOKUP_SHIFT_OPERAND__ + 1));

    register const fix32* base = __tan_table + base_index;
    return multfix32(d, *(base + 1) - *base) + *base;
}

static inline fix32 AllPassNextNext(register const fix32 filter_idx,
                                    register fix32 x,
                                    register const fix32 coeff)
{
    register allpass_filter_t* ap = __filter + filter_idx;
    register fix32 y = ap->y;

    y += x;
    y = multfix32(y, coeff);
    y -= ap->x_prev;
    ap->y = y;
    ap->x_prev = x;

    //    return y;
    ++ap;

    x = y;
    y = ap->y + x;
    y = multfix32(y, coeff);
    y -= ap->x_prev;
    ap->y = y;
    ap->x_prev = x;

    return y;
}

static void AllPassInit()
{
    int i;
    for (i = 0; i < PHASER_NUM_FILTERS; ++i) {
        __filter[i].x_prev = 0;
        __filter[i].y = 0;
    }
}


#endif // ALLPASS_H_INCLUDED