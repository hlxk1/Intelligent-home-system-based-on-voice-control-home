#ifndef __RTC_H
#define	__RTC_H

#include "stm32f4xx.h"

void RTC_CLK_Config(void);
void RTC_Config(void);
void RTC_Time_Config(void);
void RTC_Alarm_Config(void);
void RTC_Set_WakeUp(void);

#endif
