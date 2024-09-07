#ifndef __ESP8266_H
#define __ESP8266_H

#include "systick.h"
#include "stm32f4xx.h"
#include "uart.h"
#include "mqtt.h"


#include <stdio.h>
#include <string.h>
#include <stdarg.h>


#define REV_OK		0	//接收完成标志
#define REV_WAIT	1	//接收未完成标志

//#define ESP8266_WIFI_INFO		"AT+CWJAP=\"123\",\"12345678\"\r\n" //wifi的账号和密码
//#define ESP8266_WIFI_INFO		"AT+CWJAP=\"Mate40\",\"12341234\"\r\n" //wifi的账号和密码
#define ESP8266_WIFI_INFO		"AT+CWJAP=\"kyrie\",\"51522zzwlwlbb\"\r\n" //wifi的账号和密码
//#define ESP8266_SERVER "AT+CIPSTART=\"TCP\",\"mqtts.heclouds.com\",1883\r\n"  //(TCP/UDP),模式ONENET Borker Address，Borker Port
#define ESP8266_SERVER "AT+CIPSTART=\"TCP\",\"iot-06z00ct63jjd1t2.mqtt.iothub.aliyuncs.com\",1883\r\n"  //(TCP/UDP)模式，ALIYUN Borker Address，Borker Port
//#define ESP8266_SERVER "AT+CIPSTART=\"TCP\",\"192.168.10.5\",8080\r\n"  //(TCP/UDP)模式，Borker Address，Borker Port



 
#define HEX_QING  0xE6  // "晴"的UTF-8编码
#define HEX_DUOYUN 0xE5  // "多"的UTF-8编码（"多云"可以通过多个汉字比较）
#define HEX_YIN   0xE9  // "阴"的UTF-8编码
#define HEX_ZHEN   0x98  // "阵"的UTF-8编码
#define HEX_LEI   0x9B // "雷"的UTF-8编码

_Bool ESP8266_WaitRecive(void);
void ESP8266_Init(void);
void ESP8266_memset_RecvBuff(void);
_Bool ESP8266_SendCmd(char *str, char* rev);
void ESP8266_ConnectServer(void);
void recv_data_control(char * data);
void MQTT_RX_DATE_DEAL(char *recv_buf);

_Bool ESP8266_SendWIFI(char *str, char* rev);

extern unsigned int hex_value;
extern   u8 weather[50];         // 存储weather字段值的缓冲区
extern   u8 temperature[10]; // 存储temperature字段值的缓冲区
extern   u8 humidity[50];        // 存储humidity字段值的缓冲区
void ESP8266_SendATCommand(char *cmd);
void use_AT(void);
void extract_weather(const char *json, u8 *weather, size_t weather_size);
void extract_temperature_str(const char *json, u8 *temperature, size_t temp_size);
void extract_humidity(const char *json, u8 *humidity, size_t hum_size);
void displayWeatherInfoOnOLED(void);
void restart_ESP8266(void);

	
#endif 
