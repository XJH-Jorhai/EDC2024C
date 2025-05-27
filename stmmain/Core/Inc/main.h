/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "gui.h"
#include "lcd_spi_200.h"
#include "tjc_usart_hmi.h"
#include "AD9959.H"
#include "rtc.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"
#include <stdio.h>
#include <stdlib.h>

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
extern uint8_t freq; //MHz freq of carrier
extern uint8_t amp;//100mV effective value of carrier
extern uint8_t md;//% modulation depth of a&b signal
extern uint8_t tdelay;//10ns delay of b signal
extern uint8_t atten;//db attenuation of b signal
extern uint8_t sphase;//degree start phase of b signal
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define HMC472_V5_Pin GPIO_PIN_2
#define HMC472_V5_GPIO_Port GPIOE
#define HMC472_V4_Pin GPIO_PIN_3
#define HMC472_V4_GPIO_Port GPIOE
#define HMC472_V3_Pin GPIO_PIN_4
#define HMC472_V3_GPIO_Port GPIOE
#define HMC472_V2_Pin GPIO_PIN_5
#define HMC472_V2_GPIO_Port GPIOE
#define HMC472_V1_Pin GPIO_PIN_6
#define HMC472_V1_GPIO_Port GPIOE
#define LED_Pin GPIO_PIN_0
#define LED_GPIO_Port GPIOC
#define AD9959_SD3_Pin GPIO_PIN_5
#define AD9959_SD3_GPIO_Port GPIOC
#define AD9959_SD2_Pin GPIO_PIN_0
#define AD9959_SD2_GPIO_Port GPIOB
#define AD9959_SCK_Pin GPIO_PIN_1
#define AD9959_SCK_GPIO_Port GPIOB
#define AD9959_SD1_Pin GPIO_PIN_2
#define AD9959_SD1_GPIO_Port GPIOB
#define AD9959_CS_Pin GPIO_PIN_7
#define AD9959_CS_GPIO_Port GPIOE
#define AD9959_SD0_Pin GPIO_PIN_8
#define AD9959_SD0_GPIO_Port GPIOE
#define AD9959_UP_Pin GPIO_PIN_9
#define AD9959_UP_GPIO_Port GPIOE
#define AD9959_P3_Pin GPIO_PIN_10
#define AD9959_P3_GPIO_Port GPIOE
#define AD9959_RST_Pin GPIO_PIN_11
#define AD9959_RST_GPIO_Port GPIOE
#define AD9959_P2_Pin GPIO_PIN_12
#define AD9959_P2_GPIO_Port GPIOE
#define AD9959_PDC_Pin GPIO_PIN_13
#define AD9959_PDC_GPIO_Port GPIOE
#define AD9959_P1_Pin GPIO_PIN_14
#define AD9959_P1_GPIO_Port GPIOE
#define AD9959_P0_Pin GPIO_PIN_10
#define AD9959_P0_GPIO_Port GPIOB
#define LCD_Backlight_Pin GPIO_PIN_12
#define LCD_Backlight_GPIO_Port GPIOD
#define LCD_DC_Pin GPIO_PIN_13
#define LCD_DC_GPIO_Port GPIOD
#define LCD_CS_Pin GPIO_PIN_15
#define LCD_CS_GPIO_Port GPIOA
#define HMC472_V6_Pin GPIO_PIN_1
#define HMC472_V6_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */
#define LED_ON HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET)
#define LED_OFF HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET)
#define LED HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin)
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
