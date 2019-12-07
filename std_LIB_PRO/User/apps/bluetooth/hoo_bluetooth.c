#include "hoo_bluetooth.h"
#include "bsp_systick.h"

void BLUETOOTH_Init()
{
	UART3_InitConfig();
	BLE_On();
	UASRT_NVIC_Configuration();
	
}
void BLE_On()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOA,GPIO_Pin_1);   //PA1脚至低电平，使能蓝牙芯片
}
void UART3_InitConfig()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	//开启 GPIO ,GPIO复用，串口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);   //使能串口3
	//配置串口输出引脚TX
	GPIO_InitStructure.GPIO_Pin=BLUE_TX;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;       //复用推挽输出
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(BLUE_PORT,&GPIO_InitStructure);
	//配置串口输入引脚RX
	GPIO_InitStructure.GPIO_Pin=BLUE_RX;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;   //浮空输入
	GPIO_Init(BLUE_PORT,&GPIO_InitStructure);
	//配置串口输出引脚TX
	GPIO_InitStructure.GPIO_Pin=BLUE_RTS;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;       //复用推挽输出
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(BLUE_PORT,&GPIO_InitStructure);
	//配置串口参数
	USART_InitStructure.USART_BaudRate=9600;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Rx |USART_Mode_Tx ;
	USART_Init(USART3,&USART_InitStructure);
	
	//使能串口
	USART_Cmd(USART3,ENABLE);
	//使能中断
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
}
//串口中断配置
void UASRT_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	
	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}
//输出一个字符
void uart3_putchar(char c)
{
	USART_SendData(USART3, c);
	while(USART_GetFlagStatus(USART3, USART_FLAG_TC)!=SET);
}
//输出一个字符串
void uart3_senddata(char * str)
{
	while(*str!='\0')
		uart3_putchar(*str++);
	
}
char usart3_getc(void)
{
	char temp = 0;
	
	while(USART_GetFlagStatus(USART3,USART_FLAG_RXNE)!= SET); 
	
	temp = USART_ReceiveData(USART3);

	return temp;
}

int usart3_getstr(unsigned char* buff,int len)
{
	int i = 0;

	for(i = 0;i < len;i++)
	{
		buff[i] = usart3_getc();
	}

	return len;
}
char BLE_SendData(char * str)
{
	int i;
	/*打开流控*/
	GPIO_ResetBits(GPIOB,BLUE_RTS);
	delay_ms(10);
	/*发送数据*/
	for(i=0;i<8;i++)
		uart3_putchar(str[i]);
	delay_ms(10);
	/*关闭流控*/
	GPIO_SetBits(GPIOB,BLUE_RTS);
	
	return 0;
}

char data_pack(char a[],u8 interger,u8 decimal,BLE_CMD cmd)
{
	u8 i,t;
	a[0]=0xff;
	a[1]=0xff;
	a[2]=0;
	a[3]=4;
	a[4]=cmd;
	a[5]=interger;
	a[6]=decimal;
	t=a[3];
	for(i=1;i<4;i++)
	{
		t^=a[3+i];
	}
	a[7]=t;
	return 1;
}
char data_pack_lumihar(char a[],uint16_t data)
{
	u8 i,t;
	a[0]=0xff;
	a[1]=0xff;
	a[2]=0;
	a[3]=4;
	a[4]=0x22;
	a[5]=data>>8;
	a[6]=0xff&data;
	t=a[3];
	for(i=1;i<4;i++)
	{
		t^=a[3+i];
	}
	a[7]=t;
	
	return 1;
}


