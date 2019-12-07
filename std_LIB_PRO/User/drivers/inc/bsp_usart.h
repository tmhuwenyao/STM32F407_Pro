#ifndef __BSP_UART_H
#define __BSP_UART_H

#include "stm32f4xx.h"
#include "stdio.h"

#define 	RATE_BOUD 	115200
/* 开关全局中断的宏 */
#define ENABLE_INT()	__set_PRIMASK(0)	/* 使能全局中断 */
#define DISABLE_INT()	__set_PRIMASK(1)	/* 禁止全局中断 */

/*串口1 使能*/
#define USART_PRINTF_ENABLE			1		//串口printf 输出使能

/*串口缓冲区大小*/
#define TX_BUF_SIZE			255
#define RX_BUF_SIZE			255
/* 串口设备结构体 */

typedef struct
{
	USART_TypeDef *uart;		/* STM32内部串口设备指针 */
	uint8_t *pTxBuf;			/* 发送缓冲区 */
	uint8_t *pRxBuf;			/* 接收缓冲区 */
	uint16_t usTxBufSize;		/* 发送缓冲区大小 */
	uint16_t usRxBufSize;		/* 接收缓冲区大小 */
	__IO uint16_t usTxWrite;			/* 发送缓冲区写指针 */
	__IO uint16_t usTxRead;			/* 发送缓冲区读指针 */
	__IO uint16_t usTxCount;			/* 等待发送的数据个数 */

	__IO uint16_t usRxWrite;			/* 接收缓冲区写指针 */
	__IO uint16_t usRxRead;			/* 接收缓冲区读指针 */
	__IO uint16_t usRxCount;			/* 还未读取的新数据个数 */

	void (*SendBefor)(void); 	/* 开始发送之前的回调函数指针（主要用于RS485切换到发送模式） */
	void (*SendOver)(void); 	/* 发送完毕的回调函数指针（主要用于RS485将发送模式切换为接收模式） */
	void (*ReciveNew)(uint8_t _byte);	/* 串口收到数据的回调函数指针 */
}UART_T;

extern UART_T 		g_tUart1;
extern void MODS_SendData(UART_T *_pUart,uint8_t * str,uint16_t str_len);  //MODBUS 发送数据
extern void MODS_ReceieveNewData(UART_T *_pUart);  //modbus接收一帧数据
extern void UartIRQ(UART_T *_pUart);

void USART1_Config(void);
void USART1_InitConfig(void);
void USART1_NVIC_Configuration(void);
void uart1_putchar(char c);
void uart1_print(char * str);

char uart1_getchar(void);
char uart1_getstr(unsigned char* buff,char len);

#endif

