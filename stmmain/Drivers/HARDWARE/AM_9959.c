
#include "AM_9959.h"


AM_Instance AM1,AM2;
/*
初始化AM实例
*/
void AM_Instance_Init(AM_Instance* ham, AD9959_HandleTypeDef* had9959, uint16_t chcw, uint16_t chmw, DAC_HandleTypeDef* hdacx, uint32_t dac_channel)
{
	ham->CH_CW=chcw;
	ham->CH_MW=chmw;
	ham->MDepth=0.5;
	ham->TDelay=0;
	ham->had9959=had9959;
	ham->hdac=hdacx;
	ham->CH_DAC=dac_channel;
	ham->changeflag=0;
	ham->min_amp=AD9959_GetMinAmp(0,40);
}

/*
初始化本题中用到的AM
*/
uint8_t AM_Init(void)
{
	HAL_DAC_Init(&hdac);
	HAL_DACEx_DualSetValue(&AM_DAC_HANDLE,0,0,0);
	HAL_DACEx_DualStart(&AM_DAC_HANDLE);
	AM_Instance_Init(&AM1,&hAD9959,0,1,&hdac,DAC_CHANNEL_1);
	AM_Instance_Init(&AM2,&hAD9959,2,3,&hdac,DAC_CHANNEL_2);
	
	
	
	
	return HAL_OK;

}


/*
同步并应用由AM_SetXXX产生的更改。使用同一个AD9959实例的AM示例将会在同步被设置。
hmax: AM_Instance指针数组
cnt:   数组长度
*/
uint8_t AM_ApplyChanges(AM_Instance* hamx[], uint16_t cnt)
{
	uint16_t data16;
	uint32_t data32;
	float datafloat;
	
	for(uint16_t i=0;i<cnt;i++)
	{
		
		datafloat=((double)(hamx[i]->had9959->freq[hamx[i]->CH_MW])*360*(hamx[i]->TDelay)/1000000000);
		AD9959_Set_Phase(hamx[i]->had9959,hamx[i]->CH_MW,&datafloat);

		datafloat=((double)(hamx[i]->had9959->freq[hamx[i]->CH_CW])*360*(hamx[i]->TDelay)/1000000000);
		AD9959_Set_Phase(hamx[i]->had9959,hamx[i]->CH_CW,&datafloat);
		
				
		data32=2000000;
		AD9959_Set_Freq(hamx[i]->had9959, hamx[i]->CH_MW, &data32);
		
		
		datafloat=(hamx[i]->min_amp*1023)*AD9959_AmpComps(hamx[i]->had9959->freq[hamx[i]->CH_CW]);
		data16=(uint16_t)datafloat;
		
		AD9959_Set_Amp(hamx[i]->had9959, hamx[i]->CH_CW, &data16);
		
		datafloat=(hamx[i]->min_amp*1023)*AD9959_AmpComps(hamx[i]->had9959->freq[hamx[i]->CH_MW])*(hamx[i]->MDepth);
		data16=(uint16_t)datafloat;

		AD9959_Set_Amp(hamx[i]->had9959, hamx[i]->CH_MW, &data16);
		

	}

	for(uint16_t i=0;i<cnt;i++)
	{
		if(hamx[i]->changeflag==1)
		{
			AD9959_Apply_Change(hamx[i]->had9959);
			hamx[i]->changeflag=0;
		}
		
	}
	
	
	
	
	return HAL_OK;
}



/*
设置hamx指向的AM实例的载波频率
*/
uint8_t AM_SetCarrierFreq(AM_Instance* hamx, uint32_t Cfreq)
{
	AD9959_Set_Freq(hamx->had9959,hamx->CH_CW,&Cfreq);
	hamx->changeflag=1;
	return HAL_OK;
}


/*
设置hamx指向的AM实例的基波频率
*/
uint8_t AM_SetModulationFreq(AM_Instance* hamx, uint32_t Mfreq)
{
	AD9959_Set_Freq(hamx->had9959,hamx->CH_MW,&Mfreq);
	hamx->changeflag=1;
	return HAL_OK;
}

/*
设置hamx指向的AM实例的调制度
*/
uint8_t AM_SetMDepth(AM_Instance* hamx, float MD)
{
	hamx->MDepth=MD;
	hamx->changeflag=1;
	return HAL_OK;
}

/*
设置hamx1, hamx2指向的AM实例的载波时间差
hamxx1的时间偏移被设置为0，hamx2的时间偏移被设置为TD
TD:  延时，单位ns
*/
uint8_t AM_SetTDelay(AM_Instance* hamx1, AM_Instance* hamx2, uint16_t TD)
{
	hamx1->TDelay=0;
	hamx2->TDelay=TD;
	hamx1->changeflag=1;
	hamx2->changeflag=1;
	return HAL_OK;
}

uint8_t SetDAC(AM_Instance* hamx, uint16_t val)
{
	HAL_DAC_SetValue(hamx->hdac,hamx->CH_DAC,DAC_ALIGN_12B_R,val>4096?4096:val);
	DAC_Trigger(&hdac);
	return HAL_OK;
}





