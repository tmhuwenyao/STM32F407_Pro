#include "hoo_stepmotor.h"

static void STEPMOTOR_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_STEPMOTOR, ENABLE);
	//GPIOF9,F10初始化设置
	GPIO_InitStructure.GPIO_Pin = STEPMOTOR_ENA | STEPMOTOR_DIR | STEPMOTOR_PUL;//LED0和LED1对应IO口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(STEPMOTOR_PORT, &GPIO_InitStructure);//初始化GPIO
}
//定时器初始化，PID调速
static void STEPMOTOR_TIMEX_Init(void)
{
	
}

//步进电机初始化
void stepmotor_init(void)
{
	STEPMOTOR_GPIO_Init();
	STEPMOTOR_TIMEX_Init();
}