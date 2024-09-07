#ifndef __ESP8266_H
#define __ESP8266_H

#include "systick.h"
#include "stm32f4xx.h"
#include "uart.h"
#include "mqtt.h"


#include <stdio.h>
#include <string.h>
#include <stdarg.h>


#define REV_OK		0	//������ɱ�־
#define REV_WAIT	1	//����δ��ɱ�־

//#define ESP8266_WIFI_INFO		"AT+CWJAP=\"123\",\"12345678\"\r\n" //wifi���˺ź�����
//#define ESP8266_WIFI_INFO		"AT+CWJAP=\"Mate40\",\"12341234\"\r\n" //wifi���˺ź�����
#define ESP8266_WIFI_INFO		"AT+CWJAP=\"kyrie\",\"51522zzwlwlbb\"\r\n" //wifi���˺ź�����
//#define ESP8266_SERVER "AT+CIPSTART=\"TCP\",\"mqtts.heclouds.com\",1883\r\n"  //(TCP/UDP),ģʽONENET Borker Address��Borker Port
#define ESP8266_SERVER "AT+CIPSTART=\"TCP\",\"iot-06z00ct63jjd1t2.mqtt.iothub.aliyuncs.com\",1883\r\n"  //(TCP/UDP)ģʽ��ALIYUN Borker Address��Borker Port
//#define ESP8266_SERVER "AT+CIPSTART=\"TCP\",\"192.168.10.5\",8080\r\n"  //(TCP/UDP)ģʽ��Borker Address��Borker Port



 
#define HEX_QING  0xE6  // "��"��UTF-8����
#define HEX_DUOYUN 0xE5  // "��"��UTF-8���루"����"����ͨ��������ֱȽϣ�
#define HEX_YIN   0xE9  // "��"��UTF-8����
#define HEX_ZHEN   0x98  // "��"��UTF-8����
#define HEX_LEI   0x9B // "��"��UTF-8����

_Bool ESP8266_WaitRecive(void);
void ESP8266_Init(void);
void ESP8266_memset_RecvBuff(void);
_Bool ESP8266_SendCmd(char *str, char* rev);
void ESP8266_ConnectServer(void);
void recv_data_control(char * data);
void MQTT_RX_DATE_DEAL(char *recv_buf);

_Bool ESP8266_SendWIFI(char *str, char* rev);

extern unsigned int hex_value;
extern   u8 weather[50];         // �洢weather�ֶ�ֵ�Ļ�����
extern   u8 temperature[10]; // �洢temperature�ֶ�ֵ�Ļ�����
extern   u8 humidity[50];        // �洢humidity�ֶ�ֵ�Ļ�����
void ESP8266_SendATCommand(char *cmd);
void use_AT(void);
void extract_weather(const char *json, u8 *weather, size_t weather_size);
void extract_temperature_str(const char *json, u8 *temperature, size_t temp_size);
void extract_humidity(const char *json, u8 *humidity, size_t hum_size);
void displayWeatherInfoOnOLED(void);
void restart_ESP8266(void);

	
#endif 
