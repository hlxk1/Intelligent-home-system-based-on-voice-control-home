#include "hardiic.h"

/*
	PB6  SCL 
	PB7  SDA 
*/

//i2c1��Ӳ����ʼ��
void i2c1_init(void)
{
	static  int i2c1_ready = 0;	
	GPIO_InitTypeDef GPIO_InitStruct;
	I2C_InitTypeDef I2C_InitStruct;

	if(i2c1_ready == 0)
	{	
		//1.ʱ��ʹ��
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
		
		//2.gpio�Ĺ������ã����ù���
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;   //��©���ģʽ
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 ;
		GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;  //	
		GPIO_Init(GPIOB, &GPIO_InitStruct);  //����������ýṹ����������õ��Ĵ�����
		
		//2.1 ָ������Ϊiic������
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_I2C1);  //��PB8���ó�IICģʽ
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1);	 //��PB9���ó�IICģʽ
	
		
		
		//3.iic�������ĳ�ʼ��
		I2C_StructInit(&I2C_InitStruct);     //Ӧ���Ƿ�Ӧ��
		I2C_InitStruct.I2C_ClockSpeed = 400000;   //����ܳ���400k
		I2C_Init(I2C1, &I2C_InitStruct);
		
		I2C_Cmd(I2C1, ENABLE);
	
		i2c1_ready = 1; //��ʾ�Ѿ���ʼ���ˣ��´ε�����������Ͳ��ٳ�ʼ��
	}
}



//������ʼ�ź�
//����ֵ 0 --�� �ɹ�
//		��0 --��ʧ��
int8_t i2c1_start(void)
{
	uint16_t times = 0;
	
	I2C_GenerateSTART(I2C1, ENABLE);
	
	//���ev5����¼�
	while (I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)!= SUCCESS)
	{
		times ++;
		if(times > 2000)
		{
			return -1;
		}
	}
	return 0;
}



//����ֹͣ�ź�
void i2c1_stop(void)
{
	I2C_GenerateSTOP(I2C1, ENABLE);
}



//���ʹ��豸��ַ
int8_t i2c1_put_devaddr(uint8_t addr)
{
	uint16_t times = 0;
	uint32_t event = I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED;   //������ģʽ
	
	I2C_Send7bitAddress(I2C1, addr, addr&1);
	
	if(addr&1)   //����ģʽ
	{
		event = I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED;
	}
	
	/*
	    @arg I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED: EV6     
        @arg I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED: EV6
	*/
	
	//���ev6
	while (I2C_CheckEvent(I2C1, event)!= SUCCESS)
	{
		times ++;
		if(times > 2000)
		{
			return -1;
		}
	}
	return 0;
}


//��������
void i2c1_put_byte_data(uint8_t dat)
{
	I2C_SendData(I2C1, dat);

	

}



uint8_t i2c1_get_ack(void)
{
	uint16_t times = 0;

	//���ev8_2
	while (I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)!= SUCCESS)
	{
		times ++;
		if(times > 800)
		{
			return 254;
		}
	}
	return 0;
}


uint8_t i2c1_get_byte_data(void)
{
	uint8_t dat;
	
	uint16_t times = 0;

	//���ev7
	while (I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)!= SUCCESS)
	{
		times ++;
		if(times > 2000)
		{
			return 254;
		}
	}
	
	dat = I2C_ReceiveData(I2C1);
	
	return dat;
}

void i2c1_send_ack(uint8_t ack)
{
	if(ack == NO_ACK)  //���ͷ�Ӧ���ź�
	{
		I2C_AcknowledgeConfig(I2C1, DISABLE);
	}
	else
	{
		I2C_AcknowledgeConfig(I2C1, ENABLE);
	}

}

