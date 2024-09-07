#include "MQ_2.h"
#include <stdio.h>

void MQ2_IO_Config(void)
{
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStruct;
	
	/*开启LED相关的GPIO外设时钟*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 
	
	/*选择要控制的GPIO引脚*/													   
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;	

	/*设置引脚为输入模式*/
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN; 
  
	/*设置引脚不上拉也不下拉*/
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;

	/*调用库函数，使用上面配置的GPIO_InitStruct初始化GPIO*/
	GPIO_Init(GPIOA, &GPIO_InitStruct);	
}

void MQ2_ADC_Config(void)
{ 
	GPIO_InitTypeDef GPIO_InitStruct;
	ADC_InitTypeDef ADC_InitStruct;
	ADC_CommonInitTypeDef ADC_CommonInitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN; 
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	ADC_InitStruct.ADC_Resolution			= ADC_Resolution_12b;				//ADC 分辨率
	ADC_InitStruct.ADC_ScanConvMode			= ENABLE;							//禁止扫描模式，多通道采集才需要
	ADC_InitStruct.ADC_ContinuousConvMode	= ENABLE;							//连续转换
	ADC_InitStruct.ADC_ExternalTrigConvEdge	= ADC_ExternalTrigConvEdge_None;	//禁止外部边沿触发
	ADC_InitStruct.ADC_DataAlign			= ADC_DataAlign_Right;				//数据右对齐
	ADC_InitStruct.ADC_NbrOfConversion		= 1;								//转换通道 1个
	ADC_Init(ADC1, &ADC_InitStruct);

	ADC_CommonInitStruct.ADC_Mode			= ADC_Mode_Independent;				//独立ADC模式,针对单通道转换
	ADC_CommonInitStruct.ADC_DMAAccessMode	= ADC_DMAAccessMode_Disabled;		//禁止DMA直接访问模式
	ADC_CommonInitStruct.ADC_Prescaler		= ADC_Prescaler_Div2;				//时钟分频  90MHZ/2 = 45MHZ	//APB2:90MHz
	//ADC_CommonInitStruct.ADC_TwoSamplingDelay	= ADC_TwoSamplingDelay_5Cycles;	//采样时间间隔，只用于多个ADC使用时
	ADC_CommonInit(&ADC_CommonInitStruct);
	
	ADC_Cmd(ADC1, ENABLE);
}

//获取ADC单次转换的电压值，调用该函数1次就获取一次模拟量，就转换成1次数字量
void Get_Conversion_value(void)
{
	 uint16_t analog;
	 float 	data;
	
	//选择ADC外设的转换通道以及转换顺序，配置 ADC 通道转换顺序为1（第一个转换），ADC_SampleTime_3Cycles为ADC采样周期：3个周期
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 1, ADC_SampleTime_3Cycles);	 
	//总的转换时间 = 采样周期 + 12个周期 = 15个周期  --> 1个周期为1/45us  ---> 约等于0.333us
	
	//开启ADC1的转换
	ADC_SoftwareStartConv(ADC1);
	
	//获取ADC1转换器的转换结果：模拟量
	analog = ADC_GetConversionValue(ADC1);
	
	//转换成数字量
	data = analog*3.3/4096;
	
	printf("ADC单次转换的电压值为 %f v.\r\n", data);
}
