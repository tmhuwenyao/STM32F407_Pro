#include "hoo_dht11.h"
#include "bsp_systick.h"
#include "bsp_usart.h"
/*������ʪ��*/
uint8_t humi_i,humi_f;
uint8_t temp_i,temp_f;
/*����dht11_rst�����м���uart1_print����������������ӡ����
*��Ҫ��ʱ���������ʹ�ô�ӡ����
*/
void DHT11_InitConfig()
{
	RCC_AHB1PeriphClockCmd(DHT11_CLK, ENABLE); //����GPIOBʱ��
	DHT11_Out();
	GPIO_SetBits(DHT11_GPIO,DHT11_PIN);
}
/*��������Ϊ����ģʽ*/
void DHT11_In()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin=DHT11_PIN ;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;   //��������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(DHT11_GPIO,&GPIO_InitStructure);
}
/*��������Ϊ���ģʽ*/
void DHT11_Out()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin=DHT11_PIN ;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;   //�������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_Init(DHT11_GPIO,&GPIO_InitStructure);
}

/*��λDHT11,������ʼ�ź�*/
void dht11_rst(void)
{
	DHT11_Out();
	GPIO_ResetBits(DHT11_GPIO,DHT11_PIN);  //�����ŵ�ƽ���ͣ�����20ms����������18ms
	delay_ms(20);
	GPIO_SetBits(DHT11_GPIO,DHT11_PIN);
	
	delay_us(30);   //�����ŵ�ƽ���ߣ�����30u
	DHT11_In();
//	uart1_print(" reset ok....\r\n");    //���󣬲���ʹ�ô˺���
}
/*���DHT11�Ƿ���ȷ��Ӧ*/
char dht11_check(void)
{
	uint8_t count;
	while((GPIO_ReadInputDataBit(DHT11_GPIO,DHT11_PIN)==0)&&count<100)
	{
	    count++;
		delay_us(1);
	}
	if(count>=100)
		return 1;
	count=0;
	while((GPIO_ReadInputDataBit(DHT11_GPIO,DHT11_PIN)==1)&&count<100)
	{
		count++;
		delay_us(1);
	}
	if(count>=100)
		return 1;
//	uart1_print("the dht is working perfectly...");        //���󣬲���ʹ�ô˺���
	return 0;
}

/*��ȡһ��bit������*/
char dht11_read_bit(void)
{
	uint8_t count=0;
	
	/*�ȴ��ܽű�Ϊ�͵�ƽ*/
	while((GPIO_ReadInputDataBit(DHT11_GPIO,DHT11_PIN)==1)&&count<100) 
	{
		count++;
		delay_us(1);
	}
	count=0;
	/*�ȴ��ܽű�Ϊ�ߵ�ƽ*/
	while((GPIO_ReadInputDataBit(DHT11_GPIO,DHT11_PIN)==0)&&count<100) 
	{
		count++;
		delay_us(1);
	}
	delay_us(40);
	if(GPIO_ReadInputDataBit(DHT11_GPIO,DHT11_PIN)==0)  //������ƽΪ�͵�ƽ�������0
		return 0;
	else 
		return 1;
}

/*��ȡһ���ֽڵ�����*/
char dht11_read_byte(void)
{
	char data=0;
	int i;
	for(i=0;i<8;i++)
	{
		data<<=1;
		data |=dht11_read_bit();
	}
	return data;
}

/*��ȡһ�����������ݣ�40bit����*/
char dht11_read_data(u8* humi_i,u8* humi_f,u8* temp_i,u8* temp_f)
{
	u8 a[5];
	u8 i=0;
	dht11_rst();
	if(dht11_check()==1)
		uart1_print("check failed\r\n");
	
	for(i=0;i<5;i++)
	{
		a[i]=dht11_read_byte();
		//uart1_print("hello\r\n");    //�˴����ܴ�ӡ���ݣ������ȡ����ʧ��
	}
	if((a[0]+a[1]+a[2]+a[3])==a[4])
	{
		*humi_i=a[0];     //ʪ����������
		*humi_f=a[1];     //ʪ����������
		*temp_i=a[2];     //�¶���������	
		*temp_f=a[3];     //�¶���������
		 return 0;
	}
	return 1;
}



