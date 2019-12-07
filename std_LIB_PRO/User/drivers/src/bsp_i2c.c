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
/*SDA���ģʽ*/
static void SDA_OUT()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin=I2C_SDA_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(I2C_GPIO, &GPIO_InitStructure);
}
/*SDA����ģʽ*/
static void SDA_IN()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin=I2C_SDA_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init(I2C_GPIO, &GPIO_InitStructure);
}
/*������ʼ�ź�*/
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
/*����ֹͣ�ź�*/
void I2C_Stop(void)
{
	SDA_OUT();
	I2C_SCLK_1();
	I2C_SDA_0();
	delay_us(8);
	I2C_SDA_1();
	delay_us(8);
}
/*����Ӧ���ź�
*  0���϶��ش�
*  1���񶨻ش�
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
/*����Ӧ���ź�
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

/*����1BYTE������*/
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
/*����1BYTE������*/
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
*������  ��I2C_CheckDevice
*����˵�������I2C�豸��CPU���豸���͵�ַ���豸��CPU����Ӧ�𣬿��ж��豸�Ƿ����
*��    ����Address I2C���ߵ��豸��ַ
*����ֵ  ��0 ��ʾ����    1 ��ʾ��Ӧ�𣬲�����
*/
uint8_t I2C_CheckDevice(uint8_t _Address)
{
	u8 ack;
	I2C_Start();
	/*����I2C���ߵ�ַ �Ӷ�д����λ 0 д 1 ��*/
	I2C_SendByte(_Address |I2C_WR);
	ack=I2C_RecvACK();
	I2C_Stop();
	return ack;
}

