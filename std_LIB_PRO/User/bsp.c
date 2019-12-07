#include "bsp.h"

#define DEV_TYPE		100
#define DEV_MAIN		100
#define DEV_VERSION		1


void BSP_init(void)
{
	Led_InitConfig();
	USART1_InitConfig();
	SYSTICK_InitConfig(168);//初始化时钟为168M
	TIM_InitConfig(); //定时器3初始化
	HOO_RTC_Init();
	
	DHT11_InitConfig();
	
	//初始化时间
//	RTC_Set_Time(11,5,6,RTC_H12_AM);	//设置时间
//	RTC_Set_Date(19,5,11,6);			//设置日期

}


