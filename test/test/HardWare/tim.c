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
	
	//���û�����ʱ���жϴ�����ʽ
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
	
	//ʹ�ܶ�ʱ��
	TIM_Cmd(TIM6, ENABLE);
}


int time_num;

//��ʱ���жϷ�����
void TIM6_DAC_IRQHandler(void)
{
	if( SET == TIM_GetITStatus(TIM6, TIM_IT_Update) )
	{	
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
	
		//ִ����Ӧ�Ķ���
		//GPIO_ToggleBits(GPIOH, GPIO_Pin_10);
		
		//������ʱ
		time_num++;			//��ʱ������Ϊÿ0.5�����һ���ж϶���ֵ�ۼ�
		if(time_num == 10)	//��ÿ0.5*10��ı�һ�εƵ�״̬
		{
			time_num = 0;
			//ִ����Ӧ�Ķ���
			//GPIO_ToggleBits(GPIOH, GPIO_Pin_12);
		}
	
	}
}
