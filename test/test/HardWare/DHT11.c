#include "dht11.h"
#include "ADC.h"

//����ȫ�ֱ���
char data_buf[12];
DHT11_Data_TypeDef DHT11_Data;						//��ʪ��������С���ֱ𲿷ֵĽṹ��

/*
 * ��������DHT11_GPIO_Config
 * ����  ������DHT11�õ���I/O��
 * ����  ����
 * ���  ����
 */
void DHT11_Config(void)
{		
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*����DHT11_PORT������ʱ��*/
	RCC_AHB1PeriphClockCmd(DHT11_CLK, ENABLE); 

	/*ѡ��Ҫ���Ƶ�DHT11_PORT����*/															   
	GPIO_InitStructure.GPIO_Pin = DHT11_PIN;

	/*��������ģʽΪͨ���������*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;

	/*������������Ϊ50MHz */   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	/*���ÿ⺯������ʼ��DHT11_PORT*/
	GPIO_Init(DHT11_PORT, &GPIO_InitStructure); 
}

/*
 * ��������DHT11_Mode_IPU
 * ����  ��ʹDHT11-DATA���ű�Ϊ��������ģʽ
 * ����  ����
 * ���  ����
 */
static void DHT11_Mode_IPU(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/*ѡ��Ҫ���Ƶ�DHT11_PORT����*/	
	GPIO_InitStructure.GPIO_Pin = DHT11_PIN;

	/*��������ģʽΪ��������ģʽ*/ 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;

	/*������������Ϊ50MHz */   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	/*���ÿ⺯������ʼ��DHT11_PORT*/
	GPIO_Init(DHT11_PORT, &GPIO_InitStructure);	 
}

/*
 * ��������DHT11_Mode_Out_PP
 * ����  ��ʹDHT11-DATA���ű�Ϊ�������ģʽ
 * ����  ����
 * ���  ����
 */
static void DHT11_Mode_Out_PP(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/*ѡ��Ҫ���Ƶ�DHT11_PORT����*/
	GPIO_InitStructure.GPIO_Pin = DHT11_PIN;

	/*��������ģʽΪͨ���������*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;

	/*������������Ϊ50MHz */   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	/*���ÿ⺯������ʼ��DHT11_PORT*/
	GPIO_Init(DHT11_PORT, &GPIO_InitStructure);	 	 
}

/* 
 * ��DHT11��ȡһ���ֽڣ�MSB���У� ��λ����(MSB)�͵�λ����(LSB) ��
 */
static uint8_t Read_Byte(void)
{
	uint8_t i, temp=0;

	for(i=0;i<8;i++)    
	{	 
		/*ÿbit��50us�͵�ƽ���ÿ�ʼ����ѯֱ���ӻ����� ��50us �͵�ƽ ����*/  
		while( DHT11_DATA_IN() == Bit_RESET );

		/*DHT11 ��26~28us�ĸߵ�ƽ��ʾ��0������70us�ߵ�ƽ��ʾ��1����
		*ͨ����� x us��ĵ�ƽ��������������״̬��x ���������ʱ */
		DHT11_DELAY_US(40); //��ʱx us �����ʱ��Ҫ��������0������ʱ�伴��

		if(DHT11_DATA_IN()==Bit_SET)/* x us����Ϊ�ߵ�ƽ��ʾ���ݡ�1�� */
		{
			/* �ȴ�����1�ĸߵ�ƽ���� */
			while(DHT11_DATA_IN()==Bit_SET);

			temp|=(uint8_t)(0x01<<(7-i));  //�ѵ�7-iλ��1��MSB���� 
		}
		else//x us��Ϊ�͵�ƽ��ʾ���ݡ�0��
		{			   
			temp&=(uint8_t)~(0x01<<(7-i)); //�ѵ�7-iλ��0��MSB����
		}
	}
	return temp;
}
/*
 * һ�����������ݴ���Ϊ40bit����λ�ȳ�
 * 8bit ʪ������ + 8bit ʪ��С�� + 8bit �¶����� + 8bit �¶�С�� + 8bit У��� 
 */
uint8_t Read_DHT11(DHT11_Data_TypeDef *DHT11_Data)
{  
	uint16_t count;
	
	/*���ģʽ*/
	DHT11_Mode_Out_PP();
	/*��������*/
	DHT11_DATA_OUT(DHT11_LOW);
	/*��ʱ18ms����*/
	DHT11_DELAY_MS(20);

	/*�������� ������ʱ30us*/
	DHT11_DATA_OUT(DHT11_HIGH); 

	DHT11_DELAY_US(30);	//��ʱ30us

	/*������Ϊ���� �жϴӻ���Ӧ�ź�*/ 
	DHT11_Mode_IPU();

	/*�жϴӻ��Ƿ��е͵�ƽ��Ӧ�ź� �粻��Ӧ����������Ӧ����������*/   
	if(DHT11_DATA_IN()==Bit_RESET)     
	{
		count=0;
		/*��ѯֱ���ӻ����� ��80us �͵�ƽ ��Ӧ�źŽ���*/  
		while(DHT11_DATA_IN()==Bit_RESET)
		{
			count++;
			if(count>1000)  return 0;
			DHT11_DELAY_US(10);
		}    

		count=0;
		/*��ѯֱ���ӻ������� 80us �ߵ�ƽ �����źŽ���*/
		while(DHT11_DATA_IN()==Bit_SET)
		{
			count++;
			if(count>1000)  return 0;
			DHT11_DELAY_US(10);
		}  
		/*��ʼ��������*/   
		DHT11_Data->humi_int= Read_Byte();

		DHT11_Data->humi_deci= Read_Byte();

		DHT11_Data->temp_int= Read_Byte();

		DHT11_Data->temp_deci= Read_Byte();

		DHT11_Data->check_sum= Read_Byte();

		/*��ȡ���������Ÿ�Ϊ���ģʽ*/
		DHT11_Mode_Out_PP();
		/*��������*/
		DHT11_DATA_OUT(DHT11_HIGH);

		/*����ȡ�������Ƿ���ȷ*/
		if(DHT11_Data->check_sum == DHT11_Data->humi_int + DHT11_Data->humi_deci + DHT11_Data->temp_int+ DHT11_Data->temp_deci)
			return 1;
		else 
			return 0;
	}
	else
	{		
		return 0;
	}
}

/* ��ȡ��ʪ������ DHT11 */
struct DHT11_Complete_Data_TypeDef get_dht11_data(void)
{
	struct DHT11_Complete_Data_TypeDef DHT11_Complete_Data;
	char temp_str[20];  // ���ڴ洢�¶��ַ���
  char humi_str[20];  // ���ڴ洢ʪ���ַ���
	char light_str[20];
	//����DHT11_Read_TempAndHumidity��ȡ��ʪ�ȣ����ɹ����������Ϣ
	if( Read_DHT11( &DHT11_Data ) == SUCCESS)
	{
		DHT11_Complete_Data.humi = DHT11_Data.humi_int + DHT11_Data.humi_deci/100.00;
		DHT11_Complete_Data.temp = DHT11_Data.temp_int + DHT11_Data.temp_deci/100.00;
		
				//// �����¶Ⱥ�ʪ�ȵ���ʾ�ַ���
		sprintf(humi_str, "H:%0.0f%%RH T:%0.0lf��", DHT11_Complete_Data.humi,DHT11_Complete_Data.temp);
//            sprintf(humi_str, "Humi:%0.lf%%RH", DHT11_Complete_Data.humi);
//            sprintf(temp_str, "Temp:%0.2lf��", DHT11_Complete_Data.temp);
            sprintf(light_str, "light:%d LUX", light);
            // �� OLED ����ʾ�¶Ⱥ�ʪ������
		      OLED_ShowString(0, 32, (unsigned char *)humi_str, 16);  // ʪ����ʾ
					OLED_ShowString(0, 48, (unsigned char *)light_str, 16); //����
//          OLED_ShowString(0, 48, (unsigned char *)temp_str, 16);      // �¶���ʾ
		
	//	printf("��ȡDHT11�ɹ���ʪ��Ϊ%0.2lf H���¶�Ϊ %0.2lf �档\r\n\r\n",DHT11_Complete_Data.humi,DHT11_Complete_Data.temp);
	}		
	else
	{
		printf("Read DHT11 ERROR!\r\n");
		DHT11_Complete_Data.humi = 0.0;
		DHT11_Complete_Data.temp = 0.0;
	}
	//ע�⣺ DHT11һ��ͨѶʱ�����3ms��������������������鲻С��100ms,Ӧ�����ص�����������
	DHT11_DELAY_MS(1000);
	
	return DHT11_Complete_Data;
}




