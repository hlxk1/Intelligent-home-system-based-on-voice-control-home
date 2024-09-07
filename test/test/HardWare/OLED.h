#ifndef __OLED_H
#define __OLED_H


#include "stm32f4xx.h"
//#include "Hard_IIC.h"
#include "HARDIIC.h"
#include "systick.h"
#include <stdio.h>
#include "OLEDFONT.h"
#include "string.h"
extern  const unsigned char HZK16[14][32];
extern  const unsigned char HZK17[36][32];

#define OLED_DEV_ADDR 0x78

#define OLED_CMD  0	    //Ð´ÃüÁî
#define OLED_DATA 0x40	//Ð´Êý¾Ý




#define u8 unsigned char
#define u32 unsigned int




void oled_init(void);
void OLED_ShowChinese(u8 x, u8 y, u8 index);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size1);
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size1);
unsigned char utf8_to_gb2312(const unsigned char *utf8_char);
//unsigned char get16ChineseIndex(const unsigned int *str);

//void OLED_Show16ChineseString(u8 x, u8 y, const unsigned int *str);
unsigned char get16ChineseIndex(const char *str);
void OLED_Show16ChineseString(u8 x, u8 y, const char *str);

void OLED_DrawPoint(u8 x,u8 y);
void OLED_ClearPoint(u8 x,u8 y);
//void OLED_ShowChineseString(u8 x, u8 y, const u8* str);
void OLED_ShowChineseString(u8 x, u8 y, const u8 *str);
#endif
