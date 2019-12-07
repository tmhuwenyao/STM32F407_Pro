#include "hoo_stepmotor.h"

static void STEPMOTOR_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_STEPMOTOR, ENABLE);
	//GPIOF9,F10��ʼ������
	GPIO_InitStructure.GPIO_Pin = STEPMOTOR_ENA | STEPMOTOR_DIR | STEPMOTOR_PUL;//LED0��LED1��ӦIO��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(STEPMOTOR_PORT, &GPIO_InitStructure);//��ʼ��GPIO
}
//��ʱ����ʼ����PID����
static void STEPMOTOR_TIMEX_Init(void)
{
	
}

//���������ʼ��
void stepmotor_init(void)
{
	STEPMOTOR_GPIO_Init();
	STEPMOTOR_TIMEX_Init();
}