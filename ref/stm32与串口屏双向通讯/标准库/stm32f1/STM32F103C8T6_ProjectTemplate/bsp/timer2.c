#include "timer2.h"

static uint32_t time_count=0;

void timer1_init()
{

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		
	/* ���ö�ʱ������ */
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 1000 - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1; // TIMER2ʱ��Ƶ��72MHz
	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); // ʹ��TIM2����
	
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; // ѡ��TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
	
	NVIC_Init(&NVIC_InitStructure); // ʹ��NVIC����

	TIM_ITConfig(TIM2,TIM_IT_Update, ENABLE); // ʹ��TIM2�ж�
	
	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIM2
	
}

void TIM2_IRQHandler(void)
{
		
		if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) // {}��Ϊ�жϴ���
		{
			time_count++;
		}
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}


void get_time(uint32_t *__time)
{
	*__time = time_count;
}
