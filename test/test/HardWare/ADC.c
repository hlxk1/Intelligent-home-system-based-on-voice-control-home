#include "ADC.h"

/*
1、查看原理图
PF7 ADC3_IN5
2、代码编程

*/

/**
 * @brief  ADC模数转换器初始化代码
 * @param  无
 * @retval 无
 */
void ADC3_Config(void)
{
	/* 定义相关类型的结构体 */
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef ADC_InitStructure;

	/* 开启定时器时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);		// 使能ADC3时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE); // 使能GPIOF时钟

	/* 配置ADC通道相关的GPIO端口 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;		 // PF7 ADC3 通道5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;	 // 模拟输入
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // 不带上下拉
	GPIO_Init(GPIOF, &GPIO_InitStructure); // 初始化

	/* ADC工作模式配置 */
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;						// ADC分辨率为12位
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;								// 非扫描模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;							// 关闭连续转换 单次转换
	//ADC_InitStructure.ADC_ExternalTrigConv
	//ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; // 禁止触发边沿检测
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;						// 右对齐
	ADC_InitStructure.ADC_NbrOfConversion = 1;									// 转换通道数为1
	ADC_Init(ADC3, &ADC_InitStructure);										// 调用库函数使用上述配置信息初始化ADC

	/* ADC通用工作模式配置 */
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;					 // 独立模式
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles; // 两个采样阶段之间的延迟5个时钟
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;		 // DMA失能
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;					 // 预分频为4分频，ADCCLK=PCLK2/4=84/4=21Mhz，ADCx时钟最好不要超过36Mhz
	ADC_CommonInit(&ADC_CommonInitStructure);									 // 调用库函数使用上述配置信息初始化ADC

//	// 使能ADC中断
//	ADC_ITConfig(ADC3, ADC_IT_EOC, ENABLE);

//	NVIC_InitTypeDef NVIC_InitStructure;
//	
//	/* 配置NVIC中断 */
//	NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn; // 设置ADC中断通道
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; // 设置抢占优先级
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; // 设置子优先级
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // 使能NVIC中断通道
//	NVIC_Init(&NVIC_InitStructure); // 初始化NVIC
	/* 开启ADC */
	ADC_Cmd(ADC3, ENABLE);
}

/**
 * @brief  获取单次ADC数值函数
 * @param  ADCx：指定具体的ADC，如：ADC3。
			ADC_Channel：要配置的ADC通道，如：ADC_Channel_5
 * @retval 获取的单次ADC数值
 */
uint16_t Get_Adc(ADC_TypeDef *ADCx, uint8_t ADC_Channel)
{
	/* ADC1常规通道配置 */
	ADC_RegularChannelConfig(ADCx, ADC_Channel, 1, ADC_SampleTime_480Cycles);

	ADC_DiscModeChannelCountConfig(ADC3, 1);
	
	ADC_DiscModeCmd(ADC3,ENABLE);
	
	/* 启动ADC软件转换 */
	ADC_SoftwareStartConv(ADCx);

	/* 等待ADC转换结束 */
	while (!ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC))
		;

	/* 获取ADC转换值 */
	return ADC_GetConversionValue(ADCx);
}

/**
 * @brief  多次获取通道Channel的转换值平均值函数
 * @param  ADCx：指定具体的ADC，如：ADC3。
			ADC_Channel：要配置的ADC通道，如：ADC_Channel_5
			times：获取chanl的转换值的次数
 * @retval 平均转换值
 */
uint16_t Get_Adc_Average(ADC_TypeDef *ADCx, uint8_t ADC_Channel, uint8_t times)
{
	uint8_t i;
	uint32_t temp_val = 0;

	for (i = 0; i < times; i++)
	{
		temp_val += Get_Adc(ADCx, ADC_Channel);
		Delay_us(50);
	}

	return temp_val / times;
}

/**
 * @brief  获取转换后的具体电压值函数
 * @param  ADCx：指定具体的ADC，如：ADC3。
			ADC_Channel：要配置的ADC通道，如：ADC_Channel_5
			times：获取chanl的转换值的次数
 * @retval 平均转换值
 */
float Get_Conversion_value(ADC_TypeDef *ADCx, uint8_t ADC_Channel, uint8_t times)
{
	uint16_t adc_value;
	float adc_vol;

//	adc_value = Get_Adc_Average(ADCx, ADC_Channel, times);
	adc_value = Get_Adc(ADCx,ADC_Channel);
	adc_vol = (float)adc_value * (3.3 / 4096); // 2*12（分辨率） = 4096，转换值*每份电压值得到转换值对应的电压值

	return adc_vol;
}
float r;
float v; 
uint8_t light;
uint8_t GET_light(void)
{


//		delay_ms(500);
		v = Get_Conversion_value(ADC3,ADC_Channel_5,1);
//		printf("电压为%f\r\n",v);
		r = (10000*v)/(5-v);
//		printf("电阻为%f\r\n",r);
		light	= GetLux(r);
		printf("光强为%d\r\n",light);
		ADC_LIGHT_OPEN_CLOSE();

		return light;
}

//void ADC_IRQHandler(void)
//{
//	
//}
