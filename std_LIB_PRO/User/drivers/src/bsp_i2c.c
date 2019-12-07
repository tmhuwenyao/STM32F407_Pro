#include "bsp_i2c.h"

void I2C_GPIOInitConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_I2C_CLK,ENABLE);
	GPIO_InitStructure.GPIO_Pin=I2C_SDA_PIN  |I2C_SCLK_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(I2C_GPIO, &GPIO_InitStructure);
	
	GPIO_SetBits(I2C_GPIO, I2C_SDA_PIN  |I2C_SCLK_PIN);
	
}
/*SDA输出模式*/
static void SDA_OUT()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin=I2C_SDA_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(I2C_GPIO, &GPIO_InitStructure);
}
/*SDA输入模式*/
static void SDA_IN()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin=I2C_SDA_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init(I2C_GPIO, &GPIO_InitStructure);
}
/*发送起始信号*/
void I2C_Start(void)
{
	SDA_OUT();
	I2C_SDA_1();
	I2C_SCLK_1();
	delay_us(8);
	I2C_SDA_0();
	delay_us(8);
	I2C_SCLK_0();
}
/*发送停止信号*/
void I2C_Stop(void)
{
	SDA_OUT();
	I2C_SCLK_1();
	I2C_SDA_0();
	delay_us(8);
	I2C_SDA_1();
	delay_us(8);
}
/*发送应答信号
*  0：肯定回答
*  1：否定回答
*/
void I2C_SendACK(u8 ack)
{
	SDA_OUT();
	I2C_SCLK_0(); 
	delay_us(8);
	if(ack)
		I2C_SDA_1();   //NACK
	else
		I2C_SDA_0();   //ACK
	I2C_SCLK_1();
	delay_us(8);
	I2C_SCLK_0();
	delay_us(8);
	
	
}
/*接受应答信号
*0:ACK
*1:NACK
*/
u8 I2C_RecvACK(void)
{
	u8 errtime;
	SDA_IN();
    I2C_SCLK_1();
	delay_us(8);
	I2C_SDA_1();
	delay_us(8);
	
	while(I2C_SDA_READ())
	{
		errtime++;
		if(errtime>250)
		{
			I2C_Stop();
			return 1;
		}
	}
	I2C_SCLK_0();
	return 0;
}

/*发送1BYTE的数据*/
void I2C_SendByte(u8 data)
{
	u8 i;
	SDA_OUT();
	I2C_SCLK_0();
	for(i=0;i<8;i++)
	{
		if(data&0x80)
			I2C_SDA_1();
		else
			I2C_SDA_0();
		data<<=1;
		delay_us(5);
		I2C_SCLK_1();
		delay_us(5);
		I2C_SCLK_0();
		delay_us(5);
	}
}
/*接受1BYTE的数据*/
u8 I2C_RecvByte(void)
{
	u8 i,byte=0;
	SDA_IN();
	for(i=0;i<8;i++)
	{
		delay_us(5);
		I2C_SCLK_1();
		delay_us(5);
		byte<<=1;
		if(I2C_SDA_READ())
			byte|=0x01;
		I2C_SCLK_0();
		delay_us(5);
	}
	return byte;
}
/*
*函数名  ：I2C_CheckDevice
*功能说明：检查I2C设备，CPU向设备发送地址，设备向CPU返回应答，可判断设备是否存在
*参    数：Address I2C总线的设备地址
*返回值  ：0 表示存在    1 表示无应答，不存在
*/
uint8_t I2C_CheckDevice(uint8_t _Address)
{
	u8 ack;
	I2C_Start();
	/*发送I2C总线地址 加读写控制位 0 写 1 读*/
	I2C_SendByte(_Address |I2C_WR);
	ack=I2C_RecvACK();
	I2C_Stop();
	return ack;
}

