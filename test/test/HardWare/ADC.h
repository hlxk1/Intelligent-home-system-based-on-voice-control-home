#ifndef _ADC_H
#define _ADC_H

#include "stm32f4xx.h"
#include "stdio.h"
#include "stdbool.h"
#include "LED.h"
#include "systick.h"
#include "uart.h"
#include "KEY.h"
#include "EXTI.h"
//#include "USART.h"
//#include "mymisson.h"
#include "TIM.h"
#include "PWM.h"
#include "IWDG.h"
#include "WWDG.h"
#include "RTC.h"
#include "DHT11.h"
//#include "IIC.h"
//#include "I2C.h"
#include <stdlib.h>
#include "DUOJI.h"
#include "light_adc.h"
#include "OLED.h"

extern float r;
extern float v; 
extern uint8_t light;

/* 外部接口函数声明 */
void ADC3_Config(void);
uint16_t Get_Adc(ADC_TypeDef *ADCx, uint8_t ADC_Channel);
uint16_t Get_Adc_Average(ADC_TypeDef *ADCx, uint8_t ADC_Channel, uint8_t times);
float Get_Conversion_value(ADC_TypeDef *ADCx, uint8_t ADC_Channel, uint8_t times);
uint8_t GET_light(void); // 利用光敏获取的值

#endif
