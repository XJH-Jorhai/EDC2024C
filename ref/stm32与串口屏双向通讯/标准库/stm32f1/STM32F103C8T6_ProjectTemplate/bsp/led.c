#include "led.h"


/********************************************************
��������  	led_init
���ߣ�
���ڣ�    	2024.08.21
���ܣ�    	��ʼ��led����
���������	
����ֵ�� 		
�޸ļ�¼��
**********************************************************/
void led_init()
{

	//��ʼ��С���ϵ�led(PC13)
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_LED_PC13, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_LED_PC13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(PORT_LED_PC13, &GPIO_InitStructure);
	GPIO_SetBits(PORT_LED_PC13,GPIO_InitStructure.GPIO_Pin);



	
}







