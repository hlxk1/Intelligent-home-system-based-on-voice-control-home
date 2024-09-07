#include "IIC.h"


void IIC_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	//1.SDA和SCL为输出模式
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOX, ENABLE);
	GPIO_InitStructure.GPIO_Pin 	= SCL_PIN ;  	//定义引脚编号
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;	      //设置输出模式
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;	    //设置推挽输出
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_100MHz;	//设置输出速率
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;     //上下拉设置
	GPIO_Init(SCL_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin 	= SDA_PIN ;  	//定义引脚编号
	GPIO_Init(SDA_PORT, &GPIO_InitStructure);
	
	/*空闲*/
	IIC_SDA(1);
	IIC_SCL(1);
}


void IIC_SetSDAOutputMode(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	//SDA输出模式
	RCC_AHB1PeriphClockCmd(SDA_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin 	= SDA_PIN;							//定义引脚编号
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;				//设置输出模式
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;			//设置推挽输出
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_100MHz;	//设置输出速率
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;  		//上下拉设置
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
}

void IIC_SetSDAInputMode(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	//SDA输入模式
	RCC_AHB1PeriphClockCmd(SDA_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin 	= SDA_PIN;				//设置引脚编号
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IN;		//输入模式
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;		//上拉电阻设置
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
}

/*开始信号*/
void IIC_Start(void)
{
	/*SCL为高电平，SDA从高电平往低电平跳变*/
	IIC_SetSDAOutputMode();
	IIC_SDA(1);
	IIC_SCL(1);
	iic_delay(5);
	IIC_SDA(0);
	iic_delay(5);  
	IIC_SCL(0);/*准备接收信号*/
}

//停止信号
void IIC_Stop(void)
{
	/*SCL为高电平，SDA从低电平往高电平跳变*/
	IIC_SetSDAOutputMode();
	
	IIC_SDA(0);
	IIC_SCL(0);
	iic_delay(5); 	
	
	IIC_SCL(1);
	iic_delay(5); 
	
	IIC_SDA(1);/*发送总线停止信号*/
	iic_delay(5); 
}


//主机等待从机应答  
uint8_t IIC_Wait_Ack(void)
{
	
	IIC_SetSDAInputMode();
	
	IIC_SCL(0);
	iic_delay(5);
	
	IIC_SCL(1);
	iic_delay(5);
	
	if( IIC_SDA_READ)/*SCL高电平读取SDA状态*/
	{
		IIC_Stop(); /*SDA高电平表示从机NACK*/
		return 1;
	}

	IIC_SCL(0);	/*SCL低电平表示结束ACK检查*/
	iic_delay(5);
	
	return 0;
}

//IIC协议主机应答函数
void IIC_MasterAck(uint8_t ack)
{
	//1.确保SDA引脚为输出模式
	IIC_SetSDAOutputMode();
	
	//2.确保SCL和SDA为低电平
	IIC_SDA(0);
	IIC_SCL(0);
	
	//3.主机应该根据应答信号控制SDA数据线
	if( ack == 0)
	{
		IIC_SDA(0); //SCL时钟线为低电平，此时SDA数据线的数据可以进行切换,此时主机利用SDA数据线发送数据  表示主机应答
	}
	else
		IIC_SDA(1); //表示主机不应答
	
	iic_delay(5);
	
	//4.把SCL时钟线拉高，此时SDA数据线的数据会进行锁存，目的是保持数据稳定，从机才可以读取
	IIC_SCL(1);
	iic_delay(5);

	//5.把SCL时钟线拉低，目的是为了第9个时钟周期结束   此时SDA数据线的数据可以进行切换
	IIC_SCL(0);
	iic_delay(5);
}


//发送 数据or地址
void IIC_SendByte(uint8_t data) 
{
	uint8_t i = 0;
	
	/*SDA引脚为输出模式*/
	IIC_SetSDAOutputMode();
	
	//确保SCL和SDA为低电平
	IIC_SDA(0);
	IIC_SCL(0);
	
	//循环发送8bit  MSB  高位先出
	for(i=0;i<8;i++)
	{
		/*判断data的最高位是0还是1  因为此时SCL为低电平 所以SDA数据线的数据可以修改 */
		if( data & 0x80 )
		{
			IIC_SDA(1);
		}
		else
			IIC_SDA(0);
		
		data <<= 1;/*左移一位用于下次数据发送*/
		
		iic_delay(5);
		
		/*把SCL时钟线拉高，释放主机*/
		IIC_SCL(1);
		iic_delay(5);
		
		/*把SCL时钟线拉低，此时SDA数据线的数据可以进行切换*/
		IIC_SCL(0);
		iic_delay(5);
	}
}
//主机读取数据
uint8_t IIC_ReadByte(void)
{
	uint8_t i = 0;
	uint8_t data = 0;
	
	/*SDA为输入模式*/
	IIC_SetSDAInputMode();
		
	//把SCL时钟线拉低
	IIC_SCL(0);
	iic_delay(5);
	
	/*高位先输出*/
	for(i=0;i<8;i++)
	{
			
		IIC_SCL(1);
		iic_delay(5);
	
		/*收到的数据位进行左移*/
		data <<= 1;
		
		if( IIC_SDA_READ == 1 )
		{
			data |= 0x01; 
		}
		
		/*SCL时钟线拉低*/
		IIC_SCL(0);
		iic_delay(5);
	}
	return data;
}


