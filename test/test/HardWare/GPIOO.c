#include "GPIOO.h"
//PB6/TIM4_CH1/U1_TX/CAN2_TX/I2C1_SCL/DCMI_D5 舵机
//PE6/TRACED3/FSMC_A22/TIM9_CH2/DCMI_D7  灯光
void DCMI_D5_pinB6(void)
{
	GPIO_InitTypeDef GPIOB_InitStruct;
	// 1.开启GPIO对应的时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  // 2.配置GPIO引脚为推挽输出
	GPIOB_InitStruct.GPIO_Pin  = GPIO_Pin_6;    // 对应14号引脚
  GPIOB_InitStruct.GPIO_Mode = GPIO_Mode_IN;		//输入模式
//  GPIOG_InitStruct.GPIO_OType = GPIO_OType_PP;	//推挽类型
  GPIOB_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;	//上拉
	GPIO_Init(GPIOB, &GPIOB_InitStruct);
}
void DCMI_D7_pine6(void){
	GPIO_InitTypeDef GPIOE_InitStruct;
	// 1.开启GPIO对应的时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
  // 2.配置GPIO引脚为推挽输出
	GPIOE_InitStruct.GPIO_Pin  = GPIO_Pin_6;    // 对应14号引脚
  GPIOE_InitStruct.GPIO_Mode = GPIO_Mode_IN;		//输入模式
//  GPIOG_InitStruct.GPIO_OType = GPIO_OType_PP;	//推挽类型
  GPIOE_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;	//上拉
	GPIO_Init(GPIOE, &GPIOE_InitStruct);
}
int DUOJI_open = 0;
int DUOJI_close = 0;
void DUOJI_pin(void){
	if(DUOJI_open != 1){
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6) == SET){
			DUOJI_SHUN();
			Delay_ms(500);
			Delay_ms(500);
			Delay_ms(500);
			Delay_ms(500);	
			DUOJI_STOP();
			DUOJI_open = 1;
			DUOJI_close = 0;
		}
	}
	if(DUOJI_close != 1){
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6) == RESET){
			DUOJI_NI();
			Delay_ms(500);
			Delay_ms(500);
			Delay_ms(500);
			Delay_ms(500);
			DUOJI_STOP();
			DUOJI_open = 0;
			DUOJI_close = 1;
		}
	}
	

}
void LED_pin(void){
	if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_6) == SET){
		GPIO_SetBits(GPIOG, LED1_PIN|LED2_PIN);
	}else if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_6) == RESET){
		GPIO_ResetBits(GPIOG, LED1_PIN|LED2_PIN);
	}
}
