#include "delay.h"
/*
初始延时函数对应的计时器
*/
uint8_t delay_init(void)
{
	return HAL_TIM_Base_Start(&DELAY_TIMER_HANDLE);
}


/*
延时us 微秒。
us小于10时延时时间会略长。
*/
void delay_us(uint32_t us)
{
	(&DELAY_TIMER_HANDLE)->Instance->CNT = (0);
	__HAL_TIM_ENABLE(&DELAY_TIMER_HANDLE);
	while((&DELAY_TIMER_HANDLE)->Instance->CNT < us)
	{
	}
	__HAL_TIM_DISABLE(&DELAY_TIMER_HANDLE);
}

/*
延时ms 毫秒。
*/
void delay_ms(float ms)
{
	ms=ms*1000;
	for(uint16_t i=(uint32_t)ms/40000;i>0;i--)
	{
		delay_us(40000);
	}
	delay_us((uint32_t)ms%40000);
}




