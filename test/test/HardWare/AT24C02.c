#include "AT24C02.h"
#include "IIC.h"

//AT24C02�ĳ�ʼ��
void AT24C02_Init(void)
{
	IIC_init();
}

//AT24C02�ֽ�д��  1�ֽ�   �ڴ��ַ(0x00 ~ 0xFF)  ����
void AT24C02_ByteWrite(uint8_t Address,uint8_t Data)
{
	//1.�������Ϳ�ʼ�ź�
	IIC_Start();
	
	//2.��������������ַ    0xA0  1010 0000 ��ʾ���������ӻ�
	IIC_SendByte(0xA0) ;
	
	//3.�ȴ��ӻ���Ӧ���ź�
	if( IIC_Wait_Ack() == 1 ) //��ʾ��Ӧ��
	{
		printf("AT24C02������ַ����\r\n");
		IIC_Stop();  //����ӻ�û��Ӧ������������ֹͣ�źţ���ʾͨ�Ž���	
	}

	//4.���������ڴ��ַ
	IIC_SendByte(Address) ;
	
	//5.�ȴ��ӻ���Ӧ���ź�
	if( IIC_Wait_Ack() == 1 ) //��ʾ��Ӧ��
	{
		printf("AT24C02�ڴ��ַ����\r\n");
		IIC_Stop();  //����ӻ�û��Ӧ������������ֹͣ�źţ���ʾͨ�Ž���	
	}
	
	//6.����������������
	IIC_SendByte(Data) ;
	
	//7.�ȴ��ӻ���Ӧ���ź�
	if( IIC_Wait_Ack() == 1 ) //��ʾ��Ӧ��
	{
		printf("AT24C02д����������\r\n");
		IIC_Stop();  //����ӻ�û��Ӧ������������ֹͣ�źţ���ʾͨ�Ž���	
	}
	
	//8.��������ֹͣ�ź�
	IIC_Stop(); 
	
	printf("AT24C02����ַд�����ݳɹ�\r\n");
}


//AT24C02��ҳд�뺯��  һ��������д��8�ֽ�   ��Ҫ�û��Լ�����ҳ��ַ
void AT24C02_PageWrite(uint8_t PageAddress,uint8_t DataSize,uint8_t *DataPtr)
{
	//1.�������Ϳ�ʼ�ź�
	IIC_Start();
	
	//2.��������������ַ    0xA0  1010 0000 ��ʾд����
	IIC_SendByte(0xA0) ;
	
	//3.�ȴ��ӻ���Ӧ���ź�
	if( IIC_Wait_Ack() == 1 ) //��ʾ��Ӧ��
	{
		printf("AT24C02������ַ����\r\n");
		IIC_Stop();  //����ӻ�û��Ӧ������������ֹͣ�źţ���ʾͨ�Ž���	
	}

	//4.���������ڴ��ַ
	IIC_SendByte(PageAddress) ;
	
	//5.�ȴ��ӻ���Ӧ���ź�
	if( IIC_Wait_Ack() == 1 ) //��ʾ��Ӧ��
	{
		printf("AT24C02ҳ��ַ����\r\n");
		IIC_Stop();  //����ӻ�û��Ӧ������������ֹͣ�źţ���ʾͨ�Ž���	
	}
	
	//6.����ѭ��������������
	while(DataSize--)
	{
		IIC_SendByte(*DataPtr++) ;
		
		//�ȴ��ӻ���Ӧ���ź�
		if( IIC_Wait_Ack() == 1 ) //��ʾ��Ӧ��
		{
			printf("AT24C02��ȡ��������\r\n");
			IIC_Stop();  //����ӻ�û��Ӧ������������ֹͣ�źţ���ʾͨ�Ž���	
		}
	}
	
	//7.��������ֹͣ�ź�
	IIC_Stop(); 
}

//AT24C02�ĵ�ǰ��ַ��
uint8_t AT24C02_CurrentAddressRead(void)
{
	uint8_t data=0 ;
	
	//1.�������Ϳ�ʼ�ź�
	IIC_Start();
	
	//2.��������������ַ    0xA0  1010 0001 ��ʾ������
	IIC_SendByte(0xA1) ;

	//3.�ȴ��ӻ���Ӧ���ź�
	if( IIC_Wait_Ack() == 1 ) //��ʾ��Ӧ��
	{
		printf("AT24C02������ַ����������\r\n");
		IIC_Stop();  //����ӻ�û��Ӧ������������ֹͣ�źţ���ʾͨ�Ž���	
	}
	
	//4.������ȡһ���ֽ�����
	data = IIC_ReadByte();
	
	//5.��������Ӧ���ź� ��Ӧ��
	IIC_MasterAck(0x01);
	
	//6.��������ֹͣ�ź�
	IIC_Stop(); 
	
	return data;
}


//AT24C02��������� ���Զ�ȡĳ����ַ�µ�����
uint8_t AT24C02_ReadByte(uint8_t Address)
{
	uint8_t data = 0;
	
	//1.�������Ϳ�ʼ�ź�
	IIC_Start();
	
	//2.��������������ַ    0xA0  1010 0000 ��ʾд����
	IIC_SendByte(0xA0) ;

	//3.�ȴ��ӻ���Ӧ���ź�
	if( IIC_Wait_Ack() == 1 ) //��ʾ��Ӧ��
	{
		printf("AT24C02������ַд��������\r\n");
		IIC_Stop();  //����ӻ�û��Ӧ������������ֹͣ�źţ���ʾͨ�Ž���	
	}
	
	//4.���������ڴ��ַ
	IIC_SendByte(Address) ;
	
	//5.�ȴ��ӻ���Ӧ���ź�
	if( IIC_Wait_Ack() == 1 ) //��ʾ��Ӧ��
	{
		printf("AT24C02�ڴ��ַ����\r\n");
		IIC_Stop();  //����ӻ�û��Ӧ������������ֹͣ�źţ���ʾͨ�Ž���	
	}
	
	//6.�������Ϳ�ʼ�ź�
	IIC_Start();
	
	//7.��������������ַ    0xA1  1010 000 1 ��ʾ������
	IIC_SendByte(0xA1) ;
		
	//8.�ȴ��ӻ���Ӧ���ź�
	if( IIC_Wait_Ack() == 1 ) //��ʾ��Ӧ��
	{
		printf("AT24C02������ַ����������\r\n");
		IIC_Stop();  //����ӻ�û��Ӧ������������ֹͣ�źţ���ʾͨ�Ž���	
	}
	
	//9.������ȡһ���ֽ�����
	data = IIC_ReadByte();
	
	//10.��������Ӧ���ź� ��Ӧ��
	IIC_MasterAck(0x01);
	
	//11.��������ֹͣ�ź�
	IIC_Stop(); 
	
	return data;
}

//AT24C02��д���Ժ���
void AT24C02_RW(void)
{
	uint8_t i = 0;
	uint8_t recv_data = 0;
	
	//��AT24C02��д��1�ֽ�����
	AT24C02_ByteWrite(0x00,'M');
	
	Delay_ms(500);
	
	//��AT24C02�ĵ�ַ�¶�ȡ����
	recv_data = AT24C02_ReadByte(0x00);
	printf(" AT24C02 Read byte %c \r\n",recv_data);
	
	//��AT24C02�ĵ�0ҳ��д������  "nihao"
	AT24C02_PageWrite(0x00,8,(uint8_t *)"HAHAHAHA");
	
	Delay_ms(500);
	
	//��AT24C02�ĵ�0ҳ��ַ�¶�ȡ����
	for(i=0;i<8;i++)
	{
		recv_data = AT24C02_ReadByte(0x00+i);
		printf(" AT24C02 Read Data %d is %c \r\n",i,recv_data);
		Delay_ms(100);
	}
	
	printf("\r\n");
}
