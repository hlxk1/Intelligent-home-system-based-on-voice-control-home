#include "AT24C02.h"
#include "IIC.h"

//AT24C02的初始化
void AT24C02_Init(void)
{
	IIC_init();
}

//AT24C02字节写入  1字节   内存地址(0x00 ~ 0xFF)  数据
void AT24C02_ByteWrite(uint8_t Address,uint8_t Data)
{
	//1.主机发送开始信号
	IIC_Start();
	
	//2.主机发送器件地址    0xA0  1010 0000 表示主机发给从机
	IIC_SendByte(0xA0) ;
	
	//3.等待从机的应答信号
	if( IIC_Wait_Ack() == 1 ) //表示无应答
	{
		printf("AT24C02器件地址有误！\r\n");
		IIC_Stop();  //如果从机没有应答，则主机发送停止信号，表示通信结束	
	}

	//4.主机发送内存地址
	IIC_SendByte(Address) ;
	
	//5.等待从机的应答信号
	if( IIC_Wait_Ack() == 1 ) //表示无应答
	{
		printf("AT24C02内存地址有误！\r\n");
		IIC_Stop();  //如果从机没有应答，则主机发送停止信号，表示通信结束	
	}
	
	//6.主机发送数据内容
	IIC_SendByte(Data) ;
	
	//7.等待从机的应答信号
	if( IIC_Wait_Ack() == 1 ) //表示无应答
	{
		printf("AT24C02写入数据有误！\r\n");
		IIC_Stop();  //如果从机没有应答，则主机发送停止信号，表示通信结束	
	}
	
	//8.主机发送停止信号
	IIC_Stop(); 
	
	printf("AT24C02往地址写入数据成功\r\n");
}


//AT24C02的页写入函数  一次最多可以写入8字节   需要用户自己计算页地址
void AT24C02_PageWrite(uint8_t PageAddress,uint8_t DataSize,uint8_t *DataPtr)
{
	//1.主机发送开始信号
	IIC_Start();
	
	//2.主机发送器件地址    0xA0  1010 0000 表示写操作
	IIC_SendByte(0xA0) ;
	
	//3.等待从机的应答信号
	if( IIC_Wait_Ack() == 1 ) //表示无应答
	{
		printf("AT24C02器件地址有误！\r\n");
		IIC_Stop();  //如果从机没有应答，则主机发送停止信号，表示通信结束	
	}

	//4.主机发送内存地址
	IIC_SendByte(PageAddress) ;
	
	//5.等待从机的应答信号
	if( IIC_Wait_Ack() == 1 ) //表示无应答
	{
		printf("AT24C02页地址有误！\r\n");
		IIC_Stop();  //如果从机没有应答，则主机发送停止信号，表示通信结束	
	}
	
	//6.主机循环发送数据内容
	while(DataSize--)
	{
		IIC_SendByte(*DataPtr++) ;
		
		//等待从机的应答信号
		if( IIC_Wait_Ack() == 1 ) //表示无应答
		{
			printf("AT24C02读取数据有误！\r\n");
			IIC_Stop();  //如果从机没有应答，则主机发送停止信号，表示通信结束	
		}
	}
	
	//7.主机发送停止信号
	IIC_Stop(); 
}

//AT24C02的当前地址读
uint8_t AT24C02_CurrentAddressRead(void)
{
	uint8_t data=0 ;
	
	//1.主机发送开始信号
	IIC_Start();
	
	//2.主机发送器件地址    0xA0  1010 0001 表示读操作
	IIC_SendByte(0xA1) ;

	//3.等待从机的应答信号
	if( IIC_Wait_Ack() == 1 ) //表示无应答
	{
		printf("AT24C02器件地址读操作有误！\r\n");
		IIC_Stop();  //如果从机没有应答，则主机发送停止信号，表示通信结束	
	}
	
	//4.主机读取一个字节数据
	data = IIC_ReadByte();
	
	//5.主机发送应答信号 无应答
	IIC_MasterAck(0x01);
	
	//6.主机发送停止信号
	IIC_Stop(); 
	
	return data;
}


//AT24C02随机读操作 可以读取某个地址下的数据
uint8_t AT24C02_ReadByte(uint8_t Address)
{
	uint8_t data = 0;
	
	//1.主机发送开始信号
	IIC_Start();
	
	//2.主机发送器件地址    0xA0  1010 0000 表示写操作
	IIC_SendByte(0xA0) ;

	//3.等待从机的应答信号
	if( IIC_Wait_Ack() == 1 ) //表示无应答
	{
		printf("AT24C02器件地址写操作有误！\r\n");
		IIC_Stop();  //如果从机没有应答，则主机发送停止信号，表示通信结束	
	}
	
	//4.主机发送内存地址
	IIC_SendByte(Address) ;
	
	//5.等待从机的应答信号
	if( IIC_Wait_Ack() == 1 ) //表示无应答
	{
		printf("AT24C02内存地址有误！\r\n");
		IIC_Stop();  //如果从机没有应答，则主机发送停止信号，表示通信结束	
	}
	
	//6.主机发送开始信号
	IIC_Start();
	
	//7.主机发送器件地址    0xA1  1010 000 1 表示读操作
	IIC_SendByte(0xA1) ;
		
	//8.等待从机的应答信号
	if( IIC_Wait_Ack() == 1 ) //表示无应答
	{
		printf("AT24C02器件地址读操作有误！\r\n");
		IIC_Stop();  //如果从机没有应答，则主机发送停止信号，表示通信结束	
	}
	
	//9.主机读取一个字节数据
	data = IIC_ReadByte();
	
	//10.主机发送应答信号 无应答
	IIC_MasterAck(0x01);
	
	//11.主机发送停止信号
	IIC_Stop(); 
	
	return data;
}

//AT24C02读写测试函数
void AT24C02_RW(void)
{
	uint8_t i = 0;
	uint8_t recv_data = 0;
	
	//往AT24C02中写入1字节数据
	AT24C02_ByteWrite(0x00,'M');
	
	Delay_ms(500);
	
	//从AT24C02的地址下读取数据
	recv_data = AT24C02_ReadByte(0x00);
	printf(" AT24C02 Read byte %c \r\n",recv_data);
	
	//往AT24C02的第0页中写入数据  "nihao"
	AT24C02_PageWrite(0x00,8,(uint8_t *)"HAHAHAHA");
	
	Delay_ms(500);
	
	//从AT24C02的第0页地址下读取数据
	for(i=0;i<8;i++)
	{
		recv_data = AT24C02_ReadByte(0x00+i);
		printf(" AT24C02 Read Data %d is %c \r\n",i,recv_data);
		Delay_ms(100);
	}
	
	printf("\r\n");
}
