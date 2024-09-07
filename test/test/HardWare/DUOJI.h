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

void PWM_TIM3_CH1(void);//�������Ÿ��ã���ʱ������
void DUOJI_STOP(void);// ռ�ձ�ȡ�У����ֹͣ
void DUOJI_SHUN(void); // ���˳ʱ��ת
void DUOJI_NI(void); // �����ʱ��ת
void KEY_OPEN_CLOSE_STOP(void);//����1 ���˳ʱ�룬����2��ʱ�� ����3 ֹͣ // �����ڴ���
void ADC_LIGHT_OPEN_CLOSE(void);//ͨ����ǿ���ƶ��
#endif
