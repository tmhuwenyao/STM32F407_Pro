#include "hoo_dotmatrix.h"
#include "bsp_systick.h"
#include "bsp_usart.h"
/*��ʵ�����AX7219оƬ�������������*/


void MAX7219_Init(void)
{
	//�������ų�ʼ��
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(MAX7219_RCC, ENABLE);
	//GPIOF9,F10��ʼ������
	GPIO_InitStructure.GPIO_Pin = MAX7219_PIN_DATA | MAX7219_PIN_LOAD | MAX7219_PIN_CLK;//LED0��LED1��ӦIO��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(MAX729_PORT, &GPIO_InitStructure);//��ʼ��GPIO
}
void MAX7219_Clear(void)
{
	uint8_t i;
	for (i=1;i<=8;i++)
	{
		MAX7219_Write(i, 0);  
	}
}
/*�Ĵ�����Ϊ���ݼĴ����Ϳ��ƼĴ���*/
void MAX7219_Write(uint8_t reg_addr,uint8_t data)
{
	MAX7219_LOAD_L; 
	MAX7219_SendByte(reg_addr);                 		// ���ͼĴ�����ַ
	if(reg_addr>8)
		MAX7219_SendByte(data);                    		// ���ƼĴ�����������ģʽ����ʾ���ȣ�ɨ������ �ر�ģʽ����ʾ���
	else
		MAX7219_SendByte((data<<7)|(data>>1));			//�Ĵ�����ַΪ 0-7ʱ�����͵�����ʾ����
	MAX7219_LOAD_H;  
}
void MAX7219_SendByte(uint8_t data)
{
	uint8_t i;
	for (i=0; i<8; i++)
	{    
		MAX7219_CLK_L;                          // ʱ��������Ϊ��
		if(data&0x80)                      		// data���λ
			MAX7219_DATA_H; 
		else
			MAX7219_DATA_L;
		data <<= 1;
		MAX7219_CLK_H;                         // ʱ����Ϊ��
  }
}

void MAX7219_test(void)
{
	uint8_t i,j;
	printf("this is max7215 test experiment ...\n\r");
	MAX7219_Init();
//	MAX7219_CLK_H;
	#if 1
	while(1)
	{
		for(i=0;i<8;i++) //���е���
		{
			for(j=1;j<9;j++)
		{
			MAX7219_Write(j,1<<i);
		}
		delay_ms(200);
		}
    for(i=1;i<9;i++) //���е���
    {
      for(j=1;j<9;j++)
        if(j==i)
			MAX7219_Write(j,0xff);
        else 
			MAX7219_Write(j,0);
        delay_ms(200);
    }
	
	/*
    for(i=0;i<sizeof(logo)-8;i++)
    {
      for(j=1;j<9;j++)
      {
        Write(j,logo[j+i]<<1);	
      }
      _delay_ms(200);
    }
    for(i=1;i<9;i++)Write(i,0xff);
    _delay_ms(10000);
	*/
  }
  #endif
}

