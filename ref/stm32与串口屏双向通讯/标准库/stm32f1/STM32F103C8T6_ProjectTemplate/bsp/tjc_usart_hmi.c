/*

 */
 
#include "tjc_usart_hmi.h" 
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>

#define STR_LENGTH 100

typedef struct
{
	uint16_t Head;
	uint16_t Tail;
	uint16_t Lenght;
	uint8_t  Ring_data[RINGBUFF_LEN];
}RingBuff_t;

RingBuff_t ringBuff;	//����һ��ringBuff�Ļ�����


/********************************************************
��������  	uart1_init
���ߣ�    	
���ڣ�    	2024.08.18
���ܣ�    	��ʼ��uart1���ڴ�ӡ������Ϣ
���������	
����ֵ�� 		
�޸ļ�¼��
**********************************************************/
void uart1_init(uint32_t __Baud)
{
	GPIO_InitTypeDef GPIO_InitStructure;	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1,ENABLE);	
	
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin           = GPIO_Pin_9;//TX����
	GPIO_InitStructure.GPIO_Mode          = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed         = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin           = GPIO_Pin_10;//RX����
	GPIO_InitStructure.GPIO_Mode          = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
  
	USART_InitTypeDef USART_InitStructure;//�������ô��ڵĽṹ�����

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);//��������1��ʱ��

	USART_DeInit(USART1);//�����˼�ǽ���˴��ڵ���������

	USART_StructInit(&USART_InitStructure);
	USART_InitStructure.USART_BaudRate              = __Baud;//���ò�����
	USART_InitStructure.USART_WordLength            = USART_WordLength_8b;//�ֽڳ���Ϊ8bit
	USART_InitStructure.USART_StopBits              = USART_StopBits_1;//1��ֹͣλ
	USART_InitStructure.USART_Parity                = USART_Parity_No ;//û��У��λ
	USART_InitStructure.USART_Mode                  = USART_Mode_Rx | USART_Mode_Tx;//����������Ϊ�շ�ģʽ
	USART_InitStructure.USART_HardwareFlowControl   = USART_HardwareFlowControl_None; //���ṩ���� 
	USART_Init(USART1,&USART_InitStructure);//����ز�����ʼ��������1
	
	USART_ClearFlag(USART1,USART_FLAG_RXNE);//��ʼ����ʱ���������λ
	USART_ClearFlag(USART1,USART_FLAG_IDLE);//��ʼ����ʱ���������λ

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��ʼ���ý����ж�
	//USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);//��ʼ���ÿ����ж�

	USART_Cmd(USART1,ENABLE);//��������1
	
	NVIC_InitTypeDef NVIC_InitStructure;//�жϿ��ƽṹ���������

	NVIC_InitStructure.NVIC_IRQChannel                    = USART1_IRQn;//�ж�ͨ��ָ��ΪUSART1
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority  = 1;//�����ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority         = 1;//�����ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelCmd                 = ENABLE;//ȷ��ʹ��
	NVIC_Init(&NVIC_InitStructure);//��ʼ�����ô��ж�ͨ��
		
}



/********************************************************
��������  	uart2_init
���ߣ�    	
���ڣ�    	2024.08.18
���ܣ�    	��ʼ��uart2,���ڷ������ݸ�������
���������	
����ֵ�� 		
�޸ļ�¼��
**********************************************************/
void uart2_init(uint32_t __Baud)
{
	GPIO_InitTypeDef GPIO_InitStructure;	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA,ENABLE);	
	
	
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin           = GPIO_Pin_2;//TX����
	GPIO_InitStructure.GPIO_Mode          = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed         = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin           = GPIO_Pin_3;//RX����
	GPIO_InitStructure.GPIO_Mode          = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
  
	USART_InitTypeDef USART_InitStructure;//�������ô��ڵĽṹ�����

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);//��������1��ʱ��

	USART_DeInit(USART2);//�����˼�ǽ���˴��ڵ���������

	USART_StructInit(&USART_InitStructure);
	USART_InitStructure.USART_BaudRate              = __Baud;//���ò�����
	USART_InitStructure.USART_WordLength            = USART_WordLength_8b;//�ֽڳ���Ϊ8bit
	USART_InitStructure.USART_StopBits              = USART_StopBits_1;//1��ֹͣλ
	USART_InitStructure.USART_Parity                = USART_Parity_No ;//û��У��λ
	USART_InitStructure.USART_Mode                  = USART_Mode_Rx | USART_Mode_Tx;//����������Ϊ�շ�ģʽ
	USART_InitStructure.USART_HardwareFlowControl   = USART_HardwareFlowControl_None; //���ṩ���� 
	USART_Init(USART2,&USART_InitStructure);//����ز�����ʼ��������1
	
	USART_ClearFlag(USART2,USART_FLAG_RXNE);//��ʼ����ʱ���������λ
	USART_ClearFlag(USART2,USART_FLAG_IDLE);//��ʼ����ʱ���������λ

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//��ʼ���ý����ж�
	//USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);//��ʼ���ÿ����ж�

	USART_Cmd(USART2,ENABLE);//��������1
	
	NVIC_InitTypeDef NVIC_InitStructure;//�жϿ��ƽṹ���������

	NVIC_InitStructure.NVIC_IRQChannel                    = USART2_IRQn;//�ж�ͨ��ָ��ΪUSART1
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority  = 0;//�����ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority         = 1;//�����ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelCmd                 = ENABLE;//ȷ��ʹ��
	NVIC_Init(&NVIC_InitStructure);//��ʼ�����ô��ж�ͨ��
		
}

/********************************************************
��������  	TJC_SendData
���ߣ�    	
���ڣ�    	2024.08.18
���ܣ�    	�򴮿ڴ�ӡ����,��Ҫ�ƶ����͵��ַ�������
���������	
����ֵ�� 		
�޸ļ�¼��
**********************************************************/
void TJC_SendData(uint8_t *data, uint16_t len)
{
	while(len--)
	{
		//�ȴ��������ݼĴ���Ϊ��
		
		USART_SendData(USART2, *data++);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	}

}




/********************************************************
��������  	TJCPrintf
���ߣ�    	wwd
���ڣ�    	2022.10.08
���ܣ�    	�򴮿ڴ�ӡ����,ʹ��ǰ�뽫USART_SCREEN_write���������Ϊ��ĵ�Ƭ���Ĵ��ڷ��͵��ֽں���
���������	0�ο�printf
����ֵ�� 		��ӡ�����ڵ�����
�޸ļ�¼��
**********************************************************/

void TJCPrintf(const char *str, ...)
{


	uint8_t end = 0xff;
	char buffer[STR_LENGTH+1];  // ���ݳ���
	va_list arg_ptr;
	va_start(arg_ptr, str);
	int len = vsnprintf(buffer, STR_LENGTH+1, str, arg_ptr);
	va_end(arg_ptr);
	for(int i = 0; i < len; i++)
	{
		USART_SendData(USART2, buffer[i]);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);	//�ȴ��������
	}

	USART_SendData(USART2, end);			//���������Ϊ��ĵ�Ƭ���Ĵ��ڷ��͵��ֽں���
	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);	//�ȴ��������
	USART_SendData(USART2, end);			//���������Ϊ��ĵ�Ƭ���Ĵ��ڷ��͵��ֽں���
	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);	//�ȴ��������
	USART_SendData(USART2, end);			//���������Ϊ��ĵ�Ƭ���Ĵ��ڷ��͵��ֽں���
	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);	//�ȴ��������

}


/********************************************************
��������  	initRingBuff
���ߣ�
���ڣ�    	2022.10.08
���ܣ�    	��ʼ�����λ�����
���������
����ֵ�� 		void
�޸ļ�¼��
**********************************************************/
void initRingBuff(void)
{
  //��ʼ�������Ϣ
  ringBuff.Head = 0;
  ringBuff.Tail = 0;
  ringBuff.Lenght = 0;
}




/********************************************************
��������  	writeRingBuff
���ߣ�
���ڣ�    	2022.10.08
���ܣ�    	�����λ�����д������
���������
����ֵ�� 		void
�޸ļ�¼��
**********************************************************/
void writeRingBuff(uint8_t data)
{
  if(ringBuff.Lenght >= RINGBUFF_LEN) //�жϻ������Ƿ�����
  {
    return ;
  }
  ringBuff.Ring_data[ringBuff.Tail]=data;
  ringBuff.Tail = (ringBuff.Tail+1)%RINGBUFF_LEN;//��ֹԽ��Ƿ�����
  ringBuff.Lenght++;

}



/********************************************************
��������  	deleteRingBuff
���ߣ�
���ڣ�    	2022.10.08
���ܣ�    	ɾ�����ڻ���������Ӧ���ȵ�����
���������		Ҫɾ���ĳ���
����ֵ�� 		void
�޸ļ�¼��
**********************************************************/
void deleteRingBuff(uint16_t size)
{
	if(size >= ringBuff.Lenght)
	{
	    initRingBuff();
	    return;
	}
	for(int i = 0; i < size; i++)
	{

		if(ringBuff.Lenght == 0)//�жϷǿ�
		{
		initRingBuff();
		return;
		}
		ringBuff.Head = (ringBuff.Head+1)%RINGBUFF_LEN;//��ֹԽ��Ƿ�����
		ringBuff.Lenght--;

	}

}



/********************************************************
��������  	read1BFromRingBuff
���ߣ�
���ڣ�    	2022.10.08
���ܣ�    	�Ӵ��ڻ�������ȡ1�ֽ�����
���������		position:��ȡ��λ��
����ֵ�� 		����λ�õ�����(1�ֽ�)
�޸ļ�¼��
**********************************************************/
uint8_t read1BFromRingBuff(uint16_t position)
{
	uint16_t realPosition = (ringBuff.Head + position) % RINGBUFF_LEN;

	return ringBuff.Ring_data[realPosition];
}




/********************************************************
��������  	getRingBuffLenght
���ߣ�
���ڣ�    	2022.10.08
���ܣ�    	��ȡ���ڻ���������������
���������
����ֵ�� 		���ڻ���������������
�޸ļ�¼��
**********************************************************/
uint16_t getRingBuffLenght()
{
	return ringBuff.Lenght;
}


/********************************************************
��������  	isRingBuffOverflow
���ߣ�
���ڣ�    	2022.10.08
���ܣ�    	�жϻ��λ������Ƿ�����
���������
����ֵ�� 		1:���λ��������� , 2:���λ�����δ��
�޸ļ�¼��
**********************************************************/
uint8_t isRingBuffOverflow()
{
	return ringBuff.Lenght == RINGBUFF_LEN;
}


















#if !defined(__MICROLIB)
//��ʹ��΢��Ļ�����Ҫ�������ĺ���
#if (__ARMCLIB_VERSION <= 6000000)
//�����������AC5  �Ͷ�����������ṹ��
struct __FILE
{
	int handle;
};
#endif

FILE __stdout;

//����_sys_exit()�Ա���ʹ�ð�����ģʽ
void _sys_exit(int x)
{
	x = x;
}
#endif

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    USART_SendData(USART1, (uint8_t)ch);
	
	while( RESET == USART_GetFlagStatus(USART1, USART_FLAG_TXE) ){}
	
    return ch;
}



/******** ����2 �жϷ����� ***********/
void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET)//�ж��ǲ���������жϷ���
	{
		//USART_SendData(USART2,USART_ReceiveData(USART2));//�ֽ����ݷ���ȥ(������֤)

		writeRingBuff(USART_ReceiveData(USART2));
		USART_ClearITPendingBit(USART2, USART_IT_RXNE); //�Ѿ�����������־λ 
	}  
}


