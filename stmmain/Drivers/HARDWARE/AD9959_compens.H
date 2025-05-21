#ifndef _AD9959_COMPENS_H_
#define _AD9959_COMPENS_H_

#include "main.h"
#include "math.h"

#define AD9959_COMP_MAX_F  2
#define AD9959_COMP_MAP_SIZE 100

#define AD9959_COMP_Q2F(q) ((((float)(q))/INT16_MAX)/AD9959_COMP_MAX_F)


/*
对于AD9959的定点数增益数组
步进1MHz/数据，从0开始
浮点数增益 ampF=AD9959_COMP_Q2F( 定点数增益q )
ampF在0-2之间
*/
extern const uint16_t AD9959_AMP_MAP[100];

float AD9959_GetMinAmp(uint16_t rng1, uint16_t rng2);
float AD9959_AmpComps(float freq);


#endif


