#include "ADC.h"

/*
1���鿴ԭ��ͼ
PF7 ADC3_IN5
2��������

*/

/**
 * @brief  ADCģ��ת������ʼ������
 * @param  ��
 * @retval ��
 */
void ADC3_Config(void)
{
	/* ����������͵Ľṹ�� */
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef ADC_InitStructure;

	/* ������ʱ��ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);		// ʹ��ADC3ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE); // ʹ��GPIOFʱ��

	/* ����ADCͨ����ص�GPIO�˿� */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;		 // PF7 ADC3 ͨ��5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;	 // ģ������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // ����������
	GPIO_Init(GPIOF, &GPIO_InitStructure); // ��ʼ��

	/* ADC����ģʽ���� */
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;						// ADC�ֱ���Ϊ12λ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;								// ��ɨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;							// �ر�����ת�� ����ת��
	//ADC_InitStructure.ADC_ExternalTrigConv
	//ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; // ��ֹ�������ؼ��
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;						// �Ҷ���
	ADC_InitStructure.ADC_NbrOfConversion = 1;									// ת��ͨ����Ϊ1
	ADC_Init(ADC3, &ADC_InitStructure);										// ���ÿ⺯��ʹ������������Ϣ��ʼ��ADC

	/* ADCͨ�ù���ģʽ���� */
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;					 // ����ģʽ
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles; // ���������׶�֮����ӳ�5��ʱ��
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;		 // DMAʧ��
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;					 // Ԥ��ƵΪ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz��ADCxʱ����ò�Ҫ����36Mhz
	ADC_CommonInit(&ADC_CommonInitStructure);									 // ���ÿ⺯��ʹ������������Ϣ��ʼ��ADC

//	// ʹ��ADC�ж�
//	ADC_ITConfig(ADC3, ADC_IT_EOC, ENABLE);

//	NVIC_InitTypeDef NVIC_InitStructure;
//	
//	/* ����NVIC�ж� */
//	NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn; // ����ADC�ж�ͨ��
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; // ������ռ���ȼ�
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; // ���������ȼ�
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // ʹ��NVIC�ж�ͨ��
//	NVIC_Init(&NVIC_InitStructure); // ��ʼ��NVIC
	/* ����ADC */
	ADC_Cmd(ADC3, ENABLE);
}

/**
 * @brief  ��ȡ����ADC��ֵ����
 * @param  ADCx��ָ�������ADC���磺ADC3��
			ADC_Channel��Ҫ���õ�ADCͨ�����磺ADC_Channel_5
 * @retval ��ȡ�ĵ���ADC��ֵ
 */
uint16_t Get_Adc(ADC_TypeDef *ADCx, uint8_t ADC_Channel)
{
	/* ADC1����ͨ������ */
	ADC_RegularChannelConfig(ADCx, ADC_Channel, 1, ADC_SampleTime_480Cycles);

	ADC_DiscModeChannelCountConfig(ADC3, 1);
	
	ADC_DiscModeCmd(ADC3,ENABLE);
	
	/* ����ADC���ת�� */
	ADC_SoftwareStartConv(ADCx);

	/* �ȴ�ADCת������ */
	while (!ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC))
		;

	/* ��ȡADCת��ֵ */
	return ADC_GetConversionValue(ADCx);
}

/**
 * @brief  ��λ�ȡͨ��Channel��ת��ֵƽ��ֵ����
 * @param  ADCx��ָ�������ADC���磺ADC3��
			ADC_Channel��Ҫ���õ�ADCͨ�����磺ADC_Channel_5
			times����ȡchanl��ת��ֵ�Ĵ���
 * @retval ƽ��ת��ֵ
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
 * @brief  ��ȡת����ľ����ѹֵ����
 * @param  ADCx��ָ�������ADC���磺ADC3��
			ADC_Channel��Ҫ���õ�ADCͨ�����磺ADC_Channel_5
			times����ȡchanl��ת��ֵ�Ĵ���
 * @retval ƽ��ת��ֵ
 */
float Get_Conversion_value(ADC_TypeDef *ADCx, uint8_t ADC_Channel, uint8_t times)
{
	uint16_t adc_value;
	float adc_vol;

//	adc_value = Get_Adc_Average(ADCx, ADC_Channel, times);
	adc_value = Get_Adc(ADCx,ADC_Channel);
	adc_vol = (float)adc_value * (3.3 / 4096); // 2*12���ֱ��ʣ� = 4096��ת��ֵ*ÿ�ݵ�ѹֵ�õ�ת��ֵ��Ӧ�ĵ�ѹֵ

	return adc_vol;
}
float r;
float v; 
uint8_t light;
uint8_t GET_light(void)
{


//		delay_ms(500);
		v = Get_Conversion_value(ADC3,ADC_Channel_5,1);
//		printf("��ѹΪ%f\r\n",v);
		r = (10000*v)/(5-v);
//		printf("����Ϊ%f\r\n",r);
		light	= GetLux(r);
		printf("��ǿΪ%d\r\n",light);
		ADC_LIGHT_OPEN_CLOSE();

		return light;
}

//void ADC_IRQHandler(void)
//{
//	
//}
