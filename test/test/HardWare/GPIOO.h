#ifndef _GPIOO_H
#define _GPIOO_H

#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "LED.h"
//#include "delay.h"
#include "KEY.h"
#include "EXTI.h"
//#include "USART.h"
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
#include "DUOJI.h"
// #include "light_adc.h"
#include "OLED.h"
//#include "WIFI.h"

void DCMI_D5_pinB6(void);
void DCMI_D7_pine6(void);
void DUOJI_pin(void);
void LED_pin(void);


#endif
