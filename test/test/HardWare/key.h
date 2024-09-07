#ifndef _KEY_H
#define _KEY_H

#include "stm32f4xx.h"
#include "LED.h"
#include "systick.h"



// ÎªÁË·½±ãÒÆÖ²
#define KEY1_PIN GPIO_Pin_2
#define KEY2_PIN GPIO_Pin_3
#define KEY3_PIN GPIO_Pin_4
#define KEY4_PIN GPIO_Pin_5
void KEY_config(void);
uint8_t KEY_Scanf(GPIO_TypeDef*, uint16_t);
void KEY_CONTROL_LED(void);
void KEY_LED_count(GPIO_TypeDef * ,uint16_t,uint16_t);
void KEY_LED_passwd(GPIO_TypeDef *);
#endif 
