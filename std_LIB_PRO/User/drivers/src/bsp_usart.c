/**
  ******************************************************************************
  * @file    bsp_usart1.c
  * @author  HU WENYAO
  * @version V1.0
  * @date    2019-04-21
  * @brief   串口1接收函数
  ******************************************************************************
  */
#include "bsp_usart.h"
#include "hoo_modbuscomm.h"
/* 定义每个串口结构体变量 */
uint16_t modbus_recv_gap_flag = 0; //大于3.5个字符的时间间隔当做两帧数据，2ms左右
UART_T g_tUart1;
static uint8_t g_TxBuf1[TX_BUF_SIZE];		/* 发送缓冲区 */
static uint8_t g_RxBuf1[RX_BUF_SIZE];		/* 接收缓冲区 */
/*
*********************************************************************************************************
*	函 数 名: UartVarInit
*	功能说明: 初始化串口相关的变量
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void UartVarInit(void)
{
	g_tUart1.uart = USART1;						/* STM32 串口设备 */
	g_tUart1.pTxBuf = g_TxBuf1;					/* 发送缓冲区指针 */
	g_tUart1.pRxBuf = g_RxBuf1;					/* 接收缓冲区指针 */
	g_tUart1.usTxBufSize = TX_BUF_SIZE;			/* 发送缓冲区大小 */
	g_tUart1.usRxBufSize = RX_BUF_SIZE;			/* 接收缓冲区大小 */
	g_tUart1.usTxWrite = 0;						/* 发送FIFO写索引 */
	g_tUart1.usTxRead = 0;						/* 发送FIFO读索引 */
	g_tUart1.usRxWrite = 0;						/* 接收FIFO写索引 */
	g_tUart1.usRxRead = 0;						/* 接收FIFO读索引 */
	g_tUart1.usRxCount = 0;						/* 接收到的新数据个数 */
	g_tUart1.usTxCount = 0;						/* 待发送的数据个数 */
	g_tUart1.ReciveNew = MODS_ReceieveNew;		/* 接收到新数据后的回调函数 */
}
/*串口中断函数配置*/
void NVIC_UsartConfig(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	/* 使能串口1中断 */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}
/*串口配置*/
void USART1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	//开启 GPIO ,GPIO复用，串口时钟
	RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_APB2Periph_USART1,ENABLE);
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10复用为USART1
	//USART1端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA9，PA10
	   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = RATE_BOUD;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART1, &USART_InitStructure); //初始化串口1
	
	USART_Cmd(USART1, ENABLE);  //使能串口1 
	
	USART_ClearFlag(USART1, USART_FLAG_TC);
	//使能中断
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}
/*串口中断配置*/
void USART1_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	/* 配置中断优先级组 */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	/* 使能串口中断 */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; /*抢占式优先级*/
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  /*响应优先级*/
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
//输出一个字符
void uart1_putchar(char c)
{
	USART_SendData(USART1, c);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC)!=SET);
}
//输出一个字符串
void uart1_print(char * str)
{
	while(*str!='\0')
		uart1_putchar(*str++);
	
}
char uart1_getchar(void)
{
	char temp=0;
	while(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)!= SET);
	temp = USART_ReceiveData(USART1);
	return temp;
}
char uart1_getstr(unsigned char* buff,char len)
{
	int i=0;
	for(i=0;i<len;i++)
	{
		buff[i]=uart1_getchar();
	}
	return len;
}
//输出重定向
int fputc(int ch,FILE * f)
{
	USART_SendData(USART1,ch);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);
	return ch;
}

//输入重定向
int fgetc(FILE*f)
{
	while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE)==RESET);
	return (uint8_t)(USART_ReceiveData(USART1));
}
void USART1_InitConfig()
{
	UartVarInit();
	USART1_Config();
	USART1_NVIC_Configuration();
}
//输出一个字符
static void USART1_Putchar(UART_T *_pUart,uint8_t c)
{
	USART_SendData(_pUart->uart, c);
	while(USART_GetFlagStatus(_pUart->uart, USART_FLAG_TC)!=SET);
}
//输出一个字符串
void USART1_Senddata(UART_T *_pUart,uint8_t * str,uint16_t str_len)
{
	uint8_t i = 0;
	for(i = 0;i < str_len ;i++)
	{
		USART1_Putchar(_pUart,*(str + i));
	}
}
/*****modbus输出一个字节******/
void MODS_SendData(UART_T *_pUart,uint8_t * str,uint16_t str_len)
{
	USART1_Senddata( _pUart, str, str_len);
}

/*串口接收中断函数*/
void UartIRQ(UART_T *_pUart)
{
	uint8_t ch = 0;
	if (USART_GetITStatus(_pUart->uart, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(_pUart->uart,USART_IT_RXNE);
		ch = USART_ReceiveData(_pUart->uart);
		_pUart->usRxWrite =_pUart->usRxWrite % RX_BUF_SIZE;
		_pUart->pRxBuf[(_pUart->usRxWrite ++)] = ch;
		_pUart->usRxCount ++;		//接收字符的长度
		modbus_recv_gap_flag = 0;   //该标志位1，表示一帧数据接收完成
	//	_pUart->ReciveNew(data);		//该函数执行有问题，不知道为什么
	}
}


void MODS_ReceieveNewData(UART_T *_pUart)
{
	uint8_t i,frame_start = 0;
	if((_pUart->usRxCount < 8) ||(modbus_recv_gap_flag == 0))  //接收到字符数据长度小于8,或者未接受到一个完整帧
	{
		return ;
	}
	
	frame_start = (_pUart->usRxWrite - _pUart->usRxCount + RX_BUF_SIZE) % RX_BUF_SIZE;
	for( i= 0;i < _pUart->usRxCount;i++)
	{
		g_tModS.RxBuf[g_tModS.RxCount++] = _pUart->pRxBuf[(frame_start + i) % RX_BUF_SIZE] ;
	}
	_pUart->usRxCount =0 ;//接收完一帧数据，将接收数据长度清零。
	
}
