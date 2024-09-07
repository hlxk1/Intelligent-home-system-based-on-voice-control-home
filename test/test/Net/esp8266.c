#include "esp8266.h"

//usart3发送和接收数组
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
 * @brief ESP8266初始化
 * 通过AT指令设置esp8266模式并连接云端
 *
 * @return 没有返回参数
 */
void ESP8266_Init(void)
{

	ESP8266_memset_RecvBuff();
	/*退出透传模式*/
	while(ESP8266_SendCmd("+++",""));
	Delay_ms(500);
	/*测试ESP8266是否能正常进行AT控制*/
	printf("AT \r\n");
	while(ESP8266_SendCmd("AT\r\n","OK"));
	Delay_ms(500);
	/*ESP8266进行重新启动*/
	
	printf("AT+RST \r\n");
	while(ESP8266_SendCmd("AT+RST\r\n",""));
	//while(ESP8266_SendCmd("AT+RESTORE\r\n",""));
	Delay_ms(500);
	/*清除数据回传显示输入命令*/
	//printf("ATE0\r\n");
	//while(ESP8266_SendCmd("ATE0\r\n","OK"));
	//Delay_ms(500);
	printf("ESP8266 Init OK\r\n");
	
	
	/*******************以下是连接wifi设备*******************************/
	/*设置ESP8266为STA模式（客户端模式）*/
	printf("AT+CWMODE_CUR\r\n");
	while(ESP8266_SendCmd("AT+CWMODE_CUR=1\r\n", "OK"))
	Delay_ms(3000);
	/*设置对应wifi的账号和密码*/
	printf("AT+CWJAP\r\n");
	while(ESP8266_SendCmd(ESP8266_WIFI_INFO, "OK"))
	Delay_ms(3000);
	

//	while(ESP8266_SendWIFI(ESP8266_WIFI_INFO, "OK"))
//	Delay_ms(3000);
	
	printf("ESP8266 连接WIFI成功！！！\r\n");
	
	/*****************连接云服务器******************************/
	ESP8266_ConnectServer();
	printf("ESP8266 连接云服务器成功！！！\r\n");
	
	/*********************登录操作******************************/
	do{
		ESP8266_memset_RecvBuff();
		MQTT_Connect();
		Delay_ms(500);
	/****************通过串口返回判断是否订阅成功******************/
		if(recv_buf[0] == 0x20 && recv_buf[1] == 0x02)
		{
			printf("登录云服务器的MQTT成功！！！\r\n");
			connect_ack = 0;
		}
	}while(connect_ack != 0);
	connect_ack = 1;

	/*****************订阅云服务操作******************************/
	do{
		ESP8266_memset_RecvBuff();
		MQTT_SubscribeTopic();
		Delay_ms(500);
		/****************通过串口返回判断是否订阅成功******************/
		if((recv_buf[0] == 0x90 && recv_buf[1] == 0x03 && recv_buf[2] == 0x00 && recv_buf[3] == 0x01 && recv_buf[4] == 0x00)||(recv_buf[0] == 0x90 && recv_buf[1] == 0x03 && recv_buf[2] == 0x00 && recv_buf[3] == 0x01 && recv_buf[4] == 0x01))
		{
			printf("订阅MQTT主题成功！！！\r\n");
			connect_ack = 0;
		}
	}while(connect_ack != 0);
	connect_ack = 1;
	/*********************取消订阅********************************/
//	MQTT_UNSubscribeTopic();
	/*****************断开云服务操作******************************/
//	MQTT_Disconnect();
//  printf("MQTT断开服务器成功！！！\r\n");
//	printf("\r\n");
}


/**
 * @brief 清除缓冲区数据内容
 * 
 *
 * @return 没有返回参数
 */
void ESP8266_memset_RecvBuff(void)
{
	//清空发送和接收数组
	memset(usart3_txbuf,0,sizeof(usart3_txbuf));
	memset(usart3_rxbuf,0,sizeof(usart3_rxbuf));
	
	memset(recv_buf, 0, sizeof(recv_buf));
	cnt1 = 0;
}


/**
 * @brief ESP8266发送命令
 * 通过串口向ESP8266传输控制命令并核对返回结果是否正确
 *
 * @return 成功返回0，失败返回1
 */
_Bool ESP8266_SendCmd(char *str, char* rev)
{
	unsigned char timeOut = 200;
	ESP8266_memset_RecvBuff();

	Usart_SendString(USART3,str);//向esp8266发送指令
	while(timeOut--) //超时设置
	{
		if( ESP8266_WaitRecive()==REV_OK)
		{
			if(strstr((const char *)recv_buf, rev) != NULL)	//如果放回结果正确
			{
				ESP8266_memset_RecvBuff();									//清空缓存
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

	Usart_SendString(USART3,str);//向esp8266发送指令
	Delay_ms(2000);
		if( ESP8266_WaitRecive()==REV_OK)
		{
			if(strstr((const char *)recv_buf, rev) != NULL)	//如果放回结果正确
			{
				ESP8266_memset_RecvBuff();									//清空缓存
				return 0;
			}
		}
	Delay_ms(1000);	
	
		return 1;
}

/**
 * @brief ESP8266打开透传模式
 * 
 *
 * @return 没有返回结果
 */
void ESP8266_OpenTransmission(void)
{
	//设置透传模式
		memset(usart3_rxbuf,0,sizeof(usart3_rxbuf));    
		while(ESP8266_SendCmd("AT+CIPMODE=1\r\n","OK"))
		Delay_ms(500);  
}


/**
 * @brief ESP8266退出透传模式
 * 
 *
 * @return 没有返回结果
 */
void ESP8266_ExitUnvarnishedTrans(void)
{
	while(ESP8266_SendCmd("+++",""))
	Delay_ms(500);
}



/**
 * @brief ESP8266连接云服务器
 * 
 *
 * @return 没有返回结果
 */
void ESP8266_ConnectServer(void)
{
	//连接服务器
	printf("AT+CIPSTART");
	while(ESP8266_SendCmd(ESP8266_SERVER,"CONNECT"))
	Delay_ms(500);

	//设置透传模式
	ESP8266_OpenTransmission();
	
	//开启发送状态
	printf("AT+CIPSEND"); 
	while(ESP8266_SendCmd("AT+CIPSEND\r\n",">"))
	Delay_ms(500);//开始处于透传发送状态

}

char rxd3_num;	   // 存储接收到的单个字节数据的数量
/**
 * @brief 串口3的中断回调函数
 * 接收ESP8266发送的消息打印到PC端，并对服务器下发数据进行处理
 *
 * @return 没有返回结果
 */
void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		//从寄存器中获取接收到的数据
		Rx3Data = USART_ReceiveData(USART3);	
		recv_buf[cnt1++] = USART3->DR;
		rxd3_str[rxd3_num++] = Rx3Data;
		//清楚接收中断标志位
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	}
	/*服务器发送数据时，进行处理*/
	MQTT_RX_DATE_DEAL(recv_buf);
	/* 回显，把接收到的单个字节数据发送到发送方*/
	USART_SendData(USART1, Rx3Data);

}


/**
 * @brief 等待接收到的数据
 * 
 *
 * @return 返回接收完成标志/未完成标志
 */
_Bool ESP8266_WaitRecive(void)
{
	if(cnt1 == 0) 							//如果接收计数为0 则说明没有处于接收数据中，所以直接跳出，结束函数
		return REV_WAIT;
	if(cnt1 == esp8266_cnt1Pre)				//如果上一次的值和这次相同，则说明接收完毕
	{
		cnt1 = 0;							//清0接收计数
		return REV_OK;								//返回接收完成标志
	}
	esp8266_cnt1Pre = cnt1;					//置为相同
	return REV_WAIT;								//返回接收未完成标志
}


/**
 * @brief 接收数据处理
 * 
 *
 * @return 没有返回参数
 */
void MQTT_RX_DATE_DEAL(char *buf)
{
	char *ptr;
	ptr = strstr(buf,"LED");
 // 使用strstr函数找到"LED"的起始位置
  ptr = strstr(buf, "\"LED\"");
  if (ptr != NULL) {
    // 找到了"LED"的起始位置，向后查找":"的位置
    ptr = strchr(ptr, ':');
    if (ptr != NULL) {
      // 向后查找数字的位置
      ptr++;
      while (*ptr == ' ') {
        ptr++;
      }
      // 判断数字是否为0或1
      if (*ptr == '0') {
        printf("LED的值为0\n");
				memset(recv_buf,0,sizeof(recv_buf));
//				GPIO_ResetBits(GPIOG, GPIO_Pin_14); //点亮led灯
      } else if (*ptr == '1') {
        printf("LED的值为1\n");
				memset(recv_buf,0,sizeof(recv_buf));
//				GPIO_SetBits(GPIOG, GPIO_Pin_14); //熄灭led灯
      } 
    }
  }
		ptr = NULL;
}

void restart_ESP8266(void) {
    printf("Restarting ESP8266...\n");

		/*********************取消订阅********************************/
	MQTT_UNSubscribeTopic();
	/*****************断开云服务操作******************************/
	MQTT_Disconnect();
  printf("MQTT断开服务器成功！！！\r\n");
	printf("\r\n");
	 
	Delay_ms(200);
	/*退出透传模式*/
	while(ESP8266_SendCmd("+++",""));
	Delay_ms(500);
	/*测试ESP8266是否能正常进行AT控制*/
	printf("AT \r\n");
	while(ESP8266_SendCmd("AT\r\n","OK"));
	Delay_ms(500);
	/*ESP8266进行重新启动*/
//	
//    // 发送 AT+RST 命令以重启 ESP8266
//    while (ESP8266_SendCmd("AT+RST\r\n", "ready")) {
//        printf("Failed to restart ESP8266, retrying...\n");
//    }

//    // 延迟等待 ESP8266 重启完成
//    Delay_ms(5000); // 5秒延迟以确保 ESP8266 重新启动

//    // 检查 ESP8266 是否已成功重启并准备就绪
//    while (ESP8266_SendCmd("AT\r\n", "OK")) {
//        printf("ESP8266 restart failed, retrying...\n");
//    }

//    printf("ESP8266 restarted successfully.\n");
}


void use_AT(void) {
    ESP8266_memset_RecvBuff(); // 清空接收缓冲区

    // 测试 AT 命令 
    while (ESP8266_SendCmd("AT\r\n", "OK")) {
        printf("AT command failed, retrying...\n");
    }

    // 设置 Wi-Fi 模式为 Station 模式  
    while (ESP8266_SendCmd("AT+CWMODE_DEF=1\r\n", "OK")) {
        printf("Failed to set Wi-Fi mode, retrying...\n");
    }
//#define ESP8266_WIFI_INFO		"AT+CWJAP=\"kyrie\",\"51522zzwlwlbb\"\r\n" //wifi的账号和密码
		//AT+CWJAP=\"Mate40\",\"12341234\"\r\n
    // 连接到 Wi-Fi
    while (ESP8266_SendCmd("AT+CWJAP=\"kyrie\",\"51522zzwlwlbb\"\r\n", "OK")) {
        printf("Failed to connect to Wi-Fi, retrying...\n");
        Delay_ms(2000);
    }

    // 自动连接设置  
    while (ESP8266_SendCmd("AT+CWAUTOCONN=1\r\n", "OK")) {
        printf("Failed to set Auto Connect, retrying...\n");
    }

    // 建立 TCP 连接
    while (ESP8266_SendCmd("AT+CIPSTART=\"TCP\",\"restapi.amap.com\",80\r\n", "CONNECT")) {
        printf("Failed to establish TCP connection, retrying...\n");
    }

    // 设置透传模式
    while (ESP8266_SendCmd("AT+CIPMODE=1\r\n", "OK")) {
        printf("Failed to set Transparent Transmission mode, retrying...\n");
    }

    // 开始发送数据
    while (ESP8266_SendCmd("AT+CIPSEND\r\n", ">")) {
        printf("Failed to start data transmission, retrying...\n");
    }

    // 发送 HTTP GET 请求
    ESP8266_SendCmd("GET https://restapi.amap.com/v3/weather/weatherInfo?key=4392aa8b297b5903497f166c1ef71e0c&city=210100\r\n", "");

    Delay_ms(20);

    // 退出透传模式
    printf("Exiting Transparent Transmission mode...\n");
    ESP8266_ExitUnvarnishedTrans();
    Delay_ms(500);

    // 再次测试 AT 命令以确保 ESP8266 仍然正常工作
    while (ESP8266_SendCmd("AT\r\n", "OK")) {
        printf("Final AT command failed, retrying...\n");
    }

    printf("AT command sequence completed successfully.\n");
		
//		memset(rxd3_str,0,sizeof(rxd3_str));
}



// 提取"weather"字段的值
void extract_weather(const char *json, u8 *weather, size_t weather_size)
{
    const char *weather_key = "\"weather\":\"";
    const char *weather_start;
    const char *weather_end;

    // 查找"weather"字段的位置
    weather_start = strstr(json, weather_key);
    if (weather_start == NULL)
    {
        printf("Weather key not found.\n");
        return;
    }

    // 移动到字段值的开始位置
    weather_start += strlen(weather_key);

    // 查找字段值的结束位置
    weather_end = strchr(weather_start, '"');
    if (weather_end == NULL)
    {
        printf("Weather value end not found.\n");
        return;
    }

    // 计算字段值的长度
    size_t weather_len = weather_end - weather_start;
    if (weather_len >= weather_size)
    {
        printf("Buffer size is too small.\n");
        return;
    }

    // 复制字段值到目标缓冲区
    strncpy((char *)weather, weather_start, weather_len);
    weather[weather_len] = '\0'; // 确保字符串以'\0'结尾
}



void extract_temperature_str(const char *json, u8 *temperature, size_t temp_size)
{
    const char *temp_key = "\"temperature\":\"";
    const char *temp_start;
    const char *temp_end;

    // 查找"temperature"字段的位置
    temp_start = strstr(json, temp_key);
    if (temp_start == NULL)
    {
        printf("Temperature key not found.\n");
        return;
    }

    // 移动到字段值的开始位置
    temp_start += strlen(temp_key);

    // 查找字段值的结束位置
    temp_end = strchr(temp_start, '"');
    if (temp_end == NULL)
    {
        printf("Temperature value end not found.\n");
        return;
    }

    // 计算字段值的长度
    size_t temp_len = temp_end - temp_start;
    if (temp_len >= temp_size)
    {
        printf("Buffer size is too small.\n");
        return;
    }

    // 复制字段值到目标缓冲区
    strncpy((char *)temperature, temp_start, temp_len);
    temperature[temp_len] = '\0'; // 确保字符串以'\0'结尾
		
}


void extract_humidity(const char *json, u8 *humidity, size_t hum_size)
{
    const char *hum_key = "\"humidity_float\":\"";
    const char *hum_start;
    const char *hum_end;

    // 查找"humidity_float"字段的位置
    hum_start = strstr(json, hum_key);
    if (hum_start == NULL)
    {
        printf("Humidity key not found.\n");
        return;
    }

    // 移动到字段值的开始位置
    hum_start += strlen(hum_key);

    // 查找字段值的结束位置
    hum_end = strchr(hum_start, '"');
    if (hum_end == NULL)
    {
        printf("Humidity value end not found.\n");
        return;
    }

    // 计算字段值的长度
    size_t hum_len = hum_end - hum_start;
    if (hum_len >= hum_size)
    {
        printf("Buffer size is too small.\n");
        return;
    }

    // 复制字段值到目标缓冲区
    strncpy((char *)humidity, hum_start, hum_len);
    humidity[hum_len] = '\0'; // 确保字符串以'\0'结尾
}


void displayWeatherInfoOnOLED(void)
{
		u8 weather[50];         // 存储weather字段值的缓冲区
  	u8 temperature[10]; // 存储temperature字段值的缓冲区
    u8 humidity[50];        // 存储humidity字段值的缓冲区
	
	
    unsigned char displayStr[100];
	  unsigned char displayStr2[100];
	  unsigned int hex_value = 0;
	  unsigned int hex_value1 = 0;
//	  unsigned int hex_value2 = 0;
	  // 提取weather
//	recv_buf  rxd3_str
    extract_weather(rxd3_str, weather, sizeof(weather));
    printf("Weather: %s\n", (const char *)weather);
	  size_t len = strlen((char *)weather);
     // 将汉字转换为16进制表示并存储到变量
    for (size_t i = 0; i < len; i++) {
        hex_value = (hex_value << 8) | weather[i];
    }


    // 提取temperature_float
    printf("%X\n", hex_value);
		
		
	
		extract_temperature_str(rxd3_str, temperature, sizeof(temperature));
		printf("Temperature: %s\n", (const char *)temperature);


    // 提取humidity_float
    extract_humidity(rxd3_str, humidity, sizeof(humidity));
    printf("Humidity: %s\n", (const char *)humidity);
	 
//		// 提取前三个字节（即一个汉字的UTF-8编码）
//    for (size_t i = 0; i < 3 && i < len; i++) {
//        hex_value = (hex_value << 8) | (unsigned char)weather[i];
//    }
      hex_value = (unsigned char)weather[0];
		  hex_value1 = (unsigned char)weather[1];
      printf("%X\n", hex_value);
  // 判断第一个字节并进一步比较
    if ((hex_value & 0xFFFFFF) == HEX_QING) {
        printf("The weather is: 晴\n");
        OLED_ShowChineseString(0, 0, "沈阳天气:晴");
    } else if ((hex_value & 0xFFFFFF) == HEX_DUOYUN) {
        printf("The weather is: 多云\n");
        OLED_ShowChineseString(0, 0, "沈阳天气:多云");
    } else if ((hex_value & 0xFFFFFF) == HEX_YIN) {
        printf("The weather is: 阴\n");
        OLED_ShowChineseString(0, 0, "沈阳天气:阴");
    }  else if ((hex_value1 & 0xFFFFFF) == HEX_ZHEN) {
        printf("The weather is: 阴\n");
        OLED_ShowChineseString(0, 0, "沈阳天气:阵雨");
    } else if ((hex_value1 & 0xFFFFFF) == HEX_LEI) {
        printf("The weather is: 阴\n");
        OLED_ShowChineseString(0, 0, "沈阳天气:雷阵雨");
    } 
		
			else {
        printf("Unknown weather condition.\n");
    }
		
		Usart_SendString(USART2,(char *)weather);
		Usart_SendString(USART2,(char *)temperature);
		Usart_SendString(USART2,(char *)humidity);
    // 将天气、温度、湿度信息拼接成一个字符串
//		sprintf((char*)displayStr, "沈阳天气:%s 温度:%s℃ 湿度:%s%%", weather, temperature, humidity);
    sprintf((char*)displayStr, "沈阳天气:%s", weather);
    sprintf((char*)displayStr2, "室外温度:%s ℃",temperature);
    OLED_ShowChineseString(0, 16, displayStr2);


}
