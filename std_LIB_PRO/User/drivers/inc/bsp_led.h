#ifndef __BSP_LED_H
#define __BSP_LED_H

#include "stm32f4xx.h"
//#include "bsp_systick.h"
#define ON   1
#define OFF  0
#define RCC_LED		RCC_AHB1Periph_GPIOA
#define LED1_PIN	GPIO_Pin_6
#define LED2_PIN	GPIO_Pin_7
#define LED_PORT 	GPIOA

typedef enum{
	RED=0,
	GREEN,
	BLUE,
}LED_Type;

enum{
	LED_OFF = 0,
	LED_ON,
	LED_TOGGLE,  //·­×ª
};

void Led_InitConfig(void);
void Led_PWMInitConfig(void);
void Led_Control(LED_Type type,uint16_t status);

void RED_LED(u16 pwm);
void GREEN_LED(u16 pwm);
void BLUE_LED(u16 pwm);
void LED_PWM_TEST(void);
#endif
