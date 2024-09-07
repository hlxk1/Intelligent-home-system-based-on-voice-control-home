#ifndef _DUOJI_H
#define _DUOJI_H

#include "stm32f4xx.h"
#include "stdio.h"
#include "stdbool.h"
#include "LED.h"
//#include "delay.h"
#include "systick.h"
#include "KEY.h"
#include "EXTI.h"
//#include "USART.h"
#include "uart.h"
//#include "mymisson.h"
#include "TIM.h"
#include "PWM.h"
#include "IWDG.h"
#include "WWDG.h"
#include "RTC.h"
#include "ADC.h"
#include "DHT11.h"
//#include "IIC.h"
//#include "I2C.h"
#include <stdlib.h>

extern uint8_t s;

void PWM_TIM3_CH1(void);//配置引脚复用，定时器配置
void DUOJI_STOP(void);// 占空比取中，舵机停止
void DUOJI_SHUN(void); // 舵机顺时针转
void DUOJI_NI(void); // 舵机逆时针转
void KEY_OPEN_CLOSE_STOP(void);//按键1 舵机顺时针，按键2逆时针 按键3 停止 // 可用于窗帘
void ADC_LIGHT_OPEN_CLOSE(void);//通过光强控制舵机
#endif
