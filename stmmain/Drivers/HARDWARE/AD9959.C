#include "AD9959.H"


uint8_t CSR_DATA0[1] = {0x10};     // ¿ª CH0
uint8_t CSR_DATA1[1] = {0x20};      // ¿ª CH1
uint8_t CSR_DATA2[1] = {0x40};      // ¿ª CH2
uint8_t CSR_DATA3[1] = {0x80};      // ¿ª CH3		
																	
 
uint8_t FR2_DATA[2] = {0x20,0x00};//default Value = 0x0000
uint8_t CFR_DATA[3] = {0x00,0x03,0x02};//default Value = 0x000302	   
																	
uint8_t CPOW0_DATA[2] = {0x00,0x00};//default Value = 0x0000   @ = POW/2^14*360
																	


uint8_t LSRR_DATA[2] = {0x00,0x00};//default Value = 0x----
																	
uint8_t RDW_DATA[4] = {0x00,0x00,0x00,0x00};//default Value = 0x--------
																	
uint8_t FDW_DATA[4] = {0x00,0x00,0x00,0x00};//default Value = 0x--------



AD9959_HandleTypeDef hAD9959;

//AD9959³õÊ¼»¯
void Init_AD9959(void)  
{ 
	uint32_t SinFre[4]={1000000,1000000,1000000,1000000};
	float SinPhr[4]={0,0,0,0};
	uint16_t SinAmp[4]={1023,1023,1023,1023};
	for(uint16_t i=0;i<4;i++)
	{
		hAD9959.amp[i]=SinAmp[i];
		hAD9959.freq[i]=SinFre[i];
		hAD9959.phase[i]=SinPhr[i];
	}
	hAD9959.changeFlag=0;
	
	
	uint8_t FR1_DATA[3] = {0xD0,0x00,0x00};//20±¶Æµ Charge pump control = 75uA FR1<23> -- VCO gain control =0Ê± system clock below 160 MHz;
  IntReset();  //AD9959¸´Î»  
	
  WriteData_AD9959(FR1_ADD,3,FR1_DATA,1);//Ð´¹¦ÄÜ¼Ä´æÆ÷1
  WriteData_AD9959(FR2_ADD,2,FR2_DATA,1);
//  WriteData_AD9959(CFR_ADD,3,CFR_DATA,1);
//  WriteData_AD9959(CPOW0_ADD,2,CPOW0_DATA,0);
//  WriteData_AD9959(ACR_ADD,3,ACR_DATA,0);
//  WriteData_AD9959(LSRR_ADD,2,LSRR_DATA,0);
//  WriteData_AD9959(RDW_ADD,2,RDW_DATA,0);
//  WriteData_AD9959(FDW_ADD,4,FDW_DATA,1);
   //Ð´Èë³õÊ¼ÆµÂÊ
//	Write_frequence(3,SinFre[3]);
//	Write_frequence(0,SinFre[0]); 
//	Write_frequence(1,SinFre[1]);
//	Write_frequence(2,SinFre[2]);

//	Write_Phase(3, SinPhr[3]);
//	Write_Phase(0, SinPhr[0]);
//	Write_Phase(1, SinPhr[1]);
//	Write_Phase(2, SinPhr[2]);
//	
//	Write_Amplitude(3, SinAmp[3]);
//	Write_Amplitude(0, SinAmp[0]);
//	Write_Amplitude(1, SinAmp[1]);
//	Write_Amplitude(2, SinAmp[2]);
	
	
	AD9959_Set_Amp(&hAD9959,4,SinAmp);
	AD9959_Set_Freq(&hAD9959,4,SinFre);
	AD9959_Set_Phase(&hAD9959,4,SinPhr);
	
	AD9959_Apply_Change(&hAD9959);
	
	
	
} 
//ÑÓÊ±
void delay1 (uint32_t length)
{
	length = length*12;
   while(length--);
}

// AD9959 Reset
void IntReset(void)	  
{
  Reset(0);
  delay1(1);
  Reset(1);
  delay1(30);
  Reset(0);
}

// AD9959 Data Update
void AD9959_IO_Update(void)  
{
  UPDATE(0);
  delay1(2);
  UPDATE(1);
  delay1(4);
  UPDATE(0);
}

/*--------------------------------------------
Function: Write data to AD9959 via SPI
RegisterAddress: Register address
NumberofRegisters: Number of bytes
*RegisterData: Data pointer
temp: Whether to update IO registers
----------------------------------------------*/
void WriteData_AD9959(uint8_t RegisterAddress, uint8_t NumberofRegisters, uint8_t *RegisterData, uint8_t temp)
{
  uint8_t ControlValue = 0;
  uint8_t ValueToWrite = 0;
  uint8_t RegisterIndex = 0;
  uint8_t i = 0;

  ControlValue = RegisterAddress;
  
  // Write address
  SCLK(0);
  CS(0);	 
  for(i=0; i<8; i++)
  {
    SCLK(0);
    if(0x80 == (ControlValue & 0x80))
      SDIO0(1);	  
    else
      SDIO0(0);	  
    SCLK(1);
    ControlValue <<= 1;
  }
  SCLK(0);
  
  // Write data
  for (RegisterIndex=0; RegisterIndex<NumberofRegisters; RegisterIndex++)
  {
    ValueToWrite = RegisterData[RegisterIndex];
    for (i=0; i<8; i++)
    {
      SCLK(0);
      if(0x80 == (ValueToWrite & 0x80))
        SDIO0(1);	  
      else
        SDIO0(0);	  
      SCLK(1);
      ValueToWrite <<= 1;
    }
    SCLK(0);		
  }	
  
  if(temp != 0)
    AD9959_IO_Update();	
  CS(1);
}

/*---------------------------------------
Function: Set output frequency
Channel: Output channel (0-3)
Freq: Output frequency
---------------------------------------*/
#define AD9959_USE_SNAPPING
#ifdef AD9959_USE_SNAPPING
	#define AD9959_SNAPPING_GRID ((uint32_t)100000)
#endif

void Write_frequence(uint8_t Channel, uint32_t Freq)
{	 
	uint8_t CFTW0_DATA[4] = {0x00,0x00,0x00,0x00};
	uint32_t Temp;

	//å¸é™„
	//åº”è¯¥å¯ä»¥é¿å…AD9959_SNAPPING_GRIDæ•´æ•°å€ä¸Šçš„æ¼‚ç§»ï¼Œä½†æ˜¯ä¼šç•¥å¾®é™ä½Žå¯¹åº”ä½ç½®çš„ç²¾åº¦
	#ifdef AD9959_USE_SNAPPING
	if(Freq%AD9959_SNAPPING_GRID==0)
	{
		Temp=(((double)(((uint64_t)AD9959_SNAPPING_GRID)<<32))/(uint64_t)(500000000));
		Temp=Temp*(Freq/AD9959_SNAPPING_GRID);
	}
	else
	#endif
	{
		Temp = (((double)(((uint64_t)Freq)<<32))/(uint64_t)(500000000));  // Convert frequency to 32-bit tuning word
	}
	CFTW0_DATA[3] = (uint8_t)Temp;
	CFTW0_DATA[2] = (uint8_t)(Temp>>8);
	CFTW0_DATA[1] = (uint8_t)(Temp>>16);
	CFTW0_DATA[0] = (uint8_t)(Temp>>24);
  
	if(Channel == 0)	  
	{
		WriteData_AD9959(CSR_ADD, 1, CSR_DATA0, 1); // Configure channel 0
		WriteData_AD9959(CFTW0_ADD, 4, CFTW0_DATA, 1); // Set frequency
	}
	else if(Channel == 1)	
	{
		WriteData_AD9959(CSR_ADD, 1, CSR_DATA1, 1);
		WriteData_AD9959(CFTW0_ADD, 4, CFTW0_DATA, 1);
	}
	else if(Channel == 2)	
	{
		WriteData_AD9959(CSR_ADD, 1, CSR_DATA2, 1);
		WriteData_AD9959(CFTW0_ADD, 4, CFTW0_DATA, 1);
	}
	else if(Channel == 3)	
	{
		WriteData_AD9959(CSR_ADD, 1, CSR_DATA3, 1);
		WriteData_AD9959(CFTW0_ADD, 4, CFTW0_DATA, 1);
	}
}

/*---------------------------------------
Function: Set output amplitude
Channel: Output channel (0-3)
Ampli: Output amplitude
---------------------------------------*/
void Write_Amplitude(uint8_t Channel, uint16_t Ampli)
{ 
  uint16_t A_temp;
  uint8_t ACR_DATA[3] = {0x00,0x00,0x00};
  
  A_temp = Ampli | 0x1000;
  ACR_DATA[2] = (uint8_t)A_temp;
  ACR_DATA[1] = (uint8_t)(A_temp>>8);
  
  if(Channel == 0)
  {
    WriteData_AD9959(CSR_ADD, 1, CSR_DATA0, 1); 
    WriteData_AD9959(ACR_ADD, 3, ACR_DATA, 1); 
  }
  else if(Channel == 1)
  {
    WriteData_AD9959(CSR_ADD, 1, CSR_DATA1, 1); 
    WriteData_AD9959(ACR_ADD, 3, ACR_DATA, 1);
  }
  else if(Channel == 2)
  {
    WriteData_AD9959(CSR_ADD, 1, CSR_DATA2, 1); 
    WriteData_AD9959(ACR_ADD, 3, ACR_DATA, 1); 
  }
  else if(Channel == 3)
  {
    WriteData_AD9959(CSR_ADD, 1, CSR_DATA3, 1); 
    WriteData_AD9959(ACR_ADD, 3, ACR_DATA, 1); 
  }
}

/*---------------------------------------
Function: Set output phase
Channel: Output channel (0-3)
Phase: Output phase (0-16383 for 0°-360°)
---------------------------------------*/
void Write_Phase(uint8_t Channel, uint16_t Phase)
{
  uint16_t P_temp = 0;
  P_temp = (uint16_t)Phase;
  CPOW0_DATA[1] = (uint8_t)P_temp;
  CPOW0_DATA[0] = (uint8_t)(P_temp>>8);
  
  if(Channel == 0)
  {
    WriteData_AD9959(CSR_ADD, 1, CSR_DATA0, 1); 
    WriteData_AD9959(CPOW0_ADD, 2, CPOW0_DATA, 1);
  }
  else if(Channel == 1)
  {
    WriteData_AD9959(CSR_ADD, 1, CSR_DATA1, 1); 
    WriteData_AD9959(CPOW0_ADD, 2, CPOW0_DATA, 1);
  }
  else if(Channel == 2)
  {
    WriteData_AD9959(CSR_ADD, 1, CSR_DATA2, 1); 
    WriteData_AD9959(CPOW0_ADD, 2, CPOW0_DATA, 1);
  }
  else if(Channel == 3)
  {
    WriteData_AD9959(CSR_ADD, 1, CSR_DATA3, 1); 
    WriteData_AD9959(CPOW0_ADD, 2, CPOW0_DATA, 1);
  }
}


/*---------------------------------------
Function: Set output phase in struct had9959.
Channel: Output channel (0-3) or All channel (>=4)
Data: pointer to data (channel<4) or pointer to data array (channel>=4), float
---------------------------------------*/
uint8_t AD9959_Set_Phase(AD9959_HandleTypeDef* had9959, uint16_t channel, float* data)
{
	if(channel<4)
	{
		float buf=*data;
		while(buf<0)
		{
			buf+=360.0;
		}
		buf/=360.0;
		had9959->phase[channel]=((uint16_t)(buf*16383))%16383;
	}
	else
	{
		float buf;
		for(uint8_t i=0;i<4;i++)
		{
			buf=data[i];
			while(buf<0)
			{
				buf+=360.0;
			}
			buf/=360.0;
			had9959->phase[i]=(uint16_t)(buf*16383);
		}
	}
	had9959->changeFlag=1;
	return HAL_OK;
}

uint8_t AD9959_Set_Freq(AD9959_HandleTypeDef* had9959, uint16_t channel, uint32_t* data)
{
	if(channel<4)
	{
		had9959->freq[channel]=*data;
	}
	else
	{
		had9959->freq[0]=data[0];
		had9959->freq[1]=data[1];
		had9959->freq[2]=data[2];
		had9959->freq[3]=data[3];
	}
	had9959->changeFlag=1;
	return HAL_OK;
}

uint8_t AD9959_Set_Amp(AD9959_HandleTypeDef* had9959, uint16_t channel, uint16_t* data)
{
	uint16_t maxAmp=1023;
	if(channel<4)
	{
		had9959->amp[channel]=data[0]<=maxAmp? data[0]: maxAmp;
	}
	else
	{
		had9959->amp[0]=data[0]<=maxAmp? data[0]: maxAmp;
		had9959->amp[1]=data[1]<=maxAmp? data[1]: maxAmp;
		had9959->amp[2]=data[2]<=maxAmp? data[2]: maxAmp;
		had9959->amp[3]=data[3]<=maxAmp? data[3]: maxAmp;
	}
	had9959->changeFlag=1;
	return HAL_OK;
}


uint8_t AD9959_Apply_Change(AD9959_HandleTypeDef* had9959)
{
	if(had9959->changeFlag==1)
	{
		syslog("Change Applied");
		for(uint16_t i=0;i<4;i++)
		{
			Write_frequence(i,had9959->freq[i]);
			Write_Phase(i,had9959->phase[i]);
			Write_Amplitude(i,had9959->amp[i]);
		}
	}

	had9959->changeFlag=0;
	return HAL_OK;
}






