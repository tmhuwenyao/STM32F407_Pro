#ifndef __BSP_UART_H
#define __BSP_UART_H

#include "stm32f4xx.h"
#include "stdio.h"

#define 	RATE_BOUD 	115200
/* ����ȫ���жϵĺ� */
#define ENABLE_INT()	__set_PRIMASK(0)	/* ʹ��ȫ���ж� */
#define DISABLE_INT()	__set_PRIMASK(1)	/* ��ֹȫ���ж� */

/*����1 ʹ��*/
#define USART_PRINTF_ENABLE			1		//����printf ���ʹ��

/*���ڻ�������С*/
#define TX_BUF_SIZE			255
#define RX_BUF_SIZE			255
/* �����豸�ṹ�� */

typedef struct
{
	USART_TypeDef *uart;		/* STM32�ڲ������豸ָ�� */
	uint8_t *pTxBuf;			/* ���ͻ����� */
	uint8_t *pRxBuf;			/* ���ջ����� */
	uint16_t usTxBufSize;		/* ���ͻ�������С */
	uint16_t usRxBufSize;		/* ���ջ�������С */
	__IO uint16_t usTxWrite;			/* ���ͻ�����дָ�� */
	__IO uint16_t usTxRead;			/* ���ͻ�������ָ�� */
	__IO uint16_t usTxCount;			/* �ȴ����͵����ݸ��� */

	__IO uint16_t usRxWrite;			/* ���ջ�����дָ�� */
	__IO uint16_t usRxRead;			/* ���ջ�������ָ�� */
	__IO uint16_t usRxCount;			/* ��δ��ȡ�������ݸ��� */

	void (*SendBefor)(void); 	/* ��ʼ����֮ǰ�Ļص�����ָ�루��Ҫ����RS485�л�������ģʽ�� */
	void (*SendOver)(void); 	/* ������ϵĻص�����ָ�루��Ҫ����RS485������ģʽ�л�Ϊ����ģʽ�� */
	void (*ReciveNew)(uint8_t _byte);	/* �����յ����ݵĻص�����ָ�� */
}UART_T;

extern UART_T 		g_tUart1;
extern void MODS_SendData(UART_T *_pUart,uint8_t * str,uint16_t str_len);  //MODBUS ��������
extern void MODS_ReceieveNewData(UART_T *_pUart);  //modbus����һ֡����
extern void UartIRQ(UART_T *_pUart);

void USART1_Config(void);
void USART1_InitConfig(void);
void USART1_NVIC_Configuration(void);
void uart1_putchar(char c);
void uart1_print(char * str);

char uart1_getchar(void);
char uart1_getstr(unsigned char* buff,char len);

#endif

