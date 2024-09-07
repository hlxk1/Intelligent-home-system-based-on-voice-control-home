#include "Sentry2_iic.h"
#include "IIC.h"

void Sentry2_Init(void)
{
	IIC_init();
}

void Sentry2_ByteWrite(uint8_t Address,uint8_t Data)
{
	//1.����I2c��ʼ�ź�
	IIC_Start();
	
	//2.��������Sentry2��ַ  
	IIC_SendByte(0x60);
	
	//3.�ȴ��ӻ���Ӧ���ź�
	if( IIC_Wait_Ack() == 1 ) //��ʾ��Ӧ��
	{
		printf("Sentry2�豸��ַ���󣡣�д������\r\n");
		IIC_Stop();  //����ӻ�û��Ӧ������������ֹͣ�źţ���ʾͨ�Ž���	
	}

	//4.���ͼĴ�����ַ
	IIC_SendByte(Address) ;
	
	//5.�ȴ��ӻ���Ӧ���ź�
	if( IIC_Wait_Ack() == 1 ) //��ʾ��Ӧ��
	{
		printf("Sentry2��Ӧ�Ĵ�����ַ���󣡣�д������\r\n");
		IIC_Stop();  //����ӻ�û��Ӧ������������ֹͣ�źţ���ʾͨ�Ž���	
	}
	
	//6.����������������
	IIC_SendByte(Data);
	
	//7.�ȴ��ӻ���Ӧ���ź�
	if( IIC_Wait_Ack() == 1 ) //��ʾ��Ӧ��
	{
		printf("Sentry2����д�����󣡣�д������\r\n");
		IIC_Stop();  //����ӻ�û��Ӧ������������ֹͣ�źţ���ʾͨ�Ž���	
	}else{
		//8.��������ֹͣ�ź�
		IIC_Stop(); 
	
		printf("Sentry2����д��ɹ�\r\n");
	}
}

uint8_t Sentry2_ReadByte(uint8_t Address)
{
	uint8_t data = 0;
	
	//1.����I2c��ʼ�ź�
	IIC_Start();
	
	//2.��������Sentry2��ַ  
	IIC_SendByte(0x61);
	
	//3.�ȴ��ӻ���Ӧ���ź�
	if( IIC_Wait_Ack() == 1 ) //��ʾ��Ӧ��
	{
		printf("Sentry2�豸��ַ���󣡣���������\r\n");
		IIC_Stop();  //����ӻ�û��Ӧ������������ֹͣ�źţ���ʾͨ�Ž���	
	}

	//4.���ͼĴ�����ַ
	IIC_SendByte(Address) ;
	
	//5.�ȴ��ӻ���Ӧ���ź�
	if( IIC_Wait_Ack() == 1 ) //��ʾ��Ӧ��
	{
		printf("Sentry2��Ӧ��ȡ�Ĵ�����ַ���󣡣���������\r\n");
		IIC_Stop();  //����ӻ�û��Ӧ������������ֹͣ�źţ���ʾͨ�Ž���	
	}
	
	//6.����������������
	data = IIC_ReadByte();
	
	//7.�ȴ��ӻ���Ӧ���ź�
	if( IIC_Wait_Ack() == 1 ) //��ʾ��Ӧ��
	{
		printf("Sentry2��ȡ�������󣡣���������\r\n");
		IIC_Stop();  //����ӻ�û��Ӧ������������ֹͣ�źţ���ʾͨ�Ž���	
	}
		//8.��������ֹͣ�ź�
		IIC_Stop(); 
	
		printf("Sentry2���ݶ�ȡ�ɹ�\r\n");
		return data ;
	
}

void Sentry2_test(void)
{
	//uint8_t i = 0;
	uint8_t recv_data = 0;
	//��Sentry2��Ӧ�Ĵ�����ַ��д��1�ֽ�����
	Sentry2_ByteWrite(0x20,1);
	Delay_ms(500);
	//Sentry2_ByteWrite(0x21,0x01);
	//Delay_ms(500);
	
	/*for(i=0;i<8;i++)
	{
		recv_data = Sentry2_ReadByte(0x21);
		printf(" Sentry2 Read Data %d is %c \r\n",i,recv_data);
		Delay_ms(100);
	}
	*/
	printf(" Sentry2 Read Data %d \r\n",recv_data);
	printf("\r\n");
}

