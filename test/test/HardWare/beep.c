#include "beep.h"

void BEEP_GPIO_Config(void)
{	
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStruct;
	
	/*����LED��ص�GPIO����ʱ��*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 

	/*ѡ��Ҫ���Ƶ�GPIO����*/															   
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;	

	/*��������ģʽΪ���ģʽ*/
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;   

	/*�������ŵ��������Ϊ�������*/
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;

	/*��������Ϊ����ģʽ*/
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;

	/*������������Ϊ100MHz */   
	GPIO_InitStruct.GPIO_Speed = GPIO_High_Speed; 

	/*���ÿ⺯����ʹ���������õ�GPIO_InitStruct��ʼ��GPIO*/
	GPIO_Init(GPIOB, &GPIO_InitStruct);	
	
	/*�ر�BEEP*/
	GPIO_ResetBits(GPIOB, GPIO_Pin_5);
}
