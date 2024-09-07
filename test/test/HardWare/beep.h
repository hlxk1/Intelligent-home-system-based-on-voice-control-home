#ifndef __BEEP_H
#define	__BEEP_H

#include "stm32f4xx.h"


#define	ON		0
#define	OFF		1


/* 带参宏，可以像内联函数一样使用 */
#define BEEP(a)		if(a) GPIO_SetBits(GPIOB,GPIO_Pin_5); else GPIO_ResetBits(GPIOB,GPIO_Pin_5)


void BEEP_GPIO_Config(void);

#endif
