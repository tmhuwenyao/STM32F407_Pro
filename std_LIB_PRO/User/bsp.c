#include "bsp.h"

#define DEV_TYPE		100
#define DEV_MAIN		100
#define DEV_VERSION		1


void BSP_init(void)
{
	Led_InitConfig();
	USART1_InitConfig();
	SYSTICK_InitConfig(168);//��ʼ��ʱ��Ϊ168M
	TIM_InitConfig(); //��ʱ��3��ʼ��
	HOO_RTC_Init();
	
	DHT11_InitConfig();
	
	//��ʼ��ʱ��
//	RTC_Set_Time(11,5,6,RTC_H12_AM);	//����ʱ��
//	RTC_Set_Date(19,5,11,6);			//��������

}


