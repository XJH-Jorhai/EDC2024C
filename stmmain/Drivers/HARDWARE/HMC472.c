#include "HMC472.h"

void HMC472_WritePin(HMC472_Instance* handle, uint8_t idx, uint8_t val)
{
	HAL_GPIO_WritePin(handle->PORTS[idx], handle->PINS[idx], val);
}




const GPIO_TypeDef* HMC472_POTRSDEF[]={
	HMC472_V1_GPIO_Port,
	HMC472_V2_GPIO_Port,
	HMC472_V3_GPIO_Port,
	HMC472_V4_GPIO_Port,
	HMC472_V5_GPIO_Port,
	HMC472_V6_GPIO_Port
};

const uint16_t HMC472_PINSDEF[]={
	HMC472_V1_Pin,
	HMC472_V2_Pin,
	HMC472_V3_Pin,
	HMC472_V4_Pin,
	HMC472_V5_Pin,
	HMC472_V6_Pin
};

HMC472_Instance hHMC;

/*
初始化HMC472实例
*/
uint8_t HMC472_Instance_Init(HMC472_Instance* handle, const GPIO_TypeDef* ports[], const uint16_t pins[])
{
	for(uint8_t i=0;i<6;i++)
	{
		handle->PORTS[i]=ports[i];
		handle->PINS[i]=pins[i];
		HMC472_WritePin(handle,i,0);
	}
	handle->changeflag=1;
	return HAL_OK;
}

uint8_t HMC472_SetAtten(HMC472_Instance* handle, float dB)
{
	handle->atten=dB;
	handle->changeflag=1;
	return HAL_OK;
}

uint8_t HMC472_ApplyAtten(HMC472_Instance* handle)
{
	if(handle->changeflag==1)
	{
		
		handle->changeflag=0;
		//将0.5-31.5范围等效为1-63方便引脚操作
		uint16_t dB = (uint16_t)(handle->atten * 2 + 0.5f); // 标准四舍五入
		
		if(dB>63)
		{
			syslog("HMC472 atten out of rng");
			return HAL_ERROR;
		}
		
		for(int i=0;i<6;i++){
			HMC472_WritePin(handle, i, !((dB >> i) & 0x01));
		}
		syslog("HMC472 Change Applied");
	}

	return HAL_OK;
}



