#ifndef __AT24C02_H
#define	__AT24C02_H

#include "stm32f4xx.h"
#include <stdio.h>
#include "systick.h"


//指的是AT24C02的器件地址以及数据传输方向
#define  AT24C02_ADDRESS_WRITE	0xA0	// 1010 0000
#define  AT24C02_ADDRESS_READ	0xA1	// 1010 0001

void AT24C02_ByteWrite(uint8_t Address,uint8_t Data);
void AT24C02_PageWrite(uint8_t PageAddress,uint8_t DataSize,uint8_t *DataPtr);
uint8_t AT24C02_CurrentAddressRead(void);
uint8_t AT24C02_ReadByte(uint8_t Address);
void AT24C02_RW(void);

#endif
