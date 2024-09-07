#include "wwdg.h" 

void WWDG_Config(uint32_t WWDG_Prescaler, uint8_t WindowValue)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
	
	WWDG_SetPrescaler(WWDG_Prescaler);	
	
	WWDG_SetWindowValue(WindowValue);
	
	//����WWDG�ļ�����ֵ(���ֵΪ127)������WWDG
	WWDG_Enable(127);
	
	//˼�����û�����ι��������ʱ�����μ��㣿   �����û�Ӧ���ڿ���WWDG֮��� 36.66ms~49.92ms֮�����ι������ 
	//��127�ݼ���80  --> ����47��  --->  ����1�λ���780us  ---> 36.66ms   (��ʱ����û���Ӧ��ι��)
	//��127�ݼ���63  --> ����64��  --->  ����1�λ���780us  ---> 49.92ms   (����ֵ�ݼ���0x3F(63)���ѵ�ʱ��)
	
	//����WWDG����ǰ�����жϵĹ���,����Ҫ����NVIC
	NVIC_InitStruct.NVIC_IRQChannel = WWDG_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	//���WWDG����ǰ�����жϱ�־
	WWDG_ClearFlag();
	
	//ʹ��WWDG���ж�
	WWDG_EnableIT();
}

void WWDG_IRQHandler(void)
{
	if( SET == WWDG_GetFlagStatus())
	{
		WWDG_ClearFlag();
	
		//�趨��������ֵΪ127
		WWDG_SetCounter(127);
	}
}
