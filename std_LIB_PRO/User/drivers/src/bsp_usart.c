/**
  ******************************************************************************
  * @file    bsp_usart1.c
  * @author  HU WENYAO
  * @version V1.0
  * @date    2019-04-21
  * @brief   ����1���պ���
  ******************************************************************************
  */
#include "bsp_usart.h"
#include "hoo_modbuscomm.h"
/* ����ÿ�����ڽṹ����� */
uint16_t modbus_recv_gap_flag = 0; //����3.5���ַ���ʱ����������֡���ݣ�2ms����
UART_T g_tUart1;
static uint8_t g_TxBuf1[TX_BUF_SIZE];		/* ���ͻ����� */
static uint8_t g_RxBuf1[RX_BUF_SIZE];		/* ���ջ����� */
/*
*********************************************************************************************************
*	�� �� ��: UartVarInit
*	����˵��: ��ʼ��������صı���
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void UartVarInit(void)
{
	g_tUart1.uart = USART1;						/* STM32 �����豸 */
	g_tUart1.pTxBuf = g_TxBuf1;					/* ���ͻ�����ָ�� */
	g_tUart1.pRxBuf = g_RxBuf1;					/* ���ջ�����ָ�� */
	g_tUart1.usTxBufSize = TX_BUF_SIZE;			/* ���ͻ�������С */
	g_tUart1.usRxBufSize = RX_BUF_SIZE;			/* ���ջ�������С */
	g_tUart1.usTxWrite = 0;						/* ����FIFOд���� */
	g_tUart1.usTxRead = 0;						/* ����FIFO������ */
	g_tUart1.usRxWrite = 0;						/* ����FIFOд���� */
	g_tUart1.usRxRead = 0;						/* ����FIFO������ */
	g_tUart1.usRxCount = 0;						/* ���յ��������ݸ��� */
	g_tUart1.usTxCount = 0;						/* �����͵����ݸ��� */
	g_tUart1.ReciveNew = MODS_ReceieveNew;		/* ���յ������ݺ�Ļص����� */
}
/*�����жϺ�������*/
void NVIC_UsartConfig(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	/* ʹ�ܴ���1�ж� */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}
/*��������*/
void USART1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	//���� GPIO ,GPIO���ã�����ʱ��
	RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_APB2Periph_USART1,ENABLE);
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10����ΪUSART1
	//USART1�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA9��PA10
	   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = RATE_BOUD;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	
	USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1 
	
	USART_ClearFlag(USART1, USART_FLAG_TC);
	//ʹ���ж�
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}
/*�����ж�����*/
void USART1_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	/* �����ж����ȼ��� */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	/* ʹ�ܴ����ж� */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; /*��ռʽ���ȼ�*/
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  /*��Ӧ���ȼ�*/
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
//���һ���ַ�
void uart1_putchar(char c)
{
	USART_SendData(USART1, c);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC)!=SET);
}
//���һ���ַ���
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
//����ض���
int fputc(int ch,FILE * f)
{
	USART_SendData(USART1,ch);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);
	return ch;
}

//�����ض���
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
//���һ���ַ�
static void USART1_Putchar(UART_T *_pUart,uint8_t c)
{
	USART_SendData(_pUart->uart, c);
	while(USART_GetFlagStatus(_pUart->uart, USART_FLAG_TC)!=SET);
}
//���һ���ַ���
void USART1_Senddata(UART_T *_pUart,uint8_t * str,uint16_t str_len)
{
	uint8_t i = 0;
	for(i = 0;i < str_len ;i++)
	{
		USART1_Putchar(_pUart,*(str + i));
	}
}
/*****modbus���һ���ֽ�******/
void MODS_SendData(UART_T *_pUart,uint8_t * str,uint16_t str_len)
{
	USART1_Senddata( _pUart, str, str_len);
}

/*���ڽ����жϺ���*/
void UartIRQ(UART_T *_pUart)
{
	uint8_t ch = 0;
	if (USART_GetITStatus(_pUart->uart, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(_pUart->uart,USART_IT_RXNE);
		ch = USART_ReceiveData(_pUart->uart);
		_pUart->usRxWrite =_pUart->usRxWrite % RX_BUF_SIZE;
		_pUart->pRxBuf[(_pUart->usRxWrite ++)] = ch;
		_pUart->usRxCount ++;		//�����ַ��ĳ���
		modbus_recv_gap_flag = 0;   //�ñ�־λ1����ʾһ֡���ݽ������
	//	_pUart->ReciveNew(data);		//�ú���ִ�������⣬��֪��Ϊʲô
	}
}


void MODS_ReceieveNewData(UART_T *_pUart)
{
	uint8_t i,frame_start = 0;
	if((_pUart->usRxCount < 8) ||(modbus_recv_gap_flag == 0))  //���յ��ַ����ݳ���С��8,����δ���ܵ�һ������֡
	{
		return ;
	}
	
	frame_start = (_pUart->usRxWrite - _pUart->usRxCount + RX_BUF_SIZE) % RX_BUF_SIZE;
	for( i= 0;i < _pUart->usRxCount;i++)
	{
		g_tModS.RxBuf[g_tModS.RxCount++] = _pUart->pRxBuf[(frame_start + i) % RX_BUF_SIZE] ;
	}
	_pUart->usRxCount =0 ;//������һ֡���ݣ����������ݳ������㡣
	
}
