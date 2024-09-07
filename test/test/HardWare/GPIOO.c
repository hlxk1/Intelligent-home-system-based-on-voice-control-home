#include "GPIOO.h"
//PB6/TIM4_CH1/U1_TX/CAN2_TX/I2C1_SCL/DCMI_D5 ���
//PE6/TRACED3/FSMC_A22/TIM9_CH2/DCMI_D7  �ƹ�
void DCMI_D5_pinB6(void)
{
	GPIO_InitTypeDef GPIOB_InitStruct;
	// 1.����GPIO��Ӧ��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  // 2.����GPIO����Ϊ�������
	GPIOB_InitStruct.GPIO_Pin  = GPIO_Pin_6;    // ��Ӧ14������
  GPIOB_InitStruct.GPIO_Mode = GPIO_Mode_IN;		//����ģʽ
//  GPIOG_InitStruct.GPIO_OType = GPIO_OType_PP;	//��������
  GPIOB_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;	//����
	GPIO_Init(GPIOB, &GPIOB_InitStruct);
}
void DCMI_D7_pine6(void){
	GPIO_InitTypeDef GPIOE_InitStruct;
	// 1.����GPIO��Ӧ��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
  // 2.����GPIO����Ϊ�������
	GPIOE_InitStruct.GPIO_Pin  = GPIO_Pin_6;    // ��Ӧ14������
  GPIOE_InitStruct.GPIO_Mode = GPIO_Mode_IN;		//����ģʽ
//  GPIOG_InitStruct.GPIO_OType = GPIO_OType_PP;	//��������
  GPIOE_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;	//����
	GPIO_Init(GPIOE, &GPIOE_InitStruct);
}
int DUOJI_open = 0;
int DUOJI_close = 0;
void DUOJI_pin(void){
	if(DUOJI_open != 1){
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6) == SET){
			DUOJI_SHUN();
			Delay_ms(500);
			Delay_ms(500);
			Delay_ms(500);
			Delay_ms(500);	
			DUOJI_STOP();
			DUOJI_open = 1;
			DUOJI_close = 0;
		}
	}
	if(DUOJI_close != 1){
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6) == RESET){
			DUOJI_NI();
			Delay_ms(500);
			Delay_ms(500);
			Delay_ms(500);
			Delay_ms(500);
			DUOJI_STOP();
			DUOJI_open = 0;
			DUOJI_close = 1;
		}
	}
	

}
void LED_pin(void){
	if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_6) == SET){
		GPIO_SetBits(GPIOG, LED1_PIN|LED2_PIN);
	}else if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_6) == RESET){
		GPIO_ResetBits(GPIOG, LED1_PIN|LED2_PIN);
	}
}
