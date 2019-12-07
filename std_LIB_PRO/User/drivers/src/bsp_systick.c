#include "bsp_systick.h"
static u8 fac_us=0;  //΢����ʱ������
static u16 fac_ms=0;  //������ʱ������
/*��ʼ���δ�ʱ��
*SYSCLK �̶�ΪHCLK��1/8
*SYSCLKΪϵͳʱ��
*/
void SYSTICK_InitConfig(uint8_t SYSCLK)
{
	SysTick->CTRL&=0xfffffffb;//bit2��գ�ѡ���ⲿʱ�ӣ� HCLK/8
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	fac_us=SYSCLK/8;		    
	fac_ms=(u16)fac_us*1000;
	
}

/*��ʱ����
*SYSTICK->LOAD Ϊ24λ�Ķ�ʱ������������ʱ��mtime<oxffffff*8*1000/SYSCLK
*SYSCLK��λΪhz,mtime��λΪms
*��168M������,nms<=798ms 
*/
void delay_ms(uint16_t mtime)
{
	u32 temp;		   
	SysTick->LOAD=(u32)mtime*fac_ms;//ʱ�����
	SysTick->VAL =0x00;           //��յ���ʱ
	SysTick->CTRL=0x01 ;          //��ʼ��ʱ
	do
	{
		temp=SysTick->CTRL;
	}while(temp&0x01&&!(temp&(1<<16)));//�ȴ�ʱ�䵽��
	SysTick->CTRL=0x00;       //�رռ�����
	SysTick->VAL =0X00;       //��ռ�����
}
/*��ʱ����*/
void delay_us(uint32_t utime)
{
	u32 temp;		   
	SysTick->LOAD=(u32)utime*fac_us;//ʱ�����
	SysTick->VAL =0x00;           //��յ���ʱ
	SysTick->CTRL=0x01 ;          //��ʼ��ʱ
	do
	{
		temp=SysTick->CTRL;
	}while(temp&0x01&&!(temp&(1<<16)));//�ȴ�ʱ�䵽��
	SysTick->CTRL=0x00;       //�رռ�����
	SysTick->VAL =0X00;       //��ռ�����
	
}

