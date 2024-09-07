#include "pwm.h"

void TIM5_PWM_Config(void)
{
	GPIO_InitTypeDef 		GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef 		TIM_OCInitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	
	//2.ѡ��GPIO���ŵĸ��ù���
	GPIO_PinAFConfig(GPIOH, GPIO_PinSource10, GPIO_AF_TIM5);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;   
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_High_Speed; 
	GPIO_Init(GPIOH, &GPIO_InitStruct);
	
	TIM_TimeBaseInitStruct.TIM_Period			= 100-1;
	TIM_TimeBaseInitStruct.TIM_Prescaler		= 9000-1;
	TIM_TimeBaseInitStruct.TIM_CounterMode		= TIM_CounterMode_Up;
	//TIM_TimeBaseInitStruct.TIM_ClockDivision
	//TIM_TimeBaseInitStruct.TIM_RepetitionCounter
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseInitStruct);
	
	TIM_OCInitStruct.TIM_OCMode			= TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OutputState	= TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_OCPolarity		= TIM_OCPolarity_Low;	//������Ч��ƽΪ��
	TIM_OCInitStruct.TIM_Pulse			= 0;	//ָ��Ҫ���ص�����ȽϼĴ���������ֵ�����������ñȽϳ�ֵΪ0��ռ�ձ� 0%
	TIM_OC1Init(TIM5, &TIM_OCInitStruct);
	
	TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Enable);	//��CCR1������TIM5��ΧԤ���ؼĴ�����
	
	TIM_Cmd(TIM5,ENABLE);
}

