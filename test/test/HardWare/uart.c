#include "uart.h"
#include <string.h>
#include <stdio.h>

//����printf��ӡ�˿ڵ�ѡ��
#define USARTX USART1
//#define USARTX USART2
//#define USARTX USART3


void UART1_Config(uint32_t BaudRate)
{
	GPIO_InitTypeDef GPIO_InitStruct;	
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
													   
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;	//UART1_TXD
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_High_Speed; 
	GPIO_Init(GPIOA, &GPIO_InitStruct);	

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;	//UART1_RXD
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);	//���ø��ù���
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	
	USART_InitStruct.USART_BaudRate = BaudRate;	//������
	USART_InitStruct.USART_Parity	= USART_Parity_No;	//�޼���
	USART_InitStruct.USART_WordLength	= USART_WordLength_8b;	//8λ�ֳ�
	USART_InitStruct.USART_StopBits	= USART_StopBits_1;	//1λֹͣλ
	USART_InitStruct.USART_HardwareFlowControl	= USART_HardwareFlowControl_None;	//��Ӳ������
	USART_InitStruct.USART_Mode	= USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART1, &USART_InitStruct);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	//�������ڽ����ж�
	
	NVIC_InitStruct.NVIC_IRQChannel	= USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd	= ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority	= 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority	= 1;
	NVIC_Init(&NVIC_InitStruct);	//����NVIC
	
	USART_Cmd(USART1, ENABLE);	//��������
}

/***���ڶ���������****/
void UART2_Config(uint32_t BaudRate)
{
	// ��ʱ�Ӵ���2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_High_Speed;
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = BaudRate;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;

	USART_Init(USART2,&USART_InitStruct);

	

	
	NVIC_InitTypeDef NVIC2_InitStruct;
	NVIC2_InitStruct.NVIC_IRQChannel = USART2_IRQn;
	NVIC2_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC2_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC2_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC2_InitStruct);
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	
	USART_Cmd(USART2, ENABLE);
}

//����3��ʼ��
void UART3_Config(uint32_t BaudRate)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable GPIO clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	/* Enable USART clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	/* Connect USART pins to PB10 \ PB11*/
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);

	/* Configure USART Tx and Rx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOB, &GPIO_InitStructure);


	USART_InitStructure.USART_BaudRate = BaudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);


	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Enable USART */
	USART_Cmd(USART3, ENABLE);
	
	/* Enable the USARTz Receive Interrupt */
	USART_ITConfig(USART3, USART_IT_RXNE , ENABLE);
}



//�ض���c�⺯��printf�����ڣ��ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
	/* ����һ���ֽ����ݵ����� */
	USART_SendData(USARTX, (uint8_t) ch);

	/* �ȴ�������� */
	while (USART_GetFlagStatus(USARTX, USART_FLAG_TXE) == RESET);		

	return (ch);
}

//�ض���c�⺯��scanf�����ڣ���д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
	/* �ȴ������������� */
	while (USART_GetFlagStatus(USARTX, USART_FLAG_RXNE) == RESET);

	return (int)USART_ReceiveData(USARTX);
}


char rxd_data;	//����������ڴ�Ž�������
char rxd_num;	//����ֵ
char rxd_str[512];	//����ַ���
char rxd_data2;
char r_data2[1024];

void USART2_IRQHandler(void){
	if( USART_GetITStatus(USART2,USART_IT_RXNE) == SET){
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		 //���պ��� Data transfers functions
		rxd_data2 = USART_ReceiveData(USART2);
		sprintf(r_data2,"����2�ɹ���������%c\r\n",rxd_data);
		Usart_SendString(USART1,r_data2);
		
	
	}
	
}
void USART1_IRQHandler(void)  
{
	if( SET == USART_GetITStatus(USART1, USART_IT_RXNE) )
		{
			//��������жϱ�־λ
			USART_ClearITPendingBit(USART1, USART_IT_RXNE);
			
			//��������
			rxd_data = USART_ReceiveData(USART1);
			
			//���ϰѽ��յ��ĵ����ֽ����ݴ��������
			rxd_str[rxd_num++] = rxd_data;
		}
		
		//�жϵ����ֽ�����
		if(rxd_data == 'A')
		{
			GPIO_ToggleBits(GPIOH, GPIO_Pin_11);	
		}
		
		//�ж���������
		if( rxd_str[rxd_num-1] == '.')
		{
			if(0 == strcmp(rxd_str, "hello."))
			{
				GPIO_ToggleBits(GPIOH, GPIO_Pin_11);
			}
			
			//����ֵ����
			rxd_num = 0;
			//�������
			memset(rxd_str, 0, sizeof(rxd_num));
		}
		
		//���ͽ��յ�������
		USART_SendData(USARTX, rxd_data);
		
}



//volatile uint32_t	Rx3Counter	= 0;
//volatile uint8_t	Rx3Data 	= 0;
//volatile uint8_t	Rx3End 		= 0;
//volatile uint8_t	Tx3Buffer[512]={0};
//volatile uint8_t	Rx3Buffer[512]={0};

//void USART3_IRQHandler(void)
//{
//	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
//	{
//		//��������жϱ�־λ
//			USART_ClearITPendingBit(USART3, USART_IT_RXNE);
//		/* Read one byte from the receive data register */
//		Rx3Data = USART_ReceiveData(USART3);	
//	}

//	/* ���ԣ��ѽ��յ��ĵ����ֽ����ݷ��͵� ���ͷ�*/
//	USART_SendData(USART1, Rx3Data);
//}


/*****************  ����һ���ֽ� **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* ����һ���ֽ����ݵ�USART */
	USART_SendData(pUSARTx,ch);
		
	/* �ȴ��������ݼĴ���Ϊ�� */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
	//USART_ClearFlag(pUSARTx,USART_FLAG_TXE);
}

/*****************  ����ָ�����ȵ��ֽ� **********************/
void Usart_SendBytes(USART_TypeDef * pUSARTx, uint8_t *buf,uint32_t len)
{
	uint8_t *p = buf;
	
	while(len--)
	{
		USART_SendData(pUSARTx,*p);
		
		p++;
		
		//�ȴ����ݷ��ͳɹ�
		while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE)==RESET);
		USART_ClearFlag(pUSARTx, USART_FLAG_TXE);
	}
}

/****************** ����8λ������ ************************/
void Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num)
{
	uint16_t i;

	for(i=0; i<num; i++)
	{
		/* ����һ���ֽ����ݵ�USART */
		Usart_SendByte(pUSARTx,array[i]);	
	}
	/* �ȴ�������� */
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
	//USART_ClearFlag(pUSARTx,USART_FLAG_TC);
}


/*****************  �����ַ��� **********************/
void Usart_SendString(USART_TypeDef *pUSARTx, char *str)
{
	unsigned int k = 0;
	do
	{
		Usart_SendByte(pUSARTx, *(str + k));
		k++;
	} while (*(str + k) != '\0');

	/* �ȴ�������� */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET);
}


/*****************  ����һ��16λ�� **********************/
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
	uint8_t temp_h, temp_l;

	/* ȡ���߰�λ */
	temp_h = (ch&0XFF00)>>8;
	/* ȡ���Ͱ�λ */
	temp_l = ch&0XFF;

	/* ���͸߰�λ */
	USART_SendData(pUSARTx,temp_h);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);

	/* ���͵Ͱ�λ */
	USART_SendData(pUSARTx,temp_l);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

