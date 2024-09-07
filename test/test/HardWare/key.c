#include "KEY.h"

//void Delay_ms(uint32_t);
//void Delay_ms(uint32_t);
void KEY_config(void){
	// ���������ļ�
	GPIO_InitTypeDef GPIOG_InitStruct;
	// 1.����GPIO��Ӧ��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	// 2.����GPIO����Ϊ��������
	GPIOG_InitStruct.GPIO_Pin  = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;     // ��Ӧ���а�ť����
	GPIOG_InitStruct.GPIO_Mode = GPIO_Mode_IN;		//����ģʽ
	GPIOG_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;	//����
	
	GPIO_Init(GPIOG, &GPIOG_InitStruct);
}
uint8_t KEY_Scanf(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	 
	if( GPIO_ReadInputDataBit(GPIOx,GPIO_Pin)==Bit_RESET)
	{		
 
		Delay_ms(500);
		
 
		while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin)==Bit_RESET);
		return 1;
	}else{
		return 0;
	}
}
	
	void KEY_CONTROL_LED(void)
{
		if(KEY_Scanf(GPIOG, GPIO_Pin_2)==1)
		{
			GPIO_ToggleBits(LED_PIN_GROUP, LED1_PIN);
		}
		else if(KEY_Scanf(GPIOG, GPIO_Pin_3)==1)
		{
			GPIO_ToggleBits(LED_PIN_GROUP, LED2_PIN);
		}
		else if(KEY_Scanf(GPIOG, GPIO_Pin_4)==1)
		{
			GPIO_ToggleBits(LED_PIN_GROUP, LED3_PIN);
		}
		else if(KEY_Scanf(GPIOG, GPIO_Pin_5)==1)
		{
			GPIO_ToggleBits(LED_PIN_GROUP, LED4_PIN);
		}else{
			;
		}	
}
// ��ѹĳ��������ĳ��LED������
void KEY_LED_count(GPIO_TypeDef * GPIOx,uint16_t KEY_PIN,uint16_t LED_PIN){
		int sum = 0;
		int cal = 0;
		// LED����Ƹߵ�ƽ
		//GPIO_SetBits(GPIOG, LED1_PIN|LED2_PIN|LED4_PIN|LED3_PIN);
		while(1){
		// �������£���ȡ���͵�ƽ����LED��
		
		if(GPIO_ReadInputDataBit(GPIOx,KEY_PIN) ==Bit_RESET){
			sum++;
			cal = sum;
			while(cal--){
			
					GPIO_ResetBits(GPIOx, LED_PIN);
					Delay_ms(500);
				
					GPIO_SetBits(GPIOx, LED_PIN);
					Delay_ms(500);
				}		
 
		}
	}
}

void KEY_LED_passwd(GPIO_TypeDef *GPIOx){
	// һ���������룺�趨����Ϊ1001��ͨ����������������ȷ�����뷽�ɽ���ѭ��ִ����ˮ�ơ�������һΪ0��������Ϊ1��������Ϊ�˸񣬰�����Ϊ�������룩
	// �����������
	int a[4];
	int passwd[4] = {1,0,0,1};
	int x = 1;
	int i = 0;	
	while(1){
						// �������£���ȡ���͵�ƽ������һ��LED��		
		if(GPIO_ReadInputDataBit(GPIOx,KEY1_PIN) ==Bit_RESET){
			a[i] = 0;
			
			GPIO_ResetBits(GPIOx, LED1_PIN);
			Delay_ms(500);
			GPIO_SetBits(GPIOx, LED1_PIN);
			
			
			i++;
		
		}
	 		else if(GPIO_ReadInputDataBit(GPIOx,KEY2_PIN) ==Bit_RESET){
				a[i] = 1;
				if(a[i]==1){
					GPIO_ResetBits(GPIOx, LED2_PIN);
					Delay_ms(500);
					GPIO_SetBits(GPIOx, LED2_PIN);
					
				i++;
			}
		}
			else if(GPIO_ReadInputDataBit(GPIOx,KEY3_PIN) ==Bit_RESET){
			i--;
				if(i>=4){
					i=0;
				}
			GPIO_ResetBits(GPIOx, LED3_PIN);
			Delay_ms(500);
			GPIO_SetBits(GPIOx, LED3_PIN);
				
			
		}
			else if(GPIO_ReadInputDataBit(GPIOx,KEY4_PIN) ==Bit_RESET){
			i = 0;
			GPIO_ResetBits(GPIOx, LED4_PIN);

				Delay_ms(500);
			GPIO_SetBits(GPIOx, LED4_PIN);
				
		}
		
		if(i == 4){
			break;
		}
	}
	 for(int i = 0; i < 4; i++){
		if(a[i] != passwd[i]){
			x=0;
			break;
		}
	}		
	if(x){
		// �����ȷ������ˮ��
		LED_Stream(500);
	}else{
		// ����ȫ��
		GPIO_ResetBits(GPIOx, LED1_PIN|LED2_PIN|LED4_PIN|LED3_PIN);
	}

}
// 
void KEY_All_LED_alarm(){
	// ����LED����ʵ�飺���°�����LED��һ��Ƶ����˸���ɿ�������ֹͣ��˸
	while(1){
		if(GPIO_ReadInputDataBit(GPIOG,KEY1_PIN) ==Bit_RESET){
			GPIO_ResetBits(GPIOG, LED1_PIN|LED3_PIN|LED2_PIN|LED4_PIN);
			Delay_ms(200);
			GPIO_SetBits(GPIOG, LED1_PIN|LED2_PIN|LED3_PIN|LED4_PIN);
			Delay_ms(300);	
		}
			else if(GPIO_ReadInputDataBit(GPIOG,KEY2_PIN) ==Bit_RESET){
			GPIO_ResetBits(GPIOG, LED1_PIN|LED3_PIN|LED2_PIN|LED4_PIN);
			Delay_ms(200);
			GPIO_SetBits(GPIOG, LED1_PIN|LED2_PIN|LED3_PIN|LED4_PIN);
			Delay_ms(300);	
		}
			else if(GPIO_ReadInputDataBit(GPIOG,KEY3_PIN) ==Bit_RESET){
			GPIO_ResetBits(GPIOG, LED1_PIN|LED3_PIN|LED2_PIN|LED4_PIN);
			Delay_ms(200);
			GPIO_SetBits(GPIOG, LED1_PIN|LED2_PIN|LED3_PIN|LED4_PIN);
			Delay_ms(300);	
		}
			else if(GPIO_ReadInputDataBit(GPIOG,KEY4_PIN) ==Bit_RESET){
			GPIO_ResetBits(GPIOG, LED1_PIN|LED3_PIN|LED2_PIN|LED4_PIN);
			Delay_ms(200);
			GPIO_SetBits(GPIOG, LED1_PIN|LED2_PIN|LED3_PIN|LED4_PIN);
			Delay_ms(300);	
		}
	}

}
