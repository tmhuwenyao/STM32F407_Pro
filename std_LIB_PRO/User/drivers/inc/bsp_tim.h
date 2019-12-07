#ifndef __BSP_TIM_H
#define __BSP_TIM_H

#include "stm32f4xx.h"

#define TRUE 	1
#define FALSE 	0

extern uint8_t time_flag_1s ;
extern uint32_t time_count_1s ;


void TIM_InitConfig(void);

#endif

