#ifndef __BSP_BLUETOOTH_H
#define __BSP_BLUETOOTH_H

#include "stm32f4xx.h"
#define BLUE_TX   GPIO_Pin_10
#define BLUE_RX   GPIO_Pin_11
#define BLUE_RTS  GPIO_Pin_13
#define BLUE_CTS  GPIO_Pin_14
#define BLUE_PORT GPIOB


typedef enum CMD{
	TEMP =0x60,
	HUMI =0x61,
	LUMI =0x22,
}BLE_CMD;
static char data[8];

void BLUETOOTH_Init(void);
void UART3_InitConfig(void);
void UASRT_NVIC_Configuration(void);
void BLE_On(void);
char BLE_SendData(char * str);
char BLE_RecvData(void);
char data_pack(char a[],u8 i,u8 f,BLE_CMD cmd);
char data_pack_lumihar(char a[],uint16_t data);

void uart3_putchar(char c);
void uart3_senddata(char * str);
char usart3_getc(void);
int usart3_getstr(unsigned char* buff,int len);

#endif
