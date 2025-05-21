#ifndef _AD9959_H_
#define _AD9959_H_
#include "main.h"
#include "stdint.h"
#include "AD9959_compens.H"
//AD9959寄存器地址定义
#define CSR_ADD   0x00   //CSR 通道选择寄存器
#define FR1_ADD   0x01   //FR1 功能寄存器1
#define FR2_ADD   0x02   //FR2 功能寄存器2
#define CFR_ADD   0x03   //CFR 通道功能寄存器
#define CFTW0_ADD 0x04   //CTW0 通道频率转换字寄存器
#define CPOW0_ADD 0x05   //CPW0 通道相位转换字寄存器
#define ACR_ADD   0x06   //ACR 幅度控制寄存器
#define LSRR_ADD  0x07   //LSR 通道线性扫描寄存器
#define RDW_ADD   0x08   //RDW 通道线性向上扫描寄存器
#define FDW_ADD   0x09   //FDW 通道线性向下扫描寄存器

#define AD9959_FREQ ((uint64_t)500000000)

/* AD9959 Control Pin Macros */
#define CS(a)          HAL_GPIO_WritePin(AD9959_CS_GPIO_Port, AD9959_CS_Pin, (a))  // Chip Select (active low, default HIGH)
#define SCLK(a)        HAL_GPIO_WritePin(GPIOB, AD9959_SCK_Pin, (a))               // Serial Clock (SCK)
#define UPDATE(a)      HAL_GPIO_WritePin(GPIOE, AD9959_UP_Pin, (a))                // Update Trigger (pulse HIGH to latch data)
#define PS0(a)         HAL_GPIO_WritePin(GPIOB, AD9959_P0_Pin, (a))                // Phase Control Bit 0
#define PS1(a)         HAL_GPIO_WritePin(GPIOE, AD9959_P1_Pin, (a))                // Phase Control Bit 1
#define PS2(a)         HAL_GPIO_WritePin(GPIOE, AD9959_P2_Pin, (a))                // Phase Control Bit 2
#define PS3(a)         HAL_GPIO_WritePin(GPIOE, AD9959_P3_Pin, (a))                // Phase Control Bit 3
#define SDIO0(a)       HAL_GPIO_WritePin(GPIOE, AD9959_SD0_Pin, (a))               // Serial Data Bit 0 (LSB)
#define SDIO1(a)       HAL_GPIO_WritePin(GPIOB, AD9959_SD1_Pin, (a))               // Serial Data Bit 1
#define SDIO2(a)       HAL_GPIO_WritePin(GPIOB, AD9959_SD2_Pin, (a))               // Serial Data Bit 2
#define SDIO3(a)       HAL_GPIO_WritePin(GPIOC, AD9959_SD3_Pin, (a))               // Serial Data Bit 3 (MSB)
#define AD9959_PWR(a)  HAL_GPIO_WritePin(GPIOE, AD9959_PDC_Pin, (a))               // Power-Down Control (HIGH = normal operation)
#define Reset(a)       HAL_GPIO_WritePin(GPIOE, AD9959_RST_Pin, (a))               // Hardware Reset (active HIGH)


typedef struct{
	uint32_t freq[4];
	uint16_t phase[4];
	uint16_t amp[4];
	uint8_t changeFlag;
	
}AD9959_HandleTypeDef;

//extern uint32_t SinFre[4];
//extern uint16_t SinPhr[4];
//extern uint16_t SinAmp[4];
extern AD9959_HandleTypeDef hAD9959;


void delay1 (uint32_t length);
void IntReset(void);	  //AD9959复位
void AD9959_IO_Update(void);   //AD9959更新数据
void WriteData_AD9959(uint8_t RegisterAddress, uint8_t NumberofRegisters, uint8_t *RegisterData,uint8_t temp);
void Init_AD9959(void);
void Write_frequence(uint8_t Channel,uint32_t Freq);
void Write_Amplitude(uint8_t Channel, uint16_t Ampli);
void Write_Phase(uint8_t Channel,uint16_t Phase);

uint8_t AD9959_Set_Phase(AD9959_HandleTypeDef* had9959, uint16_t channel, float* data);

uint8_t AD9959_Set_Freq(AD9959_HandleTypeDef* had9959, uint16_t channel, uint32_t* data);

uint8_t AD9959_Set_Amp(AD9959_HandleTypeDef* had9959, uint16_t channel, uint16_t* data);

uint8_t AD9959_Apply_Change(AD9959_HandleTypeDef* had9959);


#endif


