#include "IIC.h"


void IIC_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	//1.SDA��SCLΪ���ģʽ
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOX, ENABLE);
	GPIO_InitStructure.GPIO_Pin 	= SCL_PIN ;  	//�������ű��
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;	      //�������ģʽ
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;	    //�����������
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_100MHz;	//�����������
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;     //����������
	GPIO_Init(SCL_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin 	= SDA_PIN ;  	//�������ű��
	GPIO_Init(SDA_PORT, &GPIO_InitStructure);
	
	/*����*/
	IIC_SDA(1);
	IIC_SCL(1);
}


void IIC_SetSDAOutputMode(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	//SDA���ģʽ
	RCC_AHB1PeriphClockCmd(SDA_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin 	= SDA_PIN;							//�������ű��
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;				//�������ģʽ
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;			//�����������
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_100MHz;	//�����������
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;  		//����������
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
}

void IIC_SetSDAInputMode(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	//SDA����ģʽ
	RCC_AHB1PeriphClockCmd(SDA_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin 	= SDA_PIN;				//�������ű��
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IN;		//����ģʽ
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;		//������������
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
}

/*��ʼ�ź�*/
void IIC_Start(void)
{
	/*SCLΪ�ߵ�ƽ��SDA�Ӹߵ�ƽ���͵�ƽ����*/
	IIC_SetSDAOutputMode();
	IIC_SDA(1);
	IIC_SCL(1);
	iic_delay(5);
	IIC_SDA(0);
	iic_delay(5);  
	IIC_SCL(0);/*׼�������ź�*/
}

//ֹͣ�ź�
void IIC_Stop(void)
{
	/*SCLΪ�ߵ�ƽ��SDA�ӵ͵�ƽ���ߵ�ƽ����*/
	IIC_SetSDAOutputMode();
	
	IIC_SDA(0);
	IIC_SCL(0);
	iic_delay(5); 	
	
	IIC_SCL(1);
	iic_delay(5); 
	
	IIC_SDA(1);/*��������ֹͣ�ź�*/
	iic_delay(5); 
}


//�����ȴ��ӻ�Ӧ��  
uint8_t IIC_Wait_Ack(void)
{
	
	IIC_SetSDAInputMode();
	
	IIC_SCL(0);
	iic_delay(5);
	
	IIC_SCL(1);
	iic_delay(5);
	
	if( IIC_SDA_READ)/*SCL�ߵ�ƽ��ȡSDA״̬*/
	{
		IIC_Stop(); /*SDA�ߵ�ƽ��ʾ�ӻ�NACK*/
		return 1;
	}

	IIC_SCL(0);	/*SCL�͵�ƽ��ʾ����ACK���*/
	iic_delay(5);
	
	return 0;
}

//IICЭ������Ӧ����
void IIC_MasterAck(uint8_t ack)
{
	//1.ȷ��SDA����Ϊ���ģʽ
	IIC_SetSDAOutputMode();
	
	//2.ȷ��SCL��SDAΪ�͵�ƽ
	IIC_SDA(0);
	IIC_SCL(0);
	
	//3.����Ӧ�ø���Ӧ���źſ���SDA������
	if( ack == 0)
	{
		IIC_SDA(0); //SCLʱ����Ϊ�͵�ƽ����ʱSDA�����ߵ����ݿ��Խ����л�,��ʱ��������SDA�����߷�������  ��ʾ����Ӧ��
	}
	else
		IIC_SDA(1); //��ʾ������Ӧ��
	
	iic_delay(5);
	
	//4.��SCLʱ�������ߣ���ʱSDA�����ߵ����ݻ�������棬Ŀ���Ǳ��������ȶ����ӻ��ſ��Զ�ȡ
	IIC_SCL(1);
	iic_delay(5);

	//5.��SCLʱ�������ͣ�Ŀ����Ϊ�˵�9��ʱ�����ڽ���   ��ʱSDA�����ߵ����ݿ��Խ����л�
	IIC_SCL(0);
	iic_delay(5);
}


//���� ����or��ַ
void IIC_SendByte(uint8_t data) 
{
	uint8_t i = 0;
	
	/*SDA����Ϊ���ģʽ*/
	IIC_SetSDAOutputMode();
	
	//ȷ��SCL��SDAΪ�͵�ƽ
	IIC_SDA(0);
	IIC_SCL(0);
	
	//ѭ������8bit  MSB  ��λ�ȳ�
	for(i=0;i<8;i++)
	{
		/*�ж�data�����λ��0����1  ��Ϊ��ʱSCLΪ�͵�ƽ ����SDA�����ߵ����ݿ����޸� */
		if( data & 0x80 )
		{
			IIC_SDA(1);
		}
		else
			IIC_SDA(0);
		
		data <<= 1;/*����һλ�����´����ݷ���*/
		
		iic_delay(5);
		
		/*��SCLʱ�������ߣ��ͷ�����*/
		IIC_SCL(1);
		iic_delay(5);
		
		/*��SCLʱ�������ͣ���ʱSDA�����ߵ����ݿ��Խ����л�*/
		IIC_SCL(0);
		iic_delay(5);
	}
}
//������ȡ����
uint8_t IIC_ReadByte(void)
{
	uint8_t i = 0;
	uint8_t data = 0;
	
	/*SDAΪ����ģʽ*/
	IIC_SetSDAInputMode();
		
	//��SCLʱ��������
	IIC_SCL(0);
	iic_delay(5);
	
	/*��λ�����*/
	for(i=0;i<8;i++)
	{
			
		IIC_SCL(1);
		iic_delay(5);
	
		/*�յ�������λ��������*/
		data <<= 1;
		
		if( IIC_SDA_READ == 1 )
		{
			data |= 0x01; 
		}
		
		/*SCLʱ��������*/
		IIC_SCL(0);
		iic_delay(5);
	}
	return data;
}


