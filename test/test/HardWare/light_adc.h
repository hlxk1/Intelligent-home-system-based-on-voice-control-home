#ifndef __LIGHT_ADC_H__
#define __LIGHT_ADC_H__

/* �����ٷ�ͷ�ļ� */
#include "stm32f4xx.h"

/* ��ر������� */
typedef struct
{
	unsigned short ohm; // �����������ֵ

	unsigned short lux; // ���ն�

} PhotoRes_TypeDef;

extern uint16_t LUX; // ���ն�

/* �ⲿ�ӿں������� */
uint16_t GetLux(float PhotoResistor);

#endif
