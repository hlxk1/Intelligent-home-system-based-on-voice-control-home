#include "exti.h"

void EXTI_Config(void)
{
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource13);
	
	EXTI_InitStruct.EXTI_Line		= EXTI_Line0;
	EXTI_InitStruct.EXTI_LineCmd	= ENABLE;
	EXTI_InitStruct.EXTI_Mode		= EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger	= EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStruct);
	
	EXTI_InitStruct.EXTI_Line		= EXTI_Line13;
	EXTI_Init(&EXTI_InitStruct);
	
	//选择优先级组2
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitStruct.NVIC_IRQChannel	= EXTI0_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd	= ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority	= 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority	= 1;
	NVIC_Init(&NVIC_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel	= EXTI15_10_IRQn;
	NVIC_Init(&NVIC_InitStruct);
}

void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0) == SET)
	{
		EXTI_ClearITPendingBit(EXTI_Line0);
	
		GPIO_ResetBits(GPIOH, GPIO_Pin_10);
	}
}

void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line13) == SET)
	{
		EXTI_ClearITPendingBit(EXTI_Line13);
	
		GPIO_SetBits(GPIOH, GPIO_Pin_10);
	}
}
