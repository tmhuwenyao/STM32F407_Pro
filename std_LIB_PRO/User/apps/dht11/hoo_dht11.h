#ifndef __HOO_DHT11_H
#define __HOO_DHT11_H

#include "stm32f4xx.h"
#define DHT11_PIN   GPIO_Pin_8
#define DHT11_GPIO  GPIOB
#define DHT11_CLK   RCC_AHB1Periph_GPIOB

extern uint8_t humi_i,humi_f;
extern uint8_t temp_i,temp_f;

void DHT11_Out(void);
void DHT11_In(void);


void DHT11_InitConfig(void);
char dht11_read_data(u8* humi_i,u8* humi_f,u8* temp_i,u8* temp_f);
char dht11_read_byte(void);
char dht11_read_bit(void);
char dht11_check(void);
void dht11_rst(void);

#endif

