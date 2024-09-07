#ifndef __IIC_H
#define __IIC_H

#include "stm32f4xx.h"
#include "systick.h"


#define iic_delay(nus)	Delay_us(nus)	//延时1us

/*  AT24C02模块对应的引脚   */
//  IIC_SCL ---  PB8   
//  IIC_SDA  --- PB9
//#define SCL_PORT    	GPIOB
//#define SCL_PIN     	GPIO_Pin_8             

//#define SDA_PORT		  GPIOB
//#define SDA_PIN     	GPIO_Pin_9     
//#define RCC_AHB1Periph_GPIOX RCC_AHB1Periph_GPIOB
//#define SCL_CLK		RCC_AHB1Periph_GPIOB
//#define SDA_CLK		RCC_AHB1Periph_GPIOB

/*   Sentry2模块对应的引脚   */
//  IIC_SCL ---  PD4   
//  IIC_SDA  --- PD3
#define SCL_PORT    	GPIOD
#define SCL_PIN     	GPIO_Pin_4             

#define SDA_PORT		  GPIOD
#define SDA_PIN     	GPIO_Pin_3     
#define RCC_AHB1Periph_GPIOX RCC_AHB1Periph_GPIOD
#define SCL_CLK		RCC_AHB1Periph_GPIOD
#define SDA_CLK		RCC_AHB1Periph_GPIOD

//指的是设置SCL时钟线的电平
#define  IIC_SCL(n)  (n) ? GPIO_SetBits(SCL_PORT,SCL_PIN) : GPIO_ResetBits(SCL_PORT,SCL_PIN)

//指的是设置SDA数据线的电平
#define  IIC_SDA(n)  (n) ? GPIO_SetBits(SDA_PORT,SDA_PIN) : GPIO_ResetBits(SDA_PORT,SDA_PIN)

//指的是读取SDA数据线的电平
#define  IIC_SDA_READ  	 GPIO_ReadInputDataBit(SDA_PORT,SDA_PIN)

void IIC_init(void);
void IIC_SetSDAOutputMode(void);
void IIC_SetSDAInputMode(void);
void IIC_Start(void);
void IIC_Stop(void);
void IIC_SendByte(uint8_t data) ;
uint8_t IIC_Wait_Ack(void);
void IIC_MasterAck(uint8_t ack);
uint8_t IIC_ReadByte(void);

#endif


