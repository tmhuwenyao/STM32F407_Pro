#ifndef __BSP_I2C_H
#define __BSP_I2C_H

#include "stm32f10x.h"
#include "bsp_systick.h"


#define I2C_GPIO             GPIOB
#define RCC_I2C_CLK          RCC_APB2Periph_GPIOB
#define I2C_SDA_PIN          GPIO_Pin_7
#define I2C_SCLK_PIN         GPIO_Pin_6
#define I2C_RD               1   /*读控制位*/
#define I2C_WR               0   /*写控制位*/

#define I2C_SDA_1()          GPIO_SetBits(I2C_GPIO, I2C_SDA_PIN)
#define I2C_SDA_0()          GPIO_ResetBits(I2C_GPIO, I2C_SDA_PIN)

#define I2C_SCLK_1()         GPIO_SetBits(I2C_GPIO, I2C_SCLK_PIN)
#define I2C_SCLK_0()         GPIO_ResetBits(I2C_GPIO, I2C_SCLK_PIN)

#define I2C_SDA_READ()       GPIO_ReadInputDataBit(I2C_GPIO,I2C_SDA_PIN)

void I2C_GPIOInitConfig(void);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_SendACK(u8 data);
u8 I2C_RecvACK(void);
uint8_t I2C_CheckDevice(uint8_t _Address);

void I2C_SendByte(u8 data);
u8 I2C_RecvByte(void);

#endif

