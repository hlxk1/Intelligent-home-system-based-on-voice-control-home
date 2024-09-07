#include "esp8266.h"

//usart3���ͺͽ�������
uint8_t usart3_txbuf[256];
uint8_t usart3_rxbuf[512];
uint8_t usart3_rxcounter;

char recv_buf[512];
uint8_t cnt1 = 0,esp8266_cnt1Pre = 0;
uint8_t num = 0;
char rxd3_str[512];

volatile uint32_t	Rx3Counter	= 0;
volatile uint8_t	Rx3Data 	= 0;
volatile uint8_t	Rx3End 		= 0;
volatile uint8_t	Tx3Buffer[512]={0};
volatile uint8_t	Rx3Buffer[512]={0};

_Bool connect_ack = 1;

/**
 * @brief ESP8266��ʼ��
 * ͨ��ATָ������esp8266ģʽ�������ƶ�
 *
 * @return û�з��ز���
 */
void ESP8266_Init(void)
{

	ESP8266_memset_RecvBuff();
	/*�˳�͸��ģʽ*/
	while(ESP8266_SendCmd("+++",""));
	Delay_ms(500);
	/*����ESP8266�Ƿ�����������AT����*/
	printf("AT \r\n");
	while(ESP8266_SendCmd("AT\r\n","OK"));
	Delay_ms(500);
	/*ESP8266������������*/
	
	printf("AT+RST \r\n");
	while(ESP8266_SendCmd("AT+RST\r\n",""));
	//while(ESP8266_SendCmd("AT+RESTORE\r\n",""));
	Delay_ms(500);
	/*������ݻش���ʾ��������*/
	//printf("ATE0\r\n");
	//while(ESP8266_SendCmd("ATE0\r\n","OK"));
	//Delay_ms(500);
	printf("ESP8266 Init OK\r\n");
	
	
	/*******************����������wifi�豸*******************************/
	/*����ESP8266ΪSTAģʽ���ͻ���ģʽ��*/
	printf("AT+CWMODE_CUR\r\n");
	while(ESP8266_SendCmd("AT+CWMODE_CUR=1\r\n", "OK"))
	Delay_ms(3000);
	/*���ö�Ӧwifi���˺ź�����*/
	printf("AT+CWJAP\r\n");
	while(ESP8266_SendCmd(ESP8266_WIFI_INFO, "OK"))
	Delay_ms(3000);
	

//	while(ESP8266_SendWIFI(ESP8266_WIFI_INFO, "OK"))
//	Delay_ms(3000);
	
	printf("ESP8266 ����WIFI�ɹ�������\r\n");
	
	/*****************�����Ʒ�����******************************/
	ESP8266_ConnectServer();
	printf("ESP8266 �����Ʒ������ɹ�������\r\n");
	
	/*********************��¼����******************************/
	do{
		ESP8266_memset_RecvBuff();
		MQTT_Connect();
		Delay_ms(500);
	/****************ͨ�����ڷ����ж��Ƿ��ĳɹ�******************/
		if(recv_buf[0] == 0x20 && recv_buf[1] == 0x02)
		{
			printf("��¼�Ʒ�������MQTT�ɹ�������\r\n");
			connect_ack = 0;
		}
	}while(connect_ack != 0);
	connect_ack = 1;

	/*****************�����Ʒ������******************************/
	do{
		ESP8266_memset_RecvBuff();
		MQTT_SubscribeTopic();
		Delay_ms(500);
		/****************ͨ�����ڷ����ж��Ƿ��ĳɹ�******************/
		if((recv_buf[0] == 0x90 && recv_buf[1] == 0x03 && recv_buf[2] == 0x00 && recv_buf[3] == 0x01 && recv_buf[4] == 0x00)||(recv_buf[0] == 0x90 && recv_buf[1] == 0x03 && recv_buf[2] == 0x00 && recv_buf[3] == 0x01 && recv_buf[4] == 0x01))
		{
			printf("����MQTT����ɹ�������\r\n");
			connect_ack = 0;
		}
	}while(connect_ack != 0);
	connect_ack = 1;
	/*********************ȡ������********************************/
//	MQTT_UNSubscribeTopic();
	/*****************�Ͽ��Ʒ������******************************/
//	MQTT_Disconnect();
//  printf("MQTT�Ͽ��������ɹ�������\r\n");
//	printf("\r\n");
}


/**
 * @brief �����������������
 * 
 *
 * @return û�з��ز���
 */
void ESP8266_memset_RecvBuff(void)
{
	//��շ��ͺͽ�������
	memset(usart3_txbuf,0,sizeof(usart3_txbuf));
	memset(usart3_rxbuf,0,sizeof(usart3_rxbuf));
	
	memset(recv_buf, 0, sizeof(recv_buf));
	cnt1 = 0;
}


/**
 * @brief ESP8266��������
 * ͨ��������ESP8266�����������˶Է��ؽ���Ƿ���ȷ
 *
 * @return �ɹ�����0��ʧ�ܷ���1
 */
_Bool ESP8266_SendCmd(char *str, char* rev)
{
	unsigned char timeOut = 200;
	ESP8266_memset_RecvBuff();

	Usart_SendString(USART3,str);//��esp8266����ָ��
	while(timeOut--) //��ʱ����
	{
		if( ESP8266_WaitRecive()==REV_OK)
		{
			if(strstr((const char *)recv_buf, rev) != NULL)	//����Żؽ����ȷ
			{
				ESP8266_memset_RecvBuff();									//��ջ���
				return 0;
			}
		}
		Delay_ms(10);
	}
		return 1;
}
_Bool ESP8266_SendWIFI(char *str, char* rev)
{
//	unsigned char timeOut = 200;
	ESP8266_memset_RecvBuff();

	Usart_SendString(USART3,str);//��esp8266����ָ��
	Delay_ms(2000);
		if( ESP8266_WaitRecive()==REV_OK)
		{
			if(strstr((const char *)recv_buf, rev) != NULL)	//����Żؽ����ȷ
			{
				ESP8266_memset_RecvBuff();									//��ջ���
				return 0;
			}
		}
	Delay_ms(1000);	
	
		return 1;
}

/**
 * @brief ESP8266��͸��ģʽ
 * 
 *
 * @return û�з��ؽ��
 */
void ESP8266_OpenTransmission(void)
{
	//����͸��ģʽ
		memset(usart3_rxbuf,0,sizeof(usart3_rxbuf));    
		while(ESP8266_SendCmd("AT+CIPMODE=1\r\n","OK"))
		Delay_ms(500);  
}


/**
 * @brief ESP8266�˳�͸��ģʽ
 * 
 *
 * @return û�з��ؽ��
 */
void ESP8266_ExitUnvarnishedTrans(void)
{
	while(ESP8266_SendCmd("+++",""))
	Delay_ms(500);
}



/**
 * @brief ESP8266�����Ʒ�����
 * 
 *
 * @return û�з��ؽ��
 */
void ESP8266_ConnectServer(void)
{
	//���ӷ�����
	printf("AT+CIPSTART");
	while(ESP8266_SendCmd(ESP8266_SERVER,"CONNECT"))
	Delay_ms(500);

	//����͸��ģʽ
	ESP8266_OpenTransmission();
	
	//��������״̬
	printf("AT+CIPSEND"); 
	while(ESP8266_SendCmd("AT+CIPSEND\r\n",">"))
	Delay_ms(500);//��ʼ����͸������״̬

}

char rxd3_num;	   // �洢���յ��ĵ����ֽ����ݵ�����
/**
 * @brief ����3���жϻص�����
 * ����ESP8266���͵���Ϣ��ӡ��PC�ˣ����Է������·����ݽ��д���
 *
 * @return û�з��ؽ��
 */
void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		//�ӼĴ����л�ȡ���յ�������
		Rx3Data = USART_ReceiveData(USART3);	
		recv_buf[cnt1++] = USART3->DR;
		rxd3_str[rxd3_num++] = Rx3Data;
		//��������жϱ�־λ
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	}
	/*��������������ʱ�����д���*/
	MQTT_RX_DATE_DEAL(recv_buf);
	/* ���ԣ��ѽ��յ��ĵ����ֽ����ݷ��͵����ͷ�*/
	USART_SendData(USART1, Rx3Data);

}


/**
 * @brief �ȴ����յ�������
 * 
 *
 * @return ���ؽ�����ɱ�־/δ��ɱ�־
 */
_Bool ESP8266_WaitRecive(void)
{
	if(cnt1 == 0) 							//������ռ���Ϊ0 ��˵��û�д��ڽ��������У�����ֱ����������������
		return REV_WAIT;
	if(cnt1 == esp8266_cnt1Pre)				//�����һ�ε�ֵ�������ͬ����˵���������
	{
		cnt1 = 0;							//��0���ռ���
		return REV_OK;								//���ؽ�����ɱ�־
	}
	esp8266_cnt1Pre = cnt1;					//��Ϊ��ͬ
	return REV_WAIT;								//���ؽ���δ��ɱ�־
}


/**
 * @brief �������ݴ���
 * 
 *
 * @return û�з��ز���
 */
void MQTT_RX_DATE_DEAL(char *buf)
{
	char *ptr;
	ptr = strstr(buf,"LED");
 // ʹ��strstr�����ҵ�"LED"����ʼλ��
  ptr = strstr(buf, "\"LED\"");
  if (ptr != NULL) {
    // �ҵ���"LED"����ʼλ�ã�������":"��λ��
    ptr = strchr(ptr, ':');
    if (ptr != NULL) {
      // ���������ֵ�λ��
      ptr++;
      while (*ptr == ' ') {
        ptr++;
      }
      // �ж������Ƿ�Ϊ0��1
      if (*ptr == '0') {
        printf("LED��ֵΪ0\n");
				memset(recv_buf,0,sizeof(recv_buf));
//				GPIO_ResetBits(GPIOG, GPIO_Pin_14); //����led��
      } else if (*ptr == '1') {
        printf("LED��ֵΪ1\n");
				memset(recv_buf,0,sizeof(recv_buf));
//				GPIO_SetBits(GPIOG, GPIO_Pin_14); //Ϩ��led��
      } 
    }
  }
		ptr = NULL;
}

void restart_ESP8266(void) {
    printf("Restarting ESP8266...\n");

		/*********************ȡ������********************************/
	MQTT_UNSubscribeTopic();
	/*****************�Ͽ��Ʒ������******************************/
	MQTT_Disconnect();
  printf("MQTT�Ͽ��������ɹ�������\r\n");
	printf("\r\n");
	 
	Delay_ms(200);
	/*�˳�͸��ģʽ*/
	while(ESP8266_SendCmd("+++",""));
	Delay_ms(500);
	/*����ESP8266�Ƿ�����������AT����*/
	printf("AT \r\n");
	while(ESP8266_SendCmd("AT\r\n","OK"));
	Delay_ms(500);
	/*ESP8266������������*/
//	
//    // ���� AT+RST ���������� ESP8266
//    while (ESP8266_SendCmd("AT+RST\r\n", "ready")) {
//        printf("Failed to restart ESP8266, retrying...\n");
//    }

//    // �ӳٵȴ� ESP8266 �������
//    Delay_ms(5000); // 5���ӳ���ȷ�� ESP8266 ��������

//    // ��� ESP8266 �Ƿ��ѳɹ�������׼������
//    while (ESP8266_SendCmd("AT\r\n", "OK")) {
//        printf("ESP8266 restart failed, retrying...\n");
//    }

//    printf("ESP8266 restarted successfully.\n");
}


void use_AT(void) {
    ESP8266_memset_RecvBuff(); // ��ս��ջ�����

    // ���� AT ���� 
    while (ESP8266_SendCmd("AT\r\n", "OK")) {
        printf("AT command failed, retrying...\n");
    }

    // ���� Wi-Fi ģʽΪ Station ģʽ  
    while (ESP8266_SendCmd("AT+CWMODE_DEF=1\r\n", "OK")) {
        printf("Failed to set Wi-Fi mode, retrying...\n");
    }
//#define ESP8266_WIFI_INFO		"AT+CWJAP=\"kyrie\",\"51522zzwlwlbb\"\r\n" //wifi���˺ź�����
		//AT+CWJAP=\"Mate40\",\"12341234\"\r\n
    // ���ӵ� Wi-Fi
    while (ESP8266_SendCmd("AT+CWJAP=\"kyrie\",\"51522zzwlwlbb\"\r\n", "OK")) {
        printf("Failed to connect to Wi-Fi, retrying...\n");
        Delay_ms(2000);
    }

    // �Զ���������  
    while (ESP8266_SendCmd("AT+CWAUTOCONN=1\r\n", "OK")) {
        printf("Failed to set Auto Connect, retrying...\n");
    }

    // ���� TCP ����
    while (ESP8266_SendCmd("AT+CIPSTART=\"TCP\",\"restapi.amap.com\",80\r\n", "CONNECT")) {
        printf("Failed to establish TCP connection, retrying...\n");
    }

    // ����͸��ģʽ
    while (ESP8266_SendCmd("AT+CIPMODE=1\r\n", "OK")) {
        printf("Failed to set Transparent Transmission mode, retrying...\n");
    }

    // ��ʼ��������
    while (ESP8266_SendCmd("AT+CIPSEND\r\n", ">")) {
        printf("Failed to start data transmission, retrying...\n");
    }

    // ���� HTTP GET ����
    ESP8266_SendCmd("GET https://restapi.amap.com/v3/weather/weatherInfo?key=4392aa8b297b5903497f166c1ef71e0c&city=210100\r\n", "");

    Delay_ms(20);

    // �˳�͸��ģʽ
    printf("Exiting Transparent Transmission mode...\n");
    ESP8266_ExitUnvarnishedTrans();
    Delay_ms(500);

    // �ٴβ��� AT ������ȷ�� ESP8266 ��Ȼ��������
    while (ESP8266_SendCmd("AT\r\n", "OK")) {
        printf("Final AT command failed, retrying...\n");
    }

    printf("AT command sequence completed successfully.\n");
		
//		memset(rxd3_str,0,sizeof(rxd3_str));
}



// ��ȡ"weather"�ֶε�ֵ
void extract_weather(const char *json, u8 *weather, size_t weather_size)
{
    const char *weather_key = "\"weather\":\"";
    const char *weather_start;
    const char *weather_end;

    // ����"weather"�ֶε�λ��
    weather_start = strstr(json, weather_key);
    if (weather_start == NULL)
    {
        printf("Weather key not found.\n");
        return;
    }

    // �ƶ����ֶ�ֵ�Ŀ�ʼλ��
    weather_start += strlen(weather_key);

    // �����ֶ�ֵ�Ľ���λ��
    weather_end = strchr(weather_start, '"');
    if (weather_end == NULL)
    {
        printf("Weather value end not found.\n");
        return;
    }

    // �����ֶ�ֵ�ĳ���
    size_t weather_len = weather_end - weather_start;
    if (weather_len >= weather_size)
    {
        printf("Buffer size is too small.\n");
        return;
    }

    // �����ֶ�ֵ��Ŀ�껺����
    strncpy((char *)weather, weather_start, weather_len);
    weather[weather_len] = '\0'; // ȷ���ַ�����'\0'��β
}



void extract_temperature_str(const char *json, u8 *temperature, size_t temp_size)
{
    const char *temp_key = "\"temperature\":\"";
    const char *temp_start;
    const char *temp_end;

    // ����"temperature"�ֶε�λ��
    temp_start = strstr(json, temp_key);
    if (temp_start == NULL)
    {
        printf("Temperature key not found.\n");
        return;
    }

    // �ƶ����ֶ�ֵ�Ŀ�ʼλ��
    temp_start += strlen(temp_key);

    // �����ֶ�ֵ�Ľ���λ��
    temp_end = strchr(temp_start, '"');
    if (temp_end == NULL)
    {
        printf("Temperature value end not found.\n");
        return;
    }

    // �����ֶ�ֵ�ĳ���
    size_t temp_len = temp_end - temp_start;
    if (temp_len >= temp_size)
    {
        printf("Buffer size is too small.\n");
        return;
    }

    // �����ֶ�ֵ��Ŀ�껺����
    strncpy((char *)temperature, temp_start, temp_len);
    temperature[temp_len] = '\0'; // ȷ���ַ�����'\0'��β
		
}


void extract_humidity(const char *json, u8 *humidity, size_t hum_size)
{
    const char *hum_key = "\"humidity_float\":\"";
    const char *hum_start;
    const char *hum_end;

    // ����"humidity_float"�ֶε�λ��
    hum_start = strstr(json, hum_key);
    if (hum_start == NULL)
    {
        printf("Humidity key not found.\n");
        return;
    }

    // �ƶ����ֶ�ֵ�Ŀ�ʼλ��
    hum_start += strlen(hum_key);

    // �����ֶ�ֵ�Ľ���λ��
    hum_end = strchr(hum_start, '"');
    if (hum_end == NULL)
    {
        printf("Humidity value end not found.\n");
        return;
    }

    // �����ֶ�ֵ�ĳ���
    size_t hum_len = hum_end - hum_start;
    if (hum_len >= hum_size)
    {
        printf("Buffer size is too small.\n");
        return;
    }

    // �����ֶ�ֵ��Ŀ�껺����
    strncpy((char *)humidity, hum_start, hum_len);
    humidity[hum_len] = '\0'; // ȷ���ַ�����'\0'��β
}


void displayWeatherInfoOnOLED(void)
{
		u8 weather[50];         // �洢weather�ֶ�ֵ�Ļ�����
  	u8 temperature[10]; // �洢temperature�ֶ�ֵ�Ļ�����
    u8 humidity[50];        // �洢humidity�ֶ�ֵ�Ļ�����
	
	
    unsigned char displayStr[100];
	  unsigned char displayStr2[100];
	  unsigned int hex_value = 0;
	  unsigned int hex_value1 = 0;
//	  unsigned int hex_value2 = 0;
	  // ��ȡweather
//	recv_buf  rxd3_str
    extract_weather(rxd3_str, weather, sizeof(weather));
    printf("Weather: %s\n", (const char *)weather);
	  size_t len = strlen((char *)weather);
     // ������ת��Ϊ16���Ʊ�ʾ���洢������
    for (size_t i = 0; i < len; i++) {
        hex_value = (hex_value << 8) | weather[i];
    }


    // ��ȡtemperature_float
    printf("%X\n", hex_value);
		
		
	
		extract_temperature_str(rxd3_str, temperature, sizeof(temperature));
		printf("Temperature: %s\n", (const char *)temperature);


    // ��ȡhumidity_float
    extract_humidity(rxd3_str, humidity, sizeof(humidity));
    printf("Humidity: %s\n", (const char *)humidity);
	 
//		// ��ȡǰ�����ֽڣ���һ�����ֵ�UTF-8���룩
//    for (size_t i = 0; i < 3 && i < len; i++) {
//        hex_value = (hex_value << 8) | (unsigned char)weather[i];
//    }
      hex_value = (unsigned char)weather[0];
		  hex_value1 = (unsigned char)weather[1];
      printf("%X\n", hex_value);
  // �жϵ�һ���ֽڲ���һ���Ƚ�
    if ((hex_value & 0xFFFFFF) == HEX_QING) {
        printf("The weather is: ��\n");
        OLED_ShowChineseString(0, 0, "��������:��");
    } else if ((hex_value & 0xFFFFFF) == HEX_DUOYUN) {
        printf("The weather is: ����\n");
        OLED_ShowChineseString(0, 0, "��������:����");
    } else if ((hex_value & 0xFFFFFF) == HEX_YIN) {
        printf("The weather is: ��\n");
        OLED_ShowChineseString(0, 0, "��������:��");
    }  else if ((hex_value1 & 0xFFFFFF) == HEX_ZHEN) {
        printf("The weather is: ��\n");
        OLED_ShowChineseString(0, 0, "��������:����");
    } else if ((hex_value1 & 0xFFFFFF) == HEX_LEI) {
        printf("The weather is: ��\n");
        OLED_ShowChineseString(0, 0, "��������:������");
    } 
		
			else {
        printf("Unknown weather condition.\n");
    }
		
		Usart_SendString(USART2,(char *)weather);
		Usart_SendString(USART2,(char *)temperature);
		Usart_SendString(USART2,(char *)humidity);
    // ���������¶ȡ�ʪ����Ϣƴ�ӳ�һ���ַ���
//		sprintf((char*)displayStr, "��������:%s �¶�:%s�� ʪ��:%s%%", weather, temperature, humidity);
    sprintf((char*)displayStr, "��������:%s", weather);
    sprintf((char*)displayStr2, "�����¶�:%s ��",temperature);
    OLED_ShowChineseString(0, 16, displayStr2);


}
