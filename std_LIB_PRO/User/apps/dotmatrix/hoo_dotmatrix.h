#ifndef	__HOO_DOTMATRIX_H_
#define	__HOO_DOTMATRIX_H_

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
/*********************** MAX7219 ÄÚ²¿¼Ä´æÆ÷**********************/
#define REG_DECODE        0x09            // ÒëÂë·½Ê½¼Ä´æÆ÷ 
#define REG_INTENSITY     0x0a            // ÁÁ¶È¼Ä´æÆ÷
#define REG_SCAN_LIMIT    0x0b            // É¨Ãè½çÏÞ¼Ä´æÆ÷
#define REG_SHUTDOWN      0x0c            // Í£»ú¼Ä´æÆ÷
#define REG_DISPLAY_TEST  0x0f            // ÏÔÊ¾²âÊÔ¼Ä´æÆ÷
#define INTENSITY_MIN     0x00            // ×îÐ¡ÏÔÊ¾ÁÁ¶È
#define INTENSITY_MAX     0x0f            // ×î´óÏÔÊ¾ÁÁ¶È
/*********************** MAX7219 ¶Ë¿ÚÐÅºÅ ************************/
#define	MAX7219_PIN_DATA    GPIO_Pin_0          //pc0
#define	MAX7219_PIN_LOAD	GPIO_Pin_1          //pc1
#define	MAX7219_PIN_CLK     GPIO_Pin_2          //pc2
#define MAX7219_RCC			RCC_AHB1Periph_GPIOC
#define MAX729_PORT			GPIOC				//Òý½Å¶Ë¿Ú


#define	MAX7219_DATA_H		(MAX729_PORT->BSRRL = MAX7219_PIN_DATA)  //DATA set high
#define	MAX7219_DATA_L		(MAX729_PORT->BSRRH = MAX7219_PIN_DATA)	 //DATA set low
#define	MAX7219_LOAD_H		(MAX729_PORT->BSRRL = MAX7219_PIN_LOAD)  //LOAD set high
#define	MAX7219_LOAD_L		(MAX729_PORT->BSRRH = MAX7219_PIN_LOAD)	 //LOAD set low
#define	MAX7219_CLK_H		(MAX729_PORT->BSRRL = MAX7219_PIN_CLK)   //CLK set high
#define	MAX7219_CLK_L		(MAX729_PORT->BSRRH = MAX7219_PIN_CLK)	 //CLK set low
extern void MAX7219_Write(uint8_t i,uint8_t j);
extern void MAX7219_SendByte(uint8_t data);
extern void MAX7219_Clear(void);
extern void MAX7219_Init(void);
extern void MAX7219_test(void);

	


#endif

