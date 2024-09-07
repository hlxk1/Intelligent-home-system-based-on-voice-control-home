#ifndef __SYSTICK_H__
#define __SYSTICK_H__

#include "stm32f4xx.h"

void SysTick_Init(void);

void Delay_us(__IO uint32_t nTime);
void TimingDelay_Decrement(void);

#define Delay_ms(x) Delay_us(1000*x)

#endif
