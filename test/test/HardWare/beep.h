#ifndef __BEEP_H
#define	__BEEP_H

#include "stm32f4xx.h"


#define	ON		0
#define	OFF		1


/* ���κ꣬��������������һ��ʹ�� */
#define BEEP(a)		if(a) GPIO_SetBits(GPIOB,GPIO_Pin_5); else GPIO_ResetBits(GPIOB,GPIO_Pin_5)


void BEEP_GPIO_Config(void);

#endif
