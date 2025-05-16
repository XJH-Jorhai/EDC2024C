/**********************************************************
                       ��������
										 
���ܣ�stm32f103rct6���ƣ�25MHzʱ�ӣ� AD9959���Ҳ���Ƶ�������Χ0-200M��
			��ʾ��12864cog
�ӿڣ����ƽӿ������AD9959.h  �����ӿ������key.h
ʱ�䣺2015/11/10
�汾��1.0
���ߣ���������
������

������������뵽�Ա��꣬�������ӽ߳�Ϊ������ ^_^
https://shop110336474.taobao.com/?spm=a230r.7195193.1997079397.2.Ic3MRJ

**********************************************************/
#include "stm32_config.h"
#include "stdio.h"
#include "led.h"
#include "lcd.h"
#include "AD9959.h"
#include "key.h"
#include "timer.h"
#include "task_manage.h"

extern u8 _return;
int main(void)
{
	
	MY_NVIC_PriorityGroup_Config(NVIC_PriorityGroup_2);	//�����жϷ���
	delay_init(72);	//��ʼ����ʱ����
	LED_Init();	//��ʼ��LED�ӿ�
	key_init();
	Init_AD9959();
	Timerx_Init(99,71);
	initial_lcd();
  delay_ms(300);
  
	while(1)
	{
		KeyRead();
		Set_PointFre(Keycode, 0);
		if(_return)
		{
			_return=0;
			LCD_Refresh_Gram();
		}
		KEY_EXIT();
	}	
}

