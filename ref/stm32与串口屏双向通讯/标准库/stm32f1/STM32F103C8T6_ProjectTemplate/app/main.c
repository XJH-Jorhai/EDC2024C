/*
 * �Ծ��۴���ͨѶ��ʾ����
 * �Ծ���wiki��ַ��http://wiki.tjc1688.com/
 * stm32��GND�Ӵ������򴮿ڹ��ߵ�GND,����
 * stm32��TX2(PA2)�Ӵ������򴮿ڹ��ߵ�RX
 * stm32��RX2(PA3)�Ӵ������򴮿ڹ��ߵ�TX
 * stm32��5V�Ӵ�������5V,����Ǵ��ڹ���,���ý�5VҲ����
 * 
 * ʹ�õ���Դ��
 * USART1(PA9,PA10):����ͨ��printfָ�������USART1����ӡ������Ϣ
 * USART2(PA2,PA3):����ͨ��TJCPrintfָ�������USART2���򴮿�����������
 * TIM2��ʱ��:���ڻ�ȡ��ǰ�ĺ�����,��now_time-last_time >= 1000Ҳ����1��ʱ,����Ļ����1������
 * LED(PC13):������ʾ�û���ǰ�Ƶ�״̬
 */
#include "stm32f10x.h"
#include "stdio.h"
#include "board.h"
#include "tjc_usart_hmi.h"
#include "led.h"
#include "timer2.h"


#define FRAME_LENGTH 7



int main(void)
{
	uint32_t now_time = 0;
	uint32_t last_time = 0;
	uint32_t  a = 0;
	//int a=0;
	board_init();
	led_init();
	uart1_init(115200);
	uart2_init(115200);
	initRingBuff();
	
  timer1_init();

	
	while(1)
	{

		get_time(&now_time);
		if(now_time-last_time >= 1000)
		{
			last_time = now_time;
			TJCPrintf("n0.val=%d",a);
			TJCPrintf("t0.txt=\"������%d\"",a);			
			//set_led_pc13(a % 2);  //��תLED
			TJCPrintf("click b0,1");
			delay_ms(50);
			TJCPrintf("click b0,0");
			a++; 

		}
		

	//stm32f103��GND�Ӵ������򴮿ڹ��ߵ�GND,����
	//stm32f103��TX1(PA9)�Ӵ������򴮿ڹ��ߵ�RX
	//stm32f103��RX1(PA10)�Ӵ������򴮿ڹ��ߵ�TX
	//stm32f103��5V�Ӵ�������5V,����Ǵ��ڹ���,���ý�5VҲ����

   //�������ݸ�ʽ��
   //��������֡���ȣ�7�ֽ�
   //֡ͷ     ����1    ����2   ����3       ֡β
   //0x55     1�ֽ�   1�ֽ�    1�ֽ�     0xffffff
   //��������01ʱ
   //֡ͷ     ����1    ����2   ����3       ֡β
   //0x55     01     led���  led״̬    0xffffff
   //����1����λ������  printh 55 01 01 00 ff ff ff  ���壺1��led�ر�
   //����2����λ������  printh 55 01 04 01 ff ff ff  ���壺4��led��
   //����3����λ������  printh 55 01 00 01 ff ff ff  ���壺0��led��
   //����4����λ������  printh 55 01 04 00 ff ff ff  ���壺4��led�ر�

   //��������02��03ʱ
   //֡ͷ     ����1    ����2   ����3       ֡β
   //0x55     02/03   ����ֵ    00    0xffffff
   //����1����λ������  printh 55 02 64 00 ff ff ff  ���壺h0.val=100
   //����2����λ������  printh 55 02 00 00 ff ff ff  ���壺h0.val=0
   //����3����λ������  printh 55 03 64 00 ff ff ff  ���壺h1.val=100
   //����4����λ������  printh 55 03 00 00 ff ff ff  ���壺h1.val=0

	  //�����ڻ��������ڵ���һ֡�ĳ���ʱ
	  while(usize >= FRAME_LENGTH)
	  {
		  //У��֡ͷ֡β�Ƿ�ƥ��
		  if(usize >= FRAME_LENGTH && u(0) == 0x55 && u(4) == 0xff && u(5) == 0xff && u(6) == 0xff)
		  {
				//ƥ�䣬���н���
				if(u(1) == 0x01)
				{
					TJCPrintf("msg.txt=\"led %d is %s\"", u(2), u(3) ? "on" : "off");
					if(u(2) == 0x00)
					{
						set_led_pc13(u(3));
					}
				}else if(u(1) == 0x02)
				{
					//�·�����h0����������Ϣ
					TJCPrintf("msg.txt=\"h0.val is %d\"", u(2));
					
				}else if(u(1) == 0x03)
				{
					//�·�����h1����������Ϣ
					TJCPrintf("msg.txt=\"h1.val is %d\"", u(2));
				}

				udelete(7);	//ɾ��������������
			  
		  }else
		  {
			  
				//��ƥ��ɾ��1�ֽ�
				udelete(1);
			  break;
		  }

	  }
	}
	
}
