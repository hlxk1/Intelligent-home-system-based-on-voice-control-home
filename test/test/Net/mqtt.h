#ifndef __MQTT_H
#define __MQTT_H

#include "esp8266.h"
#include "systick.h"
#include <stdio.h>
#include <string.h>
#include "uart.h"
#include "ADC.h"

#define BYTE0(dwTemp)       (*( char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))
	
/************************	ONENET服务器***************************************/
//#define MQTT_Client_ID  "DHT11" //设备名字
//#define MQTT_User_Name "G7NqVv6le8" //设备 ID4D2JGK3c4i 
//#define MQTT_Password  "version=2018-10-31&res=products%2FG7NqVv6le8%2Fdevices%2FDHT11&et=1729607740&method=md5&sign=VRwAjJjquF6I%2FyCFP%2FnnFQ%3D%3D" //MQTT连接ONENET的密码
//#define SubscribeTopic_Attribute "$sys/G7NqVv6le8/DHT11/thing/property/post/reply" //订阅的属性主题
//#define SubscribeTopic_Server "$sys/G7NqVv6le8/DHT11/thing/service/LED_CONTROL/invoke" //订阅的服务主题
//#define PublishTopic   "$sys/G7NqVv6le8/DHT11/thing/property/post" //发布的主题
//#define MQTTPUBLISH(temp, humi) "{\"id\":\"123\",\"params\":{\"temp\":{\"value\":%.2lf},\"humi\":{\"value\":%.2lf}}}",temp, humi //发送onejson数据

/************************	阿里云服务器***************************************/
#define MQTT_Client_ID  "k1o88dujxXB.DHT11|securemode=2,signmethod=hmacsha256,timestamp=1724338727820|" 
#define MQTT_User_Name  "DHT11&k1o88dujxXB" //设备ID
#define MQTT_Password  "55d3af5f989f0000dd62deea5e7e94af7826f40eddb0a9c6d56a7441f008a50a" //MQTT连接ONENET的密码
#define SubscribeTopic_Server_Reply "/sys/k1o88dujxXB/DHT11/thing/event/property/post_reply" //订阅的属性主题
#define SubscribeTopic_Server "/sys/k1o88dujxXB/DHT11/thing/service/property/set" 
#define PublishTopic   "/sys/k1o88dujxXB/DHT11/thing/event/property/post" //发布的主题
#define MQTTPUBLISH(temp, humi) "{\"method\":\"thing.service.property.set\",\"id\":\"1229117953\",\"params\":{\"CurrentHumidity\":%2lf,\"CurrentTemperature\":%2lf,\"LED\":1,\"GeoLocation\":{\"CoordinateSystem\":10,\"Latitude\":10,\"Longitude\":10,\"Altitude\":10}},\"version\":\"1.0.0\"}",humi,temp



void MQTT_Clear(void);
void MQTT_SendData(uint8_t* buf,uint16_t len);
void MQTT_SendHeart(void);
void MQTT_Init(void);
void MQTT_Connect(void);
void MQTT_Disconnect(void);
void MQTT_PublicTopic(float humi,float temp);
void MQTT_SubscribeTopic(void);
void MQTT_UNSubscribeTopic(void);

#endif

