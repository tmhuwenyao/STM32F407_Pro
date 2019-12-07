#include "hoo_dotmatrix.h"
#include "bsp_systick.h"
#include "bsp_usart.h"
/*本实验采用AX7219芯片进行数码管驱动*/


void MAX7219_Init(void)
{
	//控制引脚初始化
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(MAX7219_RCC, ENABLE);
	//GPIOF9,F10初始化设置
	GPIO_InitStructure.GPIO_Pin = MAX7219_PIN_DATA | MAX7219_PIN_LOAD | MAX7219_PIN_CLK;//LED0和LED1对应IO口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(MAX729_PORT, &GPIO_InitStructure);//初始化GPIO
}
void MAX7219_Clear(void)
{
	uint8_t i;
	for (i=1;i<=8;i++)
	{
		MAX7219_Write(i, 0);  
	}
}
/*寄存器分为数据寄存器和控制寄存器*/
void MAX7219_Write(uint8_t reg_addr,uint8_t data)
{
	MAX7219_LOAD_L; 
	MAX7219_SendByte(reg_addr);                 		// 发送寄存器地址
	if(reg_addr>8)
		MAX7219_SendByte(data);                    		// 控制寄存器包括编码模式，显示亮度，扫描限制 关闭模式，显示检查
	else
		MAX7219_SendByte((data<<7)|(data>>1));			//寄存器地址为 0-7时，发送的是显示数据
	MAX7219_LOAD_H;  
}
void MAX7219_SendByte(uint8_t data)
{
	uint8_t i;
	for (i=0; i<8; i++)
	{    
		MAX7219_CLK_L;                          // 时钟数据设为低
		if(data&0x80)                      		// data最高位
			MAX7219_DATA_H; 
		else
			MAX7219_DATA_L;
		data <<= 1;
		MAX7219_CLK_H;                         // 时钟设为高
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
		for(i=0;i<8;i++) //逐行点亮
		{
			for(j=1;j<9;j++)
		{
			MAX7219_Write(j,1<<i);
		}
		delay_ms(200);
		}
    for(i=1;i<9;i++) //逐列点亮
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

