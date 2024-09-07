#include "iwdg.h" 

/*
	�������Ź�(IWDG)���ʱ��Tout=��IWDG_RLR�Ĵ�����Ӧ��װ����ֵReload��*�����Ź�ʱ�ӵ����ڣ�
	
	�������Ź�ʹ��LSI��Ϊʱ�ӡ�
		LSI ��Ƶ��һ���� 30~60KHZ ֮�䣬�����¶Ⱥ͹������ϻ���һ����Ư�ƣ���
		��һ��ȡ 40KHZ�����Զ������Ź��Ķ�ʱʱ�䲢һ���ǳ���ȷ��ֻ�����ڶ�ʱ�侫��
		Ҫ��Ƚϵ͵ĳ��ϡ�
		�Ҷ������Ź�����Ը������Ƶ��һ���ķ�Ƶ����
															���� = 1 / Ƶ��
	���ԣ��������Ź�(IWDG)���ʱ����㣺Tout= Reload * �� 1 /�� 40 / ��Ƶϵ��Prescaler�� (��λ��ms)

	1��IWDG_Prescaler: specifies the IWDG Prescaler value.ָ�� IWDG Ԥ��Ƶ��ֵΪ����ֵ֮һ
	  *          This parameter can be one of the following values:
	  *            @arg IWDG_Prescaler_4: IWDG prescaler set to 4
	  *            @arg IWDG_Prescaler_8: IWDG prescaler set to 8
	  *            @arg IWDG_Prescaler_16: IWDG prescaler set to 16
	  *            @arg IWDG_Prescaler_32: IWDG prescaler set to 32
	  *            @arg IWDG_Prescaler_64: IWDG prescaler set to 64
	  *            @arg IWDG_Prescaler_128: IWDG prescaler set to 128
	  *            @arg IWDG_Prescaler_256: IWDG prescaler set to 256
	  
	2��Reload:���ؼĴ���ֵ��ȡֵ��ΧΪ��0~0xFFF
	
	���裺��Ҫ1s�Ķ������Ź���ʱ�������趨��Ƶϵ��IWDG_PrescalerΪ64
		ͨ����ʽ�� 1s = Reload *��1/40/64 ���������װ��ֵReloadΪ 625 
*/


void IWDG_Config(uint8_t IWDG_Prescaler, uint16_t Reload)
{
	//ʹ��Ԥ��Ƶ�Ĵ���PR����װ�ؼĴ���RLR��д
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	
	//����Ԥ��Ƶ��ֵ
	IWDG_SetPrescaler(IWDG_Prescaler);
	
	//������װ�ؼĴ���ֵ
	IWDG_SetReload(Reload);
	
	//ʹ��IWDG
	IWDG_Enable();
	
	//����װ�ؼĴ�����ֵ�ŵ���������
	IWDG_ReloadCounter();
}
