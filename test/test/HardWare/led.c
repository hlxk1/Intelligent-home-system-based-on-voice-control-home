#include "LED.h"


void LED_config(void){
	GPIO_InitTypeDef GPIOG_InitStruct;
	// 1.开启GPIO对应的时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
  // 2.配置GPIO引脚为推挽输出
	GPIOG_InitStruct.GPIO_Pin  = LED1_PIN|LED2_PIN |LED3_PIN |LED4_PIN;    // 对应14号引脚
  GPIOG_InitStruct.GPIO_Mode = GPIO_Mode_OUT;		//输出模式
  GPIOG_InitStruct.GPIO_Speed = GPIO_Medium_Speed; 							//中速
  GPIOG_InitStruct.GPIO_OType = GPIO_OType_PP;	//推挽类型
  GPIOG_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;	//上拉
	GPIO_Init(GPIOG, &GPIOG_InitStruct);
	// 配置好后灯就是亮灯的形式了
	// 3.输出灯高电平 关灯
	GPIO_SetBits(GPIOG, LED1_PIN|LED2_PIN|LED4_PIN|LED3_PIN);
	}
// 呼吸灯 //依据PWM技术，使其高电平输出越来越少，一亮一灭时间长短：比例 500：0


int light2 = 1000;
int mie = 1;	
void All_LED_breath(void){
		while(light2 != 1 && mie != 1000){
			GPIO_ResetBits(GPIOG, LED1_PIN|LED2_PIN |LED3_PIN |LED4_PIN); // 亮
			Delay_us(light2--);
			GPIO_SetBits(GPIOG, LED1_PIN|LED2_PIN |LED3_PIN |LED4_PIN);
			Delay_us(mie++);
		}

	while(light2 != 1000 && mie != 1){
			GPIO_ResetBits(GPIOG, LED1_PIN|LED2_PIN |LED3_PIN |LED4_PIN); // 亮
			Delay_us(light2++);
			GPIO_SetBits(GPIOG, LED1_PIN|LED2_PIN |LED3_PIN |LED4_PIN);
			Delay_us(mie--);
		}
}
void LED_breath(void){
		while(light2 != 1 && mie != 1000){
			GPIO_ResetBits(GPIOG, LED1_PIN); // 亮
			Delay_us(light2--);
			GPIO_SetBits(GPIOG, LED1_PIN);
			Delay_us(mie++);
		}

	while(light2 != 1000 && mie != 1){
			GPIO_ResetBits(GPIOG, LED1_PIN); // 亮
			Delay_us(light2++);
			GPIO_SetBits(GPIOG, LED1_PIN);
			Delay_us(mie--);


		}
//		GPIO_ResetBits(GPIOG, LED1_PIN);
//		Delay_ms(mie++);
//		GPIO_SetBits(GPIOG, LED1_PIN);
//		Delay_ms(5);
//		GPIO_ResetBits(GPIOG, LED1_PIN);
//		Delay_ms(40);
//		GPIO_SetBits(GPIOG, LED1_PIN);
//		Delay_ms(10);
//		GPIO_ResetBits(GPIOG, LED1_PIN);
//		Delay_ms(35);
//		GPIO_SetBits(GPIOG, LED1_PIN);
//		Delay_ms(15);	
//		GPIO_ResetBits(GPIOG, LED1_PIN);
//		Delay_ms(30);
//		GPIO_SetBits(GPIOG, LED1_PIN);
//		Delay_ms(20);	
//		GPIO_ResetBits(GPIOG, LED1_PIN);
//		Delay_ms(25);
//		GPIO_SetBits(GPIOG, LED1_PIN);
//		Delay_ms(25);
//		GPIO_ResetBits(GPIOG, LED1_PIN);
//		Delay_ms(20);
//		GPIO_SetBits(GPIOG, LED1_PIN);
//		Delay_ms(30);
//		GPIO_ResetBits(GPIOG, LED1_PIN);
//		Delay_ms(15);
//		GPIO_SetBits(GPIOG, LED1_PIN);
//		Delay_ms(35);
//		GPIO_ResetBits(GPIOG, LED1_PIN);
//		Delay_ms(10);
//		GPIO_SetBits(GPIOG, LED1_PIN);
//		Delay_ms(40);
//		GPIO_ResetBits(GPIOG, LED1_PIN);
//		Delay_ms(45);
//		GPIO_SetBits(GPIOG, LED1_PIN);
//		Delay_ms(5);
//		GPIO_ResetBits(GPIOG, LED1_PIN);
//		Delay_ms(50);
//		GPIO_SetBits(GPIOG, LED1_PIN);
//		Delay_ms(0);
}
// 流水灯
void LED_Stream(uint32_t ms){
 	while(1){
			GPIO_ResetBits(GPIOG, LED1_PIN);
			Delay_ms(ms);
			GPIO_SetBits(GPIOG, LED1_PIN);
			Delay_ms(ms);
			GPIO_ResetBits(GPIOG, LED2_PIN);
			Delay_ms(ms);
			GPIO_SetBits(GPIOG, LED2_PIN);
			Delay_ms(ms);
			GPIO_ResetBits(GPIOG, LED3_PIN);
			Delay_ms(ms);
			GPIO_SetBits(GPIOG, LED3_PIN);
			Delay_ms(ms);
			GPIO_ResetBits(GPIOG, LED4_PIN);
			Delay_ms(ms);
			GPIO_SetBits(GPIOG, LED4_PIN);
			Delay_ms(ms);

	}
}
int x ;
//void LED_differ_Stream(int i){
//  	//while(1){
//			GPIO_ResetBits(GPIOG, LED1_PIN);
//			delay_200ms(i);
//			GPIO_SetBits(GPIOG, LED1_PIN);
//			delay_200ms(i);
//			GPIO_ResetBits(GPIOG, LED2_PIN);
//			delay_200ms(i);
//			GPIO_SetBits(GPIOG, LED2_PIN);
//			delay_200ms(i);
//			GPIO_ResetBits(GPIOG, LED3_PIN);
//			delay_200ms(i);
//			GPIO_SetBits(GPIOG, LED3_PIN);
//			delay_200ms(i);
//			GPIO_ResetBits(GPIOG, LED4_PIN);
//			delay_200ms(i);
//			GPIO_SetBits(GPIOG, LED4_PIN);
//			delay_200ms(i);

//	//}
//}
