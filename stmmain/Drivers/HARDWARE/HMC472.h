#ifndef __HMC472_H
#define __HMC472_H

#include "main.h"
#include "math.h"


typedef struct{
	GPIO_TypeDef* PORTS[8];
	uint16_t PINS[8];
	float atten;
	uint8_t changeflag;
	
}HMC472_Instance;

extern const GPIO_TypeDef* HMC472_POTRSDEF[];
extern const uint16_t HMC472_PINSDEF[];
extern HMC472_Instance hHMC;




uint8_t HMC472_Instance_Init(HMC472_Instance* handle, const GPIO_TypeDef* ports[], const uint16_t pins[]);
uint8_t HMC472_SetAtten(HMC472_Instance* handle, float dB);
uint8_t HMC472_ApplyAtten(HMC472_Instance* handle);


#endif
