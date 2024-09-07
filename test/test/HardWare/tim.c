#include "tim.h"

void TIM6_Config(void)
{
	NVIC_InitTypeDef 		NVIC_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	
	TIM_TimeBaseInitStruct.TIM_Period			= 5000-1;
	TIM_TimeBaseInitStruct.TIM_Prescaler		= 9000-1;
	TIM_TimeBaseInitStruct.TIM_CounterMode		= TIM_CounterMode_Up;
	//TIM_TimeBaseInitStruct.TIM_ClockDivision
	//TIM_TimeBaseInitStruct.TIM_RepetitionCounter
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel	= TIM6_DAC_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd	= ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority	= 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority	= 1;
	NVIC_Init(&NVIC_InitStruct);
	
	//设置基本定时器中断触发方式
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
	
	//使能定时器
	TIM_Cmd(TIM6, ENABLE);
}


int time_num;

//定时器中断服务函数
void TIM6_DAC_IRQHandler(void)
{
	if( SET == TIM_GetITStatus(TIM6, TIM_IT_Update) )
	{	
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
	
		//执行响应的动作
		//GPIO_ToggleBits(GPIOH, GPIO_Pin_10);
		
		//增加延时
		time_num++;			//定时器配置为每0.5秒产生一次中断对数值累加
		if(time_num == 10)	//即每0.5*10秒改变一次灯的状态
		{
			time_num = 0;
			//执行响应的动作
			//GPIO_ToggleBits(GPIOH, GPIO_Pin_12);
		}
	
	}
}
