#include "MQ_2.h"
#include <stdio.h>

void MQ2_IO_Config(void)
{
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStruct;
	
	/*����LED��ص�GPIO����ʱ��*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 
	
	/*ѡ��Ҫ���Ƶ�GPIO����*/													   
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;	

	/*��������Ϊ����ģʽ*/
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN; 
  
	/*�������Ų�����Ҳ������*/
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;

	/*���ÿ⺯����ʹ���������õ�GPIO_InitStruct��ʼ��GPIO*/
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
	
	ADC_InitStruct.ADC_Resolution			= ADC_Resolution_12b;				//ADC �ֱ���
	ADC_InitStruct.ADC_ScanConvMode			= ENABLE;							//��ֹɨ��ģʽ����ͨ���ɼ�����Ҫ
	ADC_InitStruct.ADC_ContinuousConvMode	= ENABLE;							//����ת��
	ADC_InitStruct.ADC_ExternalTrigConvEdge	= ADC_ExternalTrigConvEdge_None;	//��ֹ�ⲿ���ش���
	ADC_InitStruct.ADC_DataAlign			= ADC_DataAlign_Right;				//�����Ҷ���
	ADC_InitStruct.ADC_NbrOfConversion		= 1;								//ת��ͨ�� 1��
	ADC_Init(ADC1, &ADC_InitStruct);

	ADC_CommonInitStruct.ADC_Mode			= ADC_Mode_Independent;				//����ADCģʽ,��Ե�ͨ��ת��
	ADC_CommonInitStruct.ADC_DMAAccessMode	= ADC_DMAAccessMode_Disabled;		//��ֹDMAֱ�ӷ���ģʽ
	ADC_CommonInitStruct.ADC_Prescaler		= ADC_Prescaler_Div2;				//ʱ�ӷ�Ƶ  90MHZ/2 = 45MHZ	//APB2:90MHz
	//ADC_CommonInitStruct.ADC_TwoSamplingDelay	= ADC_TwoSamplingDelay_5Cycles;	//����ʱ������ֻ���ڶ��ADCʹ��ʱ
	ADC_CommonInit(&ADC_CommonInitStruct);
	
	ADC_Cmd(ADC1, ENABLE);
}

//��ȡADC����ת���ĵ�ѹֵ�����øú���1�ξͻ�ȡһ��ģ��������ת����1��������
void Get_Conversion_value(void)
{
	 uint16_t analog;
	 float 	data;
	
	//ѡ��ADC�����ת��ͨ���Լ�ת��˳������ ADC ͨ��ת��˳��Ϊ1����һ��ת������ADC_SampleTime_3CyclesΪADC�������ڣ�3������
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 1, ADC_SampleTime_3Cycles);	 
	//�ܵ�ת��ʱ�� = �������� + 12������ = 15������  --> 1������Ϊ1/45us  ---> Լ����0.333us
	
	//����ADC1��ת��
	ADC_SoftwareStartConv(ADC1);
	
	//��ȡADC1ת������ת�������ģ����
	analog = ADC_GetConversionValue(ADC1);
	
	//ת����������
	data = analog*3.3/4096;
	
	printf("ADC����ת���ĵ�ѹֵΪ %f v.\r\n", data);
}
