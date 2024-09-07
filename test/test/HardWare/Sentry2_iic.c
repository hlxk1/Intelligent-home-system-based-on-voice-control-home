#include "Sentry2_iic.h"
#include "IIC.h"

void Sentry2_Init(void)
{
	IIC_init();
}

void Sentry2_ByteWrite(uint8_t Address,uint8_t Data)
{
	//1.发送I2c开始信号
	IIC_Start();
	
	//2.主机发送Sentry2地址  
	IIC_SendByte(0x60);
	
	//3.等待从机的应答信号
	if( IIC_Wait_Ack() == 1 ) //表示无应答
	{
		printf("Sentry2设备地址有误！（写操作）\r\n");
		IIC_Stop();  //如果从机没有应答，则主机发送停止信号，表示通信结束	
	}

	//4.发送寄存器地址
	IIC_SendByte(Address) ;
	
	//5.等待从机的应答信号
	if( IIC_Wait_Ack() == 1 ) //表示无应答
	{
		printf("Sentry2对应寄存器地址有误！（写操作）\r\n");
		IIC_Stop();  //如果从机没有应答，则主机发送停止信号，表示通信结束	
	}
	
	//6.主机发送数据内容
	IIC_SendByte(Data);
	
	//7.等待从机的应答信号
	if( IIC_Wait_Ack() == 1 ) //表示无应答
	{
		printf("Sentry2数据写入有误！（写操作）\r\n");
		IIC_Stop();  //如果从机没有应答，则主机发送停止信号，表示通信结束	
	}else{
		//8.主机发送停止信号
		IIC_Stop(); 
	
		printf("Sentry2数据写入成功\r\n");
	}
}

uint8_t Sentry2_ReadByte(uint8_t Address)
{
	uint8_t data = 0;
	
	//1.发送I2c开始信号
	IIC_Start();
	
	//2.主机发送Sentry2地址  
	IIC_SendByte(0x61);
	
	//3.等待从机的应答信号
	if( IIC_Wait_Ack() == 1 ) //表示无应答
	{
		printf("Sentry2设备地址有误！（读操作）\r\n");
		IIC_Stop();  //如果从机没有应答，则主机发送停止信号，表示通信结束	
	}

	//4.发送寄存器地址
	IIC_SendByte(Address) ;
	
	//5.等待从机的应答信号
	if( IIC_Wait_Ack() == 1 ) //表示无应答
	{
		printf("Sentry2对应读取寄存器地址有误！（读操作）\r\n");
		IIC_Stop();  //如果从机没有应答，则主机发送停止信号，表示通信结束	
	}
	
	//6.主机发送数据内容
	data = IIC_ReadByte();
	
	//7.等待从机的应答信号
	if( IIC_Wait_Ack() == 1 ) //表示无应答
	{
		printf("Sentry2读取数据有误！（读操作）\r\n");
		IIC_Stop();  //如果从机没有应答，则主机发送停止信号，表示通信结束	
	}
		//8.主机发送停止信号
		IIC_Stop(); 
	
		printf("Sentry2数据读取成功\r\n");
		return data ;
	
}

void Sentry2_test(void)
{
	//uint8_t i = 0;
	uint8_t recv_data = 0;
	//往Sentry2对应寄存器地址中写入1字节数据
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

