/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dac.h"
#include "rtc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "AD9959.H"
#include "AM_9959.h"
#include "delay.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t freq=30; //MHz freq of carrier
uint8_t amp=1;//100mV effective value of carrier
uint8_t md=0;//% modulation depth of a&b signal
uint8_t tdelay=5;//10ns delay of b signal
uint8_t atten=0;//db attenuation of b signal
uint8_t sphase=0;//degree start phase of b signal

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_RTC_Init();
  MX_SPI3_Init();
  MX_USART1_UART_Init();
  MX_TIM6_Init();
  MX_DAC_Init();
  /* USER CODE BEGIN 2 */
  AM_Init();
  AM_Instance* AM_PTRS[2]={&AM1, &AM2};
  delay_init();
  
  
  
	SPI_LCD_Init();
	syslog("LCD Init Success");
	initRingBuffer();		
	HAL_UART_Receive_IT(&TJC_UART, RxBuffer, 1);	
	char str[100];
	uint32_t nowtime = HAL_GetTick();
	syslog("Hmi Init Success");
	Init_AD9959();
	HAL_Delay(100);
  AD9959_IO_Update();
	syslog("AD9959 Init Success");
	syslog("Init Success");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

	AM_SetModulationFreq(&AM1,2000000);
  while (1)
  {
	  
		
	  
		while (getRingBufferLength() >= FRAME_LENGTH)
		{
			
			if (getRingBufferLength() >= FRAME_LENGTH && 
				read1ByteFromRingBuffer(0) == 0x55 && 
			read1ByteFromRingBuffer(4) == 0xFF && 
			read1ByteFromRingBuffer(5) == 0xFF &&
			read1ByteFromRingBuffer(6) == 0xFF)//检查帧头及帧尾
			{
				uint8_t operation_flag=read1ByteFromRingBuffer(1);
				if (operation_flag == 0x00)
				{
					freq=read1ByteFromRingBuffer(2);
					sprintf(str,"Cfreq changed to %d MHz",freq);syslog(str);
					
					uint32_t cfreq=freq*1000000;
					AM_SetCarrierFreq(&AM1, cfreq);
					AM_SetCarrierFreq(&AM2, cfreq);
				} 
				else if (operation_flag == 0x01)
				{
					amp=read1ByteFromRingBuffer(2);
					sprintf(str,"EffValue changed to %d00mV",amp);syslog(str);
				} 
				else if (operation_flag == 0x02)
				{
					md=read1ByteFromRingBuffer(2);
					sprintf(str,"ModuDepth changed to %d %%",md);syslog(str);
					
					float MD=((float)md)/100;
					AM_SetMDepth(&AM1,MD);
					AM_SetMDepth(&AM2,MD);
				} 
				else if (operation_flag == 0x03)
				{
					tdelay=read1ByteFromRingBuffer(2);
					sprintf(str,"Bsig delay changed to %d0ns",tdelay);syslog(str);
					
					uint16_t TD=tdelay*10;
					AM_SetTDelay(&AM1,&AM2,TD);
				} 
				else if (operation_flag == 0x04)
				{
					atten=read1ByteFromRingBuffer(2);
					sprintf(str,"Bsig atten changed to %ddb",atten);syslog(str);
				} 
				else if (operation_flag == 0x05)
				{
					sphase=read1ByteFromRingBuffer(2);
					sprintf(str,"Bsig delay changed to %d0°",sphase);syslog(str);
				} 
				deleteRingBuffer(7); 
			} else
			{
				deleteRingBuffer(1);
				break;
			}
		}
	LED;
	AM_ApplyChanges(AM_PTRS,2);
	HAL_Delay(1);
	}
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/********************************************************
usart callback
**********************************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

	if(huart->Instance == TJC_UART_INS)	
	{
		write1ByteToRingBuffer(RxBuffer[0]);
		HAL_UART_Receive_IT(&TJC_UART,RxBuffer,1);		
	}
	return;
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
