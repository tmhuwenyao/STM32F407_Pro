#include <stdio.h>
#include "bsp.h"
#include "hoo_dotmatrix.h"

int main(void)
{
	RTC_TimeTypeDef RTC_TimeStruct;
	RTC_DateTypeDef RTC_DateStruct;

	BSP_init();
//	MAX7219_test();
	while(1)
	{
		Monitor_CommProcess();	//后台监控操作
		if(time_flag_1s == TRUE)
		{
			RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);	
			printf("Time:%02d:%02d:%02d		",RTC_TimeStruct.RTC_Hours,RTC_TimeStruct.RTC_Minutes,RTC_TimeStruct.RTC_Seconds); 
			RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);
			printf("Date:20%02d-%02d-%02d\n\r",RTC_DateStruct.RTC_Year,RTC_DateStruct.RTC_Month,RTC_DateStruct.RTC_Date);
			Led_Control(0,LED_TOGGLE);
			time_flag_1s = FALSE;
		}
//		dht11_read_data(&humi_i,&humi_f,&temp_i,&temp_f);
//		printf("humility=%d.%d  ,temperature=%d.%d\r\n",humi_i,humi_f,temp_i,temp_f);
//		GPIO_ResetBits(DHT11_GPIO,DHT11_PIN);  //将引脚电平至低，持续20ms，至少拉低18ms
 
	}
}

