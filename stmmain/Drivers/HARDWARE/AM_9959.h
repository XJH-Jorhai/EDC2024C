#ifndef __AM_9959_H
#define __AM_9959_H
#include "main.h"
#include "dac.h"
#include "AD9959.H"
#include "AD9959_compens.H"

#ifdef ARM_MATH_CM4
#include "arm_math.h"
#else
#include "math.h"
#endif

#define AM_DAC_HANDLE  hdac

#define DAC_Trigger(hdac) (SET_BIT((hdac)->Instance->SWTRIGR, DAC_SWTRIGR_SWTRIG1))

typedef struct{
	AD9959_HandleTypeDef* had9959;
	uint8_t CH_MW;//载波（信号）
	uint8_t CH_CW;//调制（高频）
	
	DAC_HandleTypeDef* hdac;
	uint32_t CH_DAC;
	
	
	
	float MDepth;//调制度
	float TDelay;//延时
	float amp;//幅度
	
	float min_amp;//9959最小幅度比例
	
	uint8_t changeflag;
	
}AM_Instance;

extern AM_Instance AM1,AM2;




void AM_Instance_Init(AM_Instance* ham, AD9959_HandleTypeDef* had9959, uint16_t chcw, uint16_t chmw, DAC_HandleTypeDef* hdacx, uint32_t dac_channel);
uint8_t AM_Init(void);
uint8_t SetDAC(AM_Instance* hamx, uint16_t val);
uint8_t AM_ApplyChanges(AM_Instance* hamx[], uint16_t cnt);
uint8_t AM_SetCarrierFreq(AM_Instance* hamx, uint32_t Cfreq);
uint8_t AM_SetModulationFreq(AM_Instance* hamx, uint32_t Mfreq);
uint8_t AM_SetCarrierFreq(AM_Instance* hamx, uint32_t Cfreq);
uint8_t AM_SetModulationFreq(AM_Instance* hamx, uint32_t Mfreq);
uint8_t AM_SetMDepth(AM_Instance* hamx, float MD);
uint8_t AM_SetTDelay(AM_Instance* hamx1, AM_Instance* hamx2, uint16_t TD);
	


#endif
