#include "rtc.h"
#include <stdio.h>
#include "systick.h"

#define LSI_or_LES 1		//时钟源的选择，尽量使用LSI，尽管存在误差，但是板载的LSE外部低速时钟源（低频晶振）很有可能存放时间长而损坏导致RTC无法正常工作。

#define SET_FLAG 0x1289		//每次需要更新RTC（更换时间），需要修改此值，即修改RTC自带的备份寄存器的值

/* 配置框架

1、从指定的RTC备份数据寄存器中读取数据，判断RTC是否配置
2、如果没有：
	2.1配置RTC:
		打开PWR(电源控制器)外设的时钟
		解除RTC寄存器的访问保护才可以操作数据
		选择时钟源，默认配置低速外部时钟的晶振电路
		等待低速外部时钟LSE的晶振电路稳定
		对低速外部时钟LSE的时钟频率进行分频
		使能RTC实时时钟
		等待RTC APB寄存器同步
	2.2设置时间(根据WIFI_AP宏配置)、日期、闹钟；
	
3、否则：
	3.1打开PWR(电源控制器)外设的时钟
	3.2解除RTC寄存器的访问保护才可以操作数据
	3.3等待RTC APB寄存器同步

4、开启RTC唤醒功能；

5、中断服务函数显示时间和日期（获取时间和获取日期的函数必须同时调用）

*/


/* RTC 配置 */
void RTC_Config(void)
{
	//检查RTC自带的备份寄存器的值是否改变,此值用户可自调整
	if (RTC_ReadBackupRegister(RTC_BKP_DR0) != SET_FLAG)
	{
		printf("\r\n 重新配置RTC....\r\n");
		
		/* RTC 时钟源选择 */
		RTC_CLK_Config();
		
		printf("\r\n RTC configured....\n");
		
		/* 配置时间和日期 */
		RTC_Time_Config();
		
		/* 配置闹钟时间&闹钟中断*/
		RTC_Alarm_Config();
	}
	else
	{
		/* 检查是否电源复位 */
		if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
		{
			printf("\r\n 发生电源复位....\r\n");
		}
		/* 检查是否外部复位 */
		else if(RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
		{
			printf("\r\n 发生外部复位....\r\n");
		}
		
		printf("\r\n 不需要重新配置RTC....\r\n");

		/* 使能 PWR 时钟 */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

		/* 使能RTC、RTC备份寄存器和备份SRAM的访问 */
		PWR_BackupAccessCmd(ENABLE);
		
		/* 等待 RTC APB 寄存器同步 */
		RTC_WaitForSynchro();
		
		/* 清除RTC闹钟标志 */
		RTC_ClearFlag(RTC_FLAG_ALRAF);

		/* 清除17号线的中断标志(内部连接RTC闹钟) */
		EXTI_ClearITPendingBit(EXTI_Line17);
	}
	
	/* 开启RTC唤醒功能 */
	RTC_Set_WakeUp();
	
}

/* RTC 时钟源选择 */
void RTC_CLK_Config(void)
{
	RTC_InitTypeDef RTC_InitStruct;
	
	/*使能 PWR 时钟*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	
	/*使能RTC、RTC备份寄存器和备份SRAM的访问*/
	PWR_BackupAccessCmd(ENABLE);

	/* 选择时钟源需使能时钟源，且配置后需等待时钟源稳定工作*/
#if LSI_or_LES 
	/* 使用LSI作为RTC时钟源会有误差，默认选择LSE作为RTC的时钟源 */
	/* 使能LSI */ 
	RCC_LSICmd(ENABLE);

	/* 等待LSI稳定 */  
//	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
//	{
//	}

	/* 建议利用延时来等待时钟源稳定 */
	Delay_ms(100);

	/* 选择LSI作为RTC的时钟源 */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
#else	
	/* 使能LSE */ 
	RCC_LSEConfig(RCC_LSE_ON);
	/* 等待LSE稳定 */   
//	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
//	{
//	}
	Delay_ms(100);
	
	/* 选择LSE作为RTC的时钟源 */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);    
#endif

	/* 驱动日历的时钟：ck_spare = LSE/[(255+1)*(127+1)] = 1HZ */
	
	/* 使能RTC时钟 */
	RCC_RTCCLKCmd(ENABLE);
	
	/* 等待 RTC APB 寄存器同步 */
	RTC_WaitForSynchro();
	
	RTC_InitStruct.RTC_AsynchPrediv = 0x7F;	/* 设置异步预分频器的值 */	//要使用频率为 32.768 kHz 的 LSE 获得频率为 1 Hz 的内部时钟 (ck_spre)，需要将异步预分频系数设置为 128，并将同步预分频系数设置为 256。
	RTC_InitStruct.RTC_SynchPrediv	= 0xFF;	/* 设置同步预分频器的值 */
	RTC_InitStruct.RTC_HourFormat	= RTC_HourFormat_24;	//24小时制
	RTC_Init(&RTC_InitStruct);
}

/* 配置时间和日期 */
void RTC_Time_Config(void)
{
	RTC_TimeTypeDef RTC_TimeStruct;
	RTC_DateTypeDef RTC_DateStruct;
	
	RTC_TimeStruct.RTC_H12		= RTC_H12_AM;
	RTC_TimeStruct.RTC_Hours	= 10;
	RTC_TimeStruct.RTC_Minutes	= 15;
	RTC_TimeStruct.RTC_Seconds	= 20;
	RTC_SetTime(RTC_Format_BIN, &RTC_TimeStruct);	//RTC_Format_BIN：使用十进制		RTC_Format_BCD：使用16进制

	RTC_DateStruct.RTC_Year		= 22;
	RTC_DateStruct.RTC_Month	= 6;
	RTC_DateStruct.RTC_WeekDay	= 3;
	RTC_DateStruct.RTC_Date		= 10;
	RTC_SetDate(RTC_Format_BIN, &RTC_DateStruct);

	RTC_WriteBackupRegister(RTC_BKP_DR0, SET_FLAG);
}

/* 配置闹钟时间&闹钟中断*/
void RTC_Alarm_Config(void)
{
	RTC_AlarmTypeDef RTC_AlarmStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	RTC_AlarmStruct.RTC_AlarmTime.RTC_H12     = RTC_H12_AM;
	RTC_AlarmStruct.RTC_AlarmTime.RTC_Hours   = 12;
	RTC_AlarmStruct.RTC_AlarmTime.RTC_Minutes = 0;
	RTC_AlarmStruct.RTC_AlarmTime.RTC_Seconds = 0;
	RTC_AlarmStruct.RTC_AlarmDateWeekDaySel	= RTC_AlarmDateWeekDaySel_WeekDay;	//星期
	RTC_AlarmStruct.RTC_AlarmDateWeekDay	= RTC_Weekday_Monday;				//一
	RTC_AlarmStruct.RTC_AlarmMask			= RTC_AlarmMask_None;
	RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStruct);
	
	EXTI_ClearITPendingBit(EXTI_Line17);
	EXTI_InitStruct.EXTI_Line = EXTI_Line17;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;	//闹钟为上升沿触发
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = RTC_Alarm_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;	//注意抢占优先级，不要抢占窗口看门狗wwdg的中断，干预喂狗
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	RTC_ITConfig(RTC_IT_ALRA,ENABLE);
	
	RTC_AlarmCmd(RTC_Alarm_A, ENABLE);

	RTC_ClearFlag(RTC_FLAG_ALRAF);
}

/* 开启RTC唤醒功能 */
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

/* 显示年月日 */
void RTC_DataShow(void)
{
	uint8_t ShowData[50] = {0};
	RTC_DateTypeDef RTC_DateStruct;
	
	RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);
	
	sprintf((char*)ShowData,"%02d-%02d-%02d",RTC_DateStruct.RTC_Year,RTC_DateStruct.RTC_Month,RTC_DateStruct.RTC_Date); 
	
	printf("ShowData: %s \r\n", ShowData);
}


/* 显示时分秒 */
void RTC_TimeShow(void)
{
	char ShowTime[12] = {0};
	RTC_TimeTypeDef RTC_TimeStructure;
	
	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
	
	sprintf(ShowTime, "%d:%d:%d  ", RTC_TimeStructure.RTC_Hours, RTC_TimeStructure.RTC_Minutes, RTC_TimeStructure.RTC_Seconds);
	
	printf("ShowTime: %s \r\n", ShowTime);
}


//闹钟中断服务函数
void RTC_Alarm_IRQHandler(void)
{
	if(RTC_GetITStatus(RTC_IT_ALRA) != RESET)
	{
		GPIO_ToggleBits(GPIOH, GPIO_Pin_11);
		
		RTC_ClearITPendingBit(RTC_IT_ALRA);
		EXTI_ClearITPendingBit(EXTI_Line17);
	}
}

//唤醒中断服务函数
void RTC_WKUP_IRQHandler(void)
{
	if(RTC_GetITStatus(RTC_IT_WUT) != RESET)
	{
		GPIO_ToggleBits(GPIOH, GPIO_Pin_11);
		
		//显示日期
		RTC_DataShow();
		
		//显示时间
		RTC_TimeShow();
		
		RTC_ClearITPendingBit(RTC_IT_WUT);
		EXTI_ClearITPendingBit(EXTI_Line22);
	} 
}
