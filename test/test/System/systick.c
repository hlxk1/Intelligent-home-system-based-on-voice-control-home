#include "systick.h"

static __IO uint32_t TimingDelay;
 
//启动系统滴答定时器 SysTick
void SysTick_Init(void)
{
	/* SystemFrequency / 1000    1ms中断一次
	 * SystemFrequency / 100000	 10us中断一次
	 * SystemFrequency / 1000000 1us中断一次
	 */
	if (SysTick_Config(SystemCoreClock / 1000000))	//设置每10us产生中断一次，系统执行一次 SysTick_Handler() 中断函数
	{
		while (1);
	}
}

//us延时程序，1us为一个单位
	//Delay_us( 10 ) 则实现的延时为 1 * 10us = 10us
void Delay_us(__IO uint32_t nTime)
{ 
	TimingDelay = nTime;	

	while(TimingDelay != 0);
}


//获取节拍程序
//在 SysTick 中断函数 SysTick_Handler()调用
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{ 
		TimingDelay--;
	}
}
