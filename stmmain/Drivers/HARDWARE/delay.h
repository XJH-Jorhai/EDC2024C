#ifndef __DELAY_H
#define __DELAY_H
#include "main.h"
#include "tim.h"

#ifdef ARM_MATH_CM4
#include "arm_math.h"
#else
#include "math.h"
#endif

#define DELAY_TIMER_HANDLE  htim6
uint8_t delay_init(void);
void delay_us(uint32_t us);
void delay_ms(float ms);
#endif
