#include "systick.h"

static __IO uint32_t TimingDelay;
 
//����ϵͳ�δ�ʱ�� SysTick
void SysTick_Init(void)
{
	/* SystemFrequency / 1000    1ms�ж�һ��
	 * SystemFrequency / 100000	 10us�ж�һ��
	 * SystemFrequency / 1000000 1us�ж�һ��
	 */
	if (SysTick_Config(SystemCoreClock / 1000000))	//����ÿ10us�����ж�һ�Σ�ϵͳִ��һ�� SysTick_Handler() �жϺ���
	{
		while (1);
	}
}

//us��ʱ����1usΪһ����λ
	//Delay_us( 10 ) ��ʵ�ֵ���ʱΪ 1 * 10us = 10us
void Delay_us(__IO uint32_t nTime)
{ 
	TimingDelay = nTime;	

	while(TimingDelay != 0);
}


//��ȡ���ĳ���
//�� SysTick �жϺ��� SysTick_Handler()����
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{ 
		TimingDelay--;
	}
}
