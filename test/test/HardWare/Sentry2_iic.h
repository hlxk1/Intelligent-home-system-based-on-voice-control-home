#ifndef __SENTRY2_IIC_H
#define __SENTRY2_IIC_H

#include "stm32f4xx.h"
#include <stdio.h>
#include "systick.h"
#include "IIC.h"
void Sentry2_Init(void);
void Sentry2_ByteWrite(uint8_t Address,uint8_t Data);
uint8_t Sentry2_ReadByte(uint8_t Address);
void Sentry2_test(void);

#endif

