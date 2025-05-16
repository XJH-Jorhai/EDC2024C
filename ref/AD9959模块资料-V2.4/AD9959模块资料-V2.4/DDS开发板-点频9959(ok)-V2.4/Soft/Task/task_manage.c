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

#include "task_manage.h"
#include "delay.h"
#include "key.h"
#include "AD9959.h"
////#include "AD9954.h" 
#include <stdio.h>
#include <ctype.h>
#include <cstring>

#define OUT_KEY  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)//��ȡ����0
#define FLASH_SAVE_ADDR  0x0801F000  				//����FLASH �����ַ(����Ϊż��)

u8 Task_Index = 0;//�����л�
u8 Param_Mode = 0;//���������л�
u8 fre_buf[StrMax]; //����ת���ַ�����
u8 display[StrMax]; //������ʾ����
u8 P_Index = 0; //�༭λ��
u8 Task_First=1;//��һ�ν�����
u8 _return=0;
u32 SinFre[5] = {50, 50, 50, 50};
u32 SinAmp[5] = {1023, 1023, 1023, 1023};
u32 SinPhr[5] = {0, 4095, 4095*3, 4095*2};
//ɨƵ����
u32 SweepMinFre = 1000;
u32 SweepMaxFre = 10000;
u32 SweepStepFre = 100;
u16 SweepTime = 1;//ms
u8 SweepFlag = 0;

//�����ݷŵ���һ�����棬��ʾ��FloatNumС����λ����CursorEn���ʹ��
void Copybuf2dis(u8 *source, u8 dis[StrMax], u8 dispoint, u8 FloatNum, u8 CursorEn)
{
	int i, len;
	
	
	len = strlen(source);
	i = len - FloatNum;//��������
	Clear_AllStr(dis);
	if(FloatNum>0)dis[i] = '.';
	for (i = 0; i < len; i++)
	{
		if(i < (len-FloatNum)) dis[i] = source[i];
		else 
		{ dis[i+1] = source[i]; }
	}
	
	if(CursorEn)
	{
		if(dispoint < (len-FloatNum)) dis[dispoint] += 128;
		else dis[dispoint+1] += 128;	
	}
}
//
void Set_PointFre(u32 Key_Value, u8* Task_ID)
{
	//�����ж�
	switch(Key_Value)
	{
		case K_4_S: fre_buf[P_Index]++;break;
		case K_4_L: fre_buf[P_Index]++;break;
		case K_5_L: P_Index++;break;
		case K_5_S: P_Index++;break;
		case K_1_L: P_Index--;break;
		case K_1_S: P_Index--;break;
		case K_3_S: fre_buf[P_Index]--;break;
		case K_3_L: fre_buf[P_Index]--;break;
		case K_2_S: Param_Mode++;break;
	}
	if(fre_buf[P_Index] == '/') fre_buf[P_Index] = '9';//<'0'
	if(fre_buf[P_Index] == ':') fre_buf[P_Index] = '0';//>'9'
	//�����л�
	if(Key_Value == K_2_L) 
	{	
		Task_Index++;
		P_Index = 0;
		Param_Mode = 0;
		LCD_Clear();
	}
	if(Key_Value == K_2_L || Key_Value == K_2_S) Task_First=1;//���µ�һ�ν���
	if(Task_Index >= Interface) Task_Index=0;
	switch(Task_Index)
	{
		case 0: 
			Task0_PointFre(Key_Value);
			break;
		case 1: 
			Task1_Amp_Pha(Key_Value);
			break;
		case 2: 
			Task3_SweepFre(Key_Value);
			break;
	}
	//��������
	if(Task_Index != 2) SweepFlag=0;//
	else SweepFlag=1;
	//�̶���ʾ
	LCD_Show_CEStr(0,0,"9959");
}
//����

void Task0_PointFre(u32 Key_Value)//���Ҳ� (10M) 0-100 000 000
{
	u8 CFR2_DATA[2] = {0x20,0x00};
	
	u8 Showchar[StrMax];
	Param_Mode = Param_Mode%4;//����λ
	if(Task_First)
	{
		Task_First = 0;
		Key_Value = K_2_S;
		WriteData_AD9959(FR2_ADD,2,CFR2_DATA,1);
		if(Param_Mode == 0)
		sprintf(fre_buf, "%9d", SinFre[0]);//��һ�� ����
		if(Param_Mode == 1)
		sprintf(fre_buf, "%9d", SinFre[1]);//��һ�� ����
		if(Param_Mode == 2)
		sprintf(fre_buf, "%9d", SinFre[2]);//��һ�� ����
		if(Param_Mode == 3)
		sprintf(fre_buf, "%9d", SinFre[3]);//��һ�� ����
		LCD_Show_CEStr(64-8*2,0," ��Ƶ");
		
		LCD_ShowNum(1,4*8,1,SinFre[0], 12, 9);
		LCD_ShowString(1,0,1,"CH0:", 12);
		LCD_ShowString(1,14*8,1,"Hz", 12);
		
		LCD_ShowNum(1,4*8,2,SinFre[1], 12, 9);
		LCD_ShowString(1,0,2,"CH1:", 12);
		LCD_ShowString(1,14*8,2,"Hz", 12);
		
		LCD_ShowNum(1,4*8,3,SinFre[2], 12, 9);
		LCD_ShowString(1,0,3,"CH2:", 12);
		LCD_ShowString(1,14*8,3,"Hz", 12);
		
		LCD_ShowNum(1,4*8,4,SinFre[3], 12, 9);
		LCD_ShowString(1,0,4,"CH3:", 12);
		LCD_ShowString(1,14*8,4,"Hz", 12);
		_return=1;
	}
	if(Key_Value != K_NO)
	{
		//�ж�
		if(P_Index == 255) P_Index = 8;
		P_Index = P_Index%9;//����λ��
		SinFre[4] = atol(fre_buf);//�ַ�ת�����֣��ж�������
		if(SinFre[4]>200000000) 
		{
			SinFre[4]=200000000;//��������
			sprintf(fre_buf, "%9d", SinFre[4]);//�ַ�ת��
		}
		if(SinFre[4]<0) 
		{
			SinFre[4]=0;
			sprintf(fre_buf, "%9d", SinFre[4]);//�ַ�ת��
		}
		switch(Param_Mode)
		{
			case 0:
				SinFre[0] = atol(fre_buf);
				fre_buf_change(fre_buf);//fre_buf���� �� '->'0'
				Copybuf2dis(fre_buf, display, P_Index, 0, 1);				
				OLED_ShowString(4*8, 1, display,12, 5);
				break;
			case 1: 
				SinFre[1] = atol(fre_buf);//�ַ�ת�����֣��ж�������
				fre_buf_change(fre_buf);//fre_buf���� �� '->'0'
				Copybuf2dis(fre_buf, display, P_Index, 0, 1);				
				OLED_ShowString(4*8, 2, display,12, 5);
				break;
			case 2: 
				SinFre[2] = atol(fre_buf);//�ַ�ת�����֣��ж�������
				fre_buf_change(fre_buf);//fre_buf���� �� '->'0'
				Copybuf2dis(fre_buf, display, P_Index, 0, 1);			
				OLED_ShowString(4*8, 3, display,12, 5);
				break;
			case 3: 
				SinFre[3] = atol(fre_buf);//�ַ�ת�����֣��ж�������
				fre_buf_change(fre_buf);//fre_buf���� �� '->'0'
				Copybuf2dis(fre_buf, display, P_Index, 0, 1);				
				OLED_ShowString(4*8, 4, display,12, 5);
				break;
		}
		//��������д��
		Write_frequence(3,SinFre[3]);
		Write_frequence(0,SinFre[0]); 
		Write_frequence(1,SinFre[1]);
		Write_frequence(2,SinFre[2]);
		
		Write_Phase(3, SinPhr[3]);
		Write_Phase(0, SinPhr[0]);
		Write_Phase(1, SinPhr[1]);
		Write_Phase(2, SinPhr[2]);
////		AD9954_SETFRE(SinFre);
		
		_return=1;
	}
}

void Task1_Amp_Pha(u32 Key_Value)
{
	u8 CFR2_DATA[2] = {0x20,0x00};
	
		
	Param_Mode = Param_Mode%8;//����λ
	if(Task_First)
	{
		Task_First = 0;
		Key_Value = K_2_S;
		WriteData_AD9959(FR2_ADD,2,CFR2_DATA,1);
		if(Param_Mode == 0)
		sprintf(fre_buf, "%4d", SinAmp[0]);//��һ�� ����
		if(Param_Mode == 1)
		sprintf(fre_buf, "%5d", SinPhr[0]);//��һ�� ����
		if(Param_Mode == 2)
		sprintf(fre_buf, "%4d", SinAmp[1]);//��һ�� ����
		if(Param_Mode == 3)
		sprintf(fre_buf, "%5d", SinPhr[1]);//��һ�� ����
		if(Param_Mode == 4)
		sprintf(fre_buf, "%4d", SinAmp[2]);//��һ�� ����
		if(Param_Mode == 5)
		sprintf(fre_buf, "%5d", SinPhr[2]);//��һ�� ����
		if(Param_Mode == 6)
		sprintf(fre_buf, "%4d", SinAmp[3]);//��һ�� ����
		if(Param_Mode == 7)
		sprintf(fre_buf, "%5d", SinPhr[3]);//��һ�� ����
		LCD_Show_CEStr(30,0," �������� ");//ģʽ����
		
		LCD_ShowString(1,0,1,"CH0:", 12);
		LCD_ShowNum(1,3*8,1,SinAmp[0], 12, 4);
		LCD_ShowString(1,7*8,1,"A", 12);
		LCD_ShowNum(1,10*8,1,SinPhr[0], 12, 5);
		LCD_ShowString(1,15*8,1,"P", 12);
		
		LCD_ShowString(1,0,2,"CH1:", 12);
		LCD_ShowNum(1,3*8,2,SinAmp[1], 12, 4);
		LCD_ShowString(1,7*8,2,"A", 12);
		LCD_ShowNum(1,10*8,2,SinPhr[1], 12, 5);
		LCD_ShowString(1,15*8,2,"P", 12);
		
		LCD_ShowString(1,0,3,"CH2:", 12);
		LCD_ShowNum(1,3*8,3,SinAmp[2], 12, 4);
		LCD_ShowString(1,7*8,3,"A", 12);
		LCD_ShowNum(1,10*8,3,SinPhr[2], 12, 5);
		LCD_ShowString(1,15*8,3,"P", 12);
		
		LCD_ShowString(1,0,4,"CH3:", 12);
		LCD_ShowNum(1,3*8,4,SinAmp[3], 12, 4);
		LCD_ShowString(1,7*8,4,"A", 12);
		LCD_ShowNum(1,10*8,4,SinPhr[3], 12, 5);
		LCD_ShowString(1,15*8,4,"P", 12);

		_return=1;
	}
	if(Key_Value != K_NO)
	{
		//�ж�
		if(Param_Mode % 2 == 0)
		{
			P_Index = P_Index%4;//����λ��
			SinAmp[4] = atol(fre_buf);//�ַ�ת�����֣��ж�������
			if(SinAmp[4]>=1024) 
			{
				SinAmp[4]=1023;//��������
				sprintf(fre_buf, "%4d", SinAmp[4]);//�ַ�ת��
			}
			if(SinAmp[4]<0) 
			{
				SinAmp[4]=0;
				sprintf(fre_buf, "%4d", SinAmp[4]);//�ַ�ת��
			}
		}
		else
		{
			P_Index = P_Index%5;//����λ��
			SinPhr[4] = atol(fre_buf);//�ַ�ת�����֣��ж�������
			if(SinPhr[4]>=16384) 
			{
				SinPhr[4]=16383;//��������
				sprintf(fre_buf, "%5d", SinPhr[4]);//�ַ�ת��
			}
			if(SinPhr[4]<0) 
			{
				SinPhr[4]=0;
				sprintf(fre_buf, "%5d", SinPhr[4]);//�ַ�ת��
			}
		}
		switch(Param_Mode)
		{
			case 0:
				SinAmp[0] = atol(fre_buf);
				fre_buf_change(fre_buf);//fre_buf���� �� '->'0'
				Copybuf2dis(fre_buf, display, P_Index, 0, 1);				
				OLED_ShowString(3*8, 1, display,12, 5);
				break;
			case 1: 
				SinPhr[0] = atol(fre_buf);//�ַ�ת�����֣��ж�������
				fre_buf_change(fre_buf);//fre_buf���� �� '->'0'
				Copybuf2dis(fre_buf, display, P_Index, 0, 1);				
				OLED_ShowString(10*8, 1, display,12, 5);
				break;
			case 2: 
				SinAmp[1] = atol(fre_buf);//�ַ�ת�����֣��ж�������
				fre_buf_change(fre_buf);//fre_buf���� �� '->'0'
				Copybuf2dis(fre_buf, display, P_Index, 0, 1);				
				OLED_ShowString(3*8, 2, display,12, 5);
				break;
			case 3: 
				SinPhr[1] = atol(fre_buf);//�ַ�ת�����֣��ж�������
				fre_buf_change(fre_buf);//fre_buf���� �� '->'0'
				Copybuf2dis(fre_buf, display, P_Index, 0, 1);				
				OLED_ShowString(10*8, 2, display,12, 5);
				break;
			case 4: 
				SinAmp[2] = atol(fre_buf);//�ַ�ת�����֣��ж�������
				fre_buf_change(fre_buf);//fre_buf���� �� '->'0'
				Copybuf2dis(fre_buf, display, P_Index, 0, 1);				
				OLED_ShowString(3*8, 3, display,12, 5);
				break;
			case 5: 
				SinPhr[2] = atol(fre_buf);//�ַ�ת�����֣��ж�������
				fre_buf_change(fre_buf);//fre_buf���� �� '->'0'
				Copybuf2dis(fre_buf, display, P_Index, 0, 1);				
				OLED_ShowString(10*8, 3, display,12, 5);
				break;
			case 6: 
				SinAmp[3] = atol(fre_buf);//�ַ�ת�����֣��ж�������
				fre_buf_change(fre_buf);//fre_buf���� �� '->'0'
				Copybuf2dis(fre_buf, display, P_Index, 0, 1);				
				OLED_ShowString(3*8, 4, display,12, 5);
				break;
			case 7: 
				SinPhr[3] = atol(fre_buf);//�ַ�ת�����֣��ж�������
				fre_buf_change(fre_buf);//fre_buf���� �� '->'0'
				Copybuf2dis(fre_buf, display, P_Index, 0, 1);				
				OLED_ShowString(10*8, 4, display,12, 5);
				break;
		}
			//��������д��
		Write_Amplitude(3, SinAmp[3]);
		Write_Amplitude(0, SinAmp[0]);
		Write_Amplitude(1, SinAmp[1]);
		Write_Amplitude(2, SinAmp[2]);
		
	
		Write_Phase(3, SinPhr[3]);
		Write_Phase(0, SinPhr[0]);
		Write_Phase(1, SinPhr[1]);
		Write_Phase(2, SinPhr[2]);
	}

		
		
	_return=1;
}
void Task2_Triangular(u32 Key_Value)
{}

void Task3_SweepFre(u32 Key_Value)//ɨƵ
{
	u8 CFR2_DATA[2] = {0x00,0x00};//default Value = 0x0000
	u8 showstr[StrMax]={0};
	
	if(Task_First)
	{
		Task_First = 0;//�����һ�ν�����
		Key_Value = K_2_S;
		WriteData_AD9959(FR2_ADD,2,CFR2_DATA,1);
		Param_Mode %= 4;//�������ģʽ���Ա�װ���ʼֵ
		if(Param_Mode == 0) sprintf(fre_buf, "%9d", SweepMinFre);
		if(Param_Mode == 1) sprintf(fre_buf, "%9d", SweepMaxFre);
		if(Param_Mode == 2) sprintf(fre_buf, "%4d", SweepStepFre);
		if(Param_Mode == 3) sprintf(fre_buf, "%3d", SweepTime);
		LCD_Show_CEStr(64-8*3,0," ɨƵ ");//ģʽ����
		_return=1;//������ʾ��־ 
	}
	if(Key_Value != K_NO)
	{
		if(Param_Mode == 0)//��СƵ������
		{
			P_Index %= 9;//����λ��
			SweepMinFre = atol(fre_buf);//�ַ�ת��
			if(SweepMinFre>SweepMaxFre) SweepMinFre=SweepMaxFre;//��������
			if(SweepMinFre<0) SweepMinFre=0;
			sprintf(fre_buf, "%9d", SweepMinFre);//��������д��
		}
		//��ʾ
		sprintf(showstr, "%9d", SweepMinFre);//�������뻺����ʾ
		fre_buf_change(showstr);//fre_buf���� �� '->'0'
		if(Param_Mode == 0) Copybuf2dis(showstr, display, P_Index, 0, 1);
		else Copybuf2dis(showstr, display, P_Index, 0, 0);
		OLED_ShowString(64-4*11+16, 1, display, 16, 0);
		LCD_Show_CEStr(64-4*11+9*8+16,2,"Hz");
		LCD_Show_CEStr(0,2,"Min:");
		
		if(Param_Mode == 1)//���Ƶ������
		{
			P_Index %= 9;//����λ��
			SweepMaxFre = atol(fre_buf);//�ַ�ת��
			if(SweepMaxFre>200000000) SweepMaxFre=200000000;//��������
			if(SweepMaxFre<0) SweepMaxFre=0;
			sprintf(fre_buf, "%9d", SweepMaxFre);//��������д��
		}
		//��ʾ
		sprintf(showstr, "%9d", SweepMaxFre);//�������뻺����ʾ
		fre_buf_change(showstr);//fre_buf���� �� '->'0'
		if(Param_Mode == 1) Copybuf2dis(showstr, display, P_Index, 0, 1);
		else Copybuf2dis(showstr, display, P_Index, 0, 0);
		OLED_ShowString(64-4*11+16, 2, display,16, 0);
		LCD_Show_CEStr(64-4*11+9*8+16,4,"Hz");
		LCD_Show_CEStr(0,4,"Max:");
		
		if(Param_Mode == 2)//Ƶ�ʲ�������
		{
			P_Index %= 5;//����λ��
			SweepStepFre = atol(fre_buf);//�ַ�ת��
			if(SweepStepFre>10000) SweepStepFre=10000-1;//��������
			if(SweepStepFre<0) SweepStepFre=0;
			sprintf(fre_buf, "%5d", SweepStepFre);//��������д��
		}
		//��ʾ
		sprintf(showstr, "%5d", SweepStepFre);//�������뻺����ʾ
		fre_buf_change(showstr);//fre_buf���� �� '->'0'
		if(Param_Mode == 2) Copybuf2dis(showstr, display, P_Index, 0, 1);
		else Copybuf2dis(showstr, display, P_Index, 0, 0);
		display[5]=0;//ֻ��ʾ4λ
		OLED_ShowString(0, 3, display,16, 0);
		LCD_Show_CEStr(8*5,6,"Hz");
		
		if(Param_Mode == 3)//����ʱ������
		{
			P_Index %= 3;//����λ��
			SweepTime = atol(fre_buf);//�ַ�ת��
			if(SweepTime>255) SweepTime=255;//��������
			if(SweepTime<0) SweepTime=0;
			sprintf(fre_buf, "%3d", SweepTime);//��������д��
		}
		//��ʾ
		sprintf(showstr, "%3d", SweepTime);//�������뻺����ʾ
		fre_buf_change(showstr);//fre_buf���� �� '->'0'
		if(Param_Mode == 3) Copybuf2dis(showstr, display, P_Index, 0, 1);
		else Copybuf2dis(showstr, display, P_Index, 0, 0);
		display[3]=0;//ֻ��ʾ3λ
		OLED_ShowString(64, 3, display,16, 0);
		LCD_Show_CEStr(64+3*8,6,"Ms");
		//���ݴ���д��
//		Linear_Sweep(SweepMinFre, SweepMaxFre, SweepStepFre,  SweepTime, SweepStepFre, SweepTime, 2);
		
		_return=1;
	}
}

//
void fre_buf_change(u8 *strbuf)
{
	int i;
	for (i = 0 ; i < strlen(strbuf); i++)
	 if(strbuf[i]==0x20) strbuf[i] = '0';
	for (i = 0 ; i < strlen(fre_buf); i++)
	 if(fre_buf[i]==0x20) fre_buf[i] = '0';
}
void Clear_AllStr(u8 *str)
{
	u8 i, len;
	len = strlen(str);
	for(i = 0; i < len; i++)
	str[i] = NULL;
}

