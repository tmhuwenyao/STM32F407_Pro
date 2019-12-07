#ifndef __HOO_SETPMOTOR_H
#define __HOO_SETPMOTOR_H

#include "stm32f4xx.h"

#define RCC_STEPMOTOR			RCC_AHB1Periph_GPIOC
#define STEPMOTOR_DIR			GPIO_Pin_6
#define STEPMOTOR_ENA			GPIO_Pin_7
#define STEPMOTOR_PUL			GPIO_Pin_8
#define STEPMOTOR_PORT 			GPIOC

extern void stepmotor_init(void);

#endif

