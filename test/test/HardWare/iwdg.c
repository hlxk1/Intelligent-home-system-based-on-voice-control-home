#include "iwdg.h" 

/*
	独立看门狗(IWDG)溢出时间Tout=（IWDG_RLR寄存器对应的装载数值Reload）*（看门狗时钟的周期）
	
	独立看门狗使用LSI作为时钟。
		LSI 的频率一般在 30~60KHZ 之间，根据温度和工作场合会有一定的漂移，我
		们一般取 40KHZ，所以独立看门狗的定时时间并一定非常精确，只适用于对时间精度
		要求比较低的场合。
		且独立看门狗还会对该输入分频进一步的分频处理。
															周期 = 1 / 频率
	所以，独立看门狗(IWDG)溢出时间计算：Tout= Reload * （ 1 /（ 40 / 分频系数Prescaler） (单位：ms)

	1、IWDG_Prescaler: specifies the IWDG Prescaler value.指定 IWDG 预分频器值为以下值之一
	  *          This parameter can be one of the following values:
	  *            @arg IWDG_Prescaler_4: IWDG prescaler set to 4
	  *            @arg IWDG_Prescaler_8: IWDG prescaler set to 8
	  *            @arg IWDG_Prescaler_16: IWDG prescaler set to 16
	  *            @arg IWDG_Prescaler_32: IWDG prescaler set to 32
	  *            @arg IWDG_Prescaler_64: IWDG prescaler set to 64
	  *            @arg IWDG_Prescaler_128: IWDG prescaler set to 128
	  *            @arg IWDG_Prescaler_256: IWDG prescaler set to 256
	  
	2、Reload:重载寄存器值，取值范围为：0~0xFFF
	
	假设：需要1s的独立看门狗超时，假设设定分频系数IWDG_Prescaler为64
		通过公式： 1s = Reload *（1/40/64 ），求出重装载值Reload为 625 
*/


void IWDG_Config(uint8_t IWDG_Prescaler, uint16_t Reload)
{
	//使能预分频寄存器PR和重装载寄存器RLR可写
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	
	//设置预分频器值
	IWDG_SetPrescaler(IWDG_Prescaler);
	
	//设置重装载寄存器值
	IWDG_SetReload(Reload);
	
	//使能IWDG
	IWDG_Enable();
	
	//把重装载寄存器的值放到计数器中
	IWDG_ReloadCounter();
}
