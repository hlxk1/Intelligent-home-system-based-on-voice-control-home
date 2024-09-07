#include "beep.h"

void BEEP_GPIO_Config(void)
{	
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStruct;
	
	/*开启LED相关的GPIO外设时钟*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 

	/*选择要控制的GPIO引脚*/															   
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;	

	/*设置引脚模式为输出模式*/
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;   

	/*设置引脚的输出类型为推挽输出*/
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;

	/*设置引脚为上拉模式*/
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;

	/*设置引脚速率为100MHz */   
	GPIO_InitStruct.GPIO_Speed = GPIO_High_Speed; 

	/*调用库函数，使用上面配置的GPIO_InitStruct初始化GPIO*/
	GPIO_Init(GPIOB, &GPIO_InitStruct);	
	
	/*关闭BEEP*/
	GPIO_ResetBits(GPIOB, GPIO_Pin_5);
}
