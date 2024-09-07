#include "wwdg.h" 

void WWDG_Config(uint32_t WWDG_Prescaler, uint8_t WindowValue)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
	
	WWDG_SetPrescaler(WWDG_Prescaler);	
	
	WWDG_SetWindowValue(WindowValue);
	
	//设置WWDG的计数初值(最大值为127)并开启WWDG
	WWDG_Enable(127);
	
	//思考：用户进行喂狗操作的时间段如何计算？   所以用户应该在开启WWDG之后的 36.66ms~49.92ms之间进行喂狗操作 
	//从127递减到80  --> 计数47次  --->  计数1次花费780us  ---> 36.66ms   (该时间段用户不应该喂狗)
	//从127递减到63  --> 计数64次  --->  计数1次花费780us  ---> 49.92ms   (计数值递减到0x3F(63)花费的时间)
	
	//利用WWDG的提前唤醒中断的功能,则需要配置NVIC
	NVIC_InitStruct.NVIC_IRQChannel = WWDG_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	//清除WWDG的提前唤醒中断标志
	WWDG_ClearFlag();
	
	//使能WWDG的中断
	WWDG_EnableIT();
}

void WWDG_IRQHandler(void)
{
	if( SET == WWDG_GetFlagStatus())
	{
		WWDG_ClearFlag();
	
		//设定计数器初值为127
		WWDG_SetCounter(127);
	}
}
