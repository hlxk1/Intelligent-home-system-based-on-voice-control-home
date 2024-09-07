#ifndef _LED_H
#define _LED_H

#include "stm32f4xx.h"
#include "systick.h"
#include "KEY.h"

// ÎªÁË·½±ãÒÆÖ²
#define LED1_PIN GPIO_Pin_14
#define LED2_PIN GPIO_Pin_13
#define LED3_PIN GPIO_Pin_6
#define LED4_PIN GPIO_Pin_11
#define LED_PIN_GROUP GPIOG
#define LED_Colock RCC_AHB1Periph_GPIOG

void LED_config(void);
void LED_Stream(uint32_t);
void LED_differ_Stream(  int);
void LED_breath(void);
void All_LED_breath(void);

#endif
