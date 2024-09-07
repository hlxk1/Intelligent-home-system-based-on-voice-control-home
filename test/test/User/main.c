#include "stm32f4xx.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "systick.h"
#include "exti.h"
#include "tim.h"
#include "pwm.h"
#include "uart.h"
#include <stdio.h>
#include "iwdg.h" 
#include "wwdg.h" 
#include "rtc.h" 
#include "DHT11.h"
#include "IIC.h"
#include <stdio.h>
#include <string.h>
#include "oled.h"
#include "hardiic.h"
#include "esp8266.h"
#include "mqtt.h"
#include "DUOJI.h"
#include "GPIOO.h"
#include "ADC.h"
int main(void)
{
	KEY_config();
	LED_config();
	PWM_TIM3_CH1();
	ADC3_Config();
	struct DHT11_Complete_Data_TypeDef DHT11_Data;
	UART1_Config(115200);
	UART2_Config(115200);
	UART3_Config(115200);
	
	SysTick_Init();
	DHT11_Config();
	DCMI_D5_pinB6();
	DCMI_D7_pine6();
	oled_init();
	restart_ESP8266();
	Delay_ms(500);
	use_AT();
	displayWeatherInfoOnOLED();
	ESP8266_Init();
	while(1)
	{
		LED_pin();
		DUOJI_pin();
		ADC_LIGHT_OPEN_CLOSE();//光照空灯
		
		DHT11_Data = get_dht11_data();
		MQTT_PublicTopic(DHT11_Data.temp,DHT11_Data.humi);
		Delay_ms(500);
		printf("MQTT发布数据成功！！！\r\n");
		printf("temp:%0.2lf , humi:%0.2lf\r\n",DHT11_Data.temp,DHT11_Data.humi);
		printf("Send Data To SERVER:");
		printf("\r\n");
		GET_light(); // 得到light
	}
}


