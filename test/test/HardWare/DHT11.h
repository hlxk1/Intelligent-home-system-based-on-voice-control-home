#ifndef __DHT11_H
#define __DHT11_H

//包含头文件
#include "stm32f4xx.h"
#include "systick.h"
#include "uart.h"
#include <stdio.h>
#include "oled.h"
#include "esp8266.h"
#include "mqtt.h"
//宏定义
/* 可以在下面的宏定义中把后面的延时函数替换换SysTick的延时函数，就是想用那个就换成那个的 */
#define DHT11_DELAY_US( nus)	Delay_us( nus)	//延时10us
#define DHT11_DELAY_MS( nms)	Delay_ms( nms)

#define DHT11_HIGH				1
#define DHT11_LOW				0

#define DHT11_CLK				RCC_AHB1Periph_GPIOG
#define DHT11_PIN				GPIO_Pin_9             
#define DHT11_PORT			GPIOG


#define DHT11_DATA_IN()			GPIO_ReadInputDataBit(DHT11_PORT,DHT11_PIN)

#define DHT11_DATA_OUT(a)		if (a)								\
								GPIO_SetBits(DHT11_PORT,DHT11_PIN);	\
								else								\
								GPIO_ResetBits(DHT11_PORT,DHT11_PIN);

//变量声明
typedef struct
{
	uint8_t  humi_int;			//湿度的整数部分
	uint8_t  humi_deci;	 		//湿度的小数部分
	uint8_t  temp_int;	 		//温度的整数部分
	uint8_t  temp_deci;	 		//温度的小数部分
	uint8_t  check_sum;	 		//校验和
}DHT11_Data_TypeDef;


struct DHT11_Complete_Data_TypeDef
{
	float humi;    //湿度的完整数据
	float temp;	   //温度的完整数据
};


extern char data_buf[12];
extern DHT11_Data_TypeDef DHT11_Data;	//温湿度结构体

//函数声明
void DHT11_Config(void);
uint8_t Read_DHT11(DHT11_Data_TypeDef *DHT11_Data);
struct DHT11_Complete_Data_TypeDef get_dht11_data(void);

#endif
