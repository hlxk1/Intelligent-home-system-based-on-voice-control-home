#include "rtc.h"
#include <stdio.h>
#include "systick.h"

#define LSI_or_LES 1		//ʱ��Դ��ѡ�񣬾���ʹ��LSI�����ܴ��������ǰ��ص�LSE�ⲿ����ʱ��Դ����Ƶ���񣩺��п��ܴ��ʱ�䳤���𻵵���RTC�޷�����������

#define SET_FLAG 0x1289		//ÿ����Ҫ����RTC������ʱ�䣩����Ҫ�޸Ĵ�ֵ�����޸�RTC�Դ��ı��ݼĴ�����ֵ

/* ���ÿ��

1����ָ����RTC�������ݼĴ����ж�ȡ���ݣ��ж�RTC�Ƿ�����
2�����û�У�
	2.1����RTC:
		��PWR(��Դ������)�����ʱ��
		���RTC�Ĵ����ķ��ʱ����ſ��Բ�������
		ѡ��ʱ��Դ��Ĭ�����õ����ⲿʱ�ӵľ����·
		�ȴ������ⲿʱ��LSE�ľ����·�ȶ�
		�Ե����ⲿʱ��LSE��ʱ��Ƶ�ʽ��з�Ƶ
		ʹ��RTCʵʱʱ��
		�ȴ�RTC APB�Ĵ���ͬ��
	2.2����ʱ��(����WIFI_AP������)�����ڡ����ӣ�
	
3������
	3.1��PWR(��Դ������)�����ʱ��
	3.2���RTC�Ĵ����ķ��ʱ����ſ��Բ�������
	3.3�ȴ�RTC APB�Ĵ���ͬ��

4������RTC���ѹ��ܣ�

5���жϷ�������ʾʱ������ڣ���ȡʱ��ͻ�ȡ���ڵĺ�������ͬʱ���ã�

*/


/* RTC ���� */
void RTC_Config(void)
{
	//���RTC�Դ��ı��ݼĴ�����ֵ�Ƿ�ı�,��ֵ�û����Ե���
	if (RTC_ReadBackupRegister(RTC_BKP_DR0) != SET_FLAG)
	{
		printf("\r\n ��������RTC....\r\n");
		
		/* RTC ʱ��Դѡ�� */
		RTC_CLK_Config();
		
		printf("\r\n RTC configured....\n");
		
		/* ����ʱ������� */
		RTC_Time_Config();
		
		/* ��������ʱ��&�����ж�*/
		RTC_Alarm_Config();
	}
	else
	{
		/* ����Ƿ��Դ��λ */
		if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
		{
			printf("\r\n ������Դ��λ....\r\n");
		}
		/* ����Ƿ��ⲿ��λ */
		else if(RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
		{
			printf("\r\n �����ⲿ��λ....\r\n");
		}
		
		printf("\r\n ����Ҫ��������RTC....\r\n");

		/* ʹ�� PWR ʱ�� */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

		/* ʹ��RTC��RTC���ݼĴ����ͱ���SRAM�ķ��� */
		PWR_BackupAccessCmd(ENABLE);
		
		/* �ȴ� RTC APB �Ĵ���ͬ�� */
		RTC_WaitForSynchro();
		
		/* ���RTC���ӱ�־ */
		RTC_ClearFlag(RTC_FLAG_ALRAF);

		/* ���17���ߵ��жϱ�־(�ڲ�����RTC����) */
		EXTI_ClearITPendingBit(EXTI_Line17);
	}
	
	/* ����RTC���ѹ��� */
	RTC_Set_WakeUp();
	
}

/* RTC ʱ��Դѡ�� */
void RTC_CLK_Config(void)
{
	RTC_InitTypeDef RTC_InitStruct;
	
	/*ʹ�� PWR ʱ��*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	
	/*ʹ��RTC��RTC���ݼĴ����ͱ���SRAM�ķ���*/
	PWR_BackupAccessCmd(ENABLE);

	/* ѡ��ʱ��Դ��ʹ��ʱ��Դ�������ú���ȴ�ʱ��Դ�ȶ�����*/
#if LSI_or_LES 
	/* ʹ��LSI��ΪRTCʱ��Դ������Ĭ��ѡ��LSE��ΪRTC��ʱ��Դ */
	/* ʹ��LSI */ 
	RCC_LSICmd(ENABLE);

	/* �ȴ�LSI�ȶ� */  
//	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
//	{
//	}

	/* ����������ʱ���ȴ�ʱ��Դ�ȶ� */
	Delay_ms(100);

	/* ѡ��LSI��ΪRTC��ʱ��Դ */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
#else	
	/* ʹ��LSE */ 
	RCC_LSEConfig(RCC_LSE_ON);
	/* �ȴ�LSE�ȶ� */   
//	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
//	{
//	}
	Delay_ms(100);
	
	/* ѡ��LSE��ΪRTC��ʱ��Դ */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);    
#endif

	/* ����������ʱ�ӣ�ck_spare = LSE/[(255+1)*(127+1)] = 1HZ */
	
	/* ʹ��RTCʱ�� */
	RCC_RTCCLKCmd(ENABLE);
	
	/* �ȴ� RTC APB �Ĵ���ͬ�� */
	RTC_WaitForSynchro();
	
	RTC_InitStruct.RTC_AsynchPrediv = 0x7F;	/* �����첽Ԥ��Ƶ����ֵ */	//Ҫʹ��Ƶ��Ϊ 32.768 kHz �� LSE ���Ƶ��Ϊ 1 Hz ���ڲ�ʱ�� (ck_spre)����Ҫ���첽Ԥ��Ƶϵ������Ϊ 128������ͬ��Ԥ��Ƶϵ������Ϊ 256��
	RTC_InitStruct.RTC_SynchPrediv	= 0xFF;	/* ����ͬ��Ԥ��Ƶ����ֵ */
	RTC_InitStruct.RTC_HourFormat	= RTC_HourFormat_24;	//24Сʱ��
	RTC_Init(&RTC_InitStruct);
}

/* ����ʱ������� */
void RTC_Time_Config(void)
{
	RTC_TimeTypeDef RTC_TimeStruct;
	RTC_DateTypeDef RTC_DateStruct;
	
	RTC_TimeStruct.RTC_H12		= RTC_H12_AM;
	RTC_TimeStruct.RTC_Hours	= 10;
	RTC_TimeStruct.RTC_Minutes	= 15;
	RTC_TimeStruct.RTC_Seconds	= 20;
	RTC_SetTime(RTC_Format_BIN, &RTC_TimeStruct);	//RTC_Format_BIN��ʹ��ʮ����		RTC_Format_BCD��ʹ��16����

	RTC_DateStruct.RTC_Year		= 22;
	RTC_DateStruct.RTC_Month	= 6;
	RTC_DateStruct.RTC_WeekDay	= 3;
	RTC_DateStruct.RTC_Date		= 10;
	RTC_SetDate(RTC_Format_BIN, &RTC_DateStruct);

	RTC_WriteBackupRegister(RTC_BKP_DR0, SET_FLAG);
}

/* ��������ʱ��&�����ж�*/
void RTC_Alarm_Config(void)
{
	RTC_AlarmTypeDef RTC_AlarmStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	RTC_AlarmStruct.RTC_AlarmTime.RTC_H12     = RTC_H12_AM;
	RTC_AlarmStruct.RTC_AlarmTime.RTC_Hours   = 12;
	RTC_AlarmStruct.RTC_AlarmTime.RTC_Minutes = 0;
	RTC_AlarmStruct.RTC_AlarmTime.RTC_Seconds = 0;
	RTC_AlarmStruct.RTC_AlarmDateWeekDaySel	= RTC_AlarmDateWeekDaySel_WeekDay;	//����
	RTC_AlarmStruct.RTC_AlarmDateWeekDay	= RTC_Weekday_Monday;				//һ
	RTC_AlarmStruct.RTC_AlarmMask			= RTC_AlarmMask_None;
	RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStruct);
	
	EXTI_ClearITPendingBit(EXTI_Line17);
	EXTI_InitStruct.EXTI_Line = EXTI_Line17;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;	//����Ϊ�����ش���
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = RTC_Alarm_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;	//ע����ռ���ȼ�����Ҫ��ռ���ڿ��Ź�wwdg���жϣ���Ԥι��
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	RTC_ITConfig(RTC_IT_ALRA,ENABLE);
	
	RTC_AlarmCmd(RTC_Alarm_A, ENABLE);

	RTC_ClearFlag(RTC_FLAG_ALRAF);
}

/* ����RTC���ѹ��� */
void RTC_Set_WakeUp(void)
{
	NVIC_InitTypeDef  NVIC_InitStruct;
	EXTI_InitTypeDef  EXTI_InitStruct;
	
	/* DISABLE Wakeup Counter */
	RTC_WakeUpCmd(DISABLE);
	
	EXTI_ClearITPendingBit(EXTI_Line22);
	EXTI_InitStruct.EXTI_Line = EXTI_Line22;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);

	/* Enable the RTC Wakeup Interrupt */
	NVIC_InitStruct.NVIC_IRQChannel = RTC_WKUP_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);  

	/* Configure the RTC WakeUp Clock source: CK_SPRE (1Hz) */
	RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);
	RTC_SetWakeUpCounter(0x0);

	/* Enable the RTC Wakeup Interrupt */
	RTC_ITConfig(RTC_IT_WUT, ENABLE);

	/* Enable Wakeup Counter */
	RTC_WakeUpCmd(ENABLE);
	
	/* Clears the RTC's interrupt pending bits. */
	RTC_ClearITPendingBit(RTC_IT_WUT);
}

/* ��ʾ������ */
void RTC_DataShow(void)
{
	uint8_t ShowData[50] = {0};
	RTC_DateTypeDef RTC_DateStruct;
	
	RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);
	
	sprintf((char*)ShowData,"%02d-%02d-%02d",RTC_DateStruct.RTC_Year,RTC_DateStruct.RTC_Month,RTC_DateStruct.RTC_Date); 
	
	printf("ShowData: %s \r\n", ShowData);
}


/* ��ʾʱ���� */
void RTC_TimeShow(void)
{
	char ShowTime[12] = {0};
	RTC_TimeTypeDef RTC_TimeStructure;
	
	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
	
	sprintf(ShowTime, "%d:%d:%d  ", RTC_TimeStructure.RTC_Hours, RTC_TimeStructure.RTC_Minutes, RTC_TimeStructure.RTC_Seconds);
	
	printf("ShowTime: %s \r\n", ShowTime);
}


//�����жϷ�����
void RTC_Alarm_IRQHandler(void)
{
	if(RTC_GetITStatus(RTC_IT_ALRA) != RESET)
	{
		GPIO_ToggleBits(GPIOH, GPIO_Pin_11);
		
		RTC_ClearITPendingBit(RTC_IT_ALRA);
		EXTI_ClearITPendingBit(EXTI_Line17);
	}
}

//�����жϷ�����
void RTC_WKUP_IRQHandler(void)
{
	if(RTC_GetITStatus(RTC_IT_WUT) != RESET)
	{
		GPIO_ToggleBits(GPIOH, GPIO_Pin_11);
		
		//��ʾ����
		RTC_DataShow();
		
		//��ʾʱ��
		RTC_TimeShow();
		
		RTC_ClearITPendingBit(RTC_IT_WUT);
		EXTI_ClearITPendingBit(EXTI_Line22);
	} 
}
