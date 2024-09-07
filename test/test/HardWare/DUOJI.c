#include "DUOJI.h"
//PC6/TIM3_CH1/TIM8_CH1/U6_TX/SDIO_D6/DCMI_D0/I2S2_MCK

void PWM_TIM3_CH1(void){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM3); // 配置引脚位定时器	
	
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF; // 复用模式
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;// 推挽因为要有高低电平
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_High_Speed;
	
	GPIO_Init(GPIOC,&GPIO_InitStruct);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 1999; //2000
	TIM_TimeBaseInitStruct.TIM_Prescaler =	840 - 1;
//	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 
	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);
	
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = 0;
	TIM_OC1Init(TIM3, &TIM_OCInitStruct);
	
	TIM_Cmd(TIM3,ENABLE);
	
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable); // 预装载
	TIM_ARRPreloadConfig(TIM3, ENABLE); // ARR 使能
 
	
//	0.01s
//	舵机的控制：
//舵机的控制一般需要一个 20ms 左右的时基脉冲，该脉冲的高
//电平部分一般为 0.5ms~2.5ms 范围内的角度控制脉冲部分。以 180
//度角度伺服为例，那么对应的控制关系是这样的：
//0.5ms--------------0 度；
//1.0ms------------45 度；
//1.5ms------------90 度；
//2.0ms-----------135 度；
//2.5ms-----------180 度；
}
void DUOJI_STOP(void){
	TIM_SetCompare1(TIM3, 150);
}
void DUOJI_SHUN(void){
	TIM_SetCompare1(TIM3, 50);
}
void DUOJI_NI(void){
	TIM_SetCompare1(TIM3, 250);
}
void KEY_OPEN_CLOSE_STOP(void){
	if(KEY_Scanf(GPIOG, KEY1_PIN)==1)
	{
		DUOJI_SHUN();
		Delay_ms(500);
		Delay_ms(500);
		Delay_ms(500);
		Delay_ms(500);

		DUOJI_STOP();
	}else if(KEY_Scanf(GPIOG, KEY2_PIN)==1){
		DUOJI_NI();
		Delay_ms(500);
		Delay_ms(500);
		Delay_ms(500);
		Delay_ms(500);

		DUOJI_STOP();		
	}else if(KEY_Scanf(GPIOG, KEY3_PIN)==1){
		DUOJI_STOP();
	}
	
}
 
uint8_t LIGHT_FLAG_DUOJI = 0;// 如果打开过了为1，关闭过了为2
void ADC_LIGHT_OPEN_CLOSE(void)
{
	if(LIGHT_FLAG_DUOJI != 1){// 不是打开过的
		if(light >= 232){
			DUOJI_SHUN();
			Delay_ms(500);
			Delay_ms(500);
			Delay_ms(500);
			Delay_ms(500);	
			DUOJI_STOP();
			LIGHT_FLAG_DUOJI = 1;
			}
	}
	if(LIGHT_FLAG_DUOJI != 2){
		if(light <= 7){
			DUOJI_NI();
			Delay_ms(500);
			Delay_ms(500);
			Delay_ms(500);
			Delay_ms(500);
			DUOJI_STOP();
			LIGHT_FLAG_DUOJI = 2;
		}
	}
	
}
//void TIM3_IRQHandler(void)
//{
//	if(TIM_GetITStatus(TIM3, TIM_IT_CC1)==1){
//		TIM_ClearFlag(TIM3, TIM_FLAG_CC1);
//		
//	}
//}
