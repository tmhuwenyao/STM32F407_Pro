#include "bsp_tim.h"
uint8_t time_flag_1s = 0;
uint32_t time_count_1s = 0;
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
static void TIM_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	TIM_TimeBaseInitStructure.TIM_Prescaler=8400-1 ; //计数频率 84M hz/(7199+1)=10Khz
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period=99 ;    //定时1ms的计数
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	/*清除时间更新标志位*/
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	
	TIM_Cmd(TIM3, ENABLE);
	
}
static void TIM_NVIC_InitConfig(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStruct.NVIC_IRQChannel=TIM3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStruct);
}
void TIM_InitConfig(void)
{
	TIM_Config();
	TIM_NVIC_InitConfig();
}

