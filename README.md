# Intelligent home system based on voice control home
STM32F407ZE development

<a href="https://github.com/hlxk1/Intelligent-home-system-based-on-voice-control-home/blob/main/README_CN.md">中文简体</a>

****

### Project background

With the rapid development of Internet of Things (IoT) technology, smart home systems have become a part of modern life. By integrating various sensors, control modules and wireless communication technologies, smart home systems can achieve automatic management and remote control of the home environment. This project aims to develop a smart home system based on STM32F407ZE microcontroller, which uses voice recognition, WiFi connection and other technologies to provide users with convenient home control and information acquisition functions.

### Project content

** Voice recognition function: ** The system should support voice commands for obtaining weather, time, temperature and humidity and other information, and can control lights, curtains and alarm clocks and other devices by voice.

** Weather alerts: ** The system should be able to get current and future weather information through WiFi, and remind the user when there is a rainy day.

3. ** Automatic control: ** Support automatic curtain opening and closing control, based on sunrise and sunset time and indoor temperature, lighting conditions to adjust.

** Low power mode: ** The system automatically enters standby mode at night to reduce energy consumption.

5. ** User interface: ** OLED screen displays network time, weather information and system status.

** Module integration: ** Supports data interaction and control with the cloud through Bluetooth, WiFi and other ways.

### Project technical points

**STM32F407ZE microcontroller programming: ** Multi-threaded task management, peripheral control and communication protocol processing.

2. **ESP8266 WiFi module: ** Used to obtain network time and weather information to achieve remote control function.

3. ** Speech recognition module: ** Through the speech recognition chip or algorithm, to achieve the recognition and execution of user voice instructions.

**OLED display control: ** Displays real-time data, including time, weather and system status.

5. ** Sensor integration: ** Temperature and humidity sensors, light sensors, etc., for environmental data acquisition.

** Low power design: ** Optimize energy consumption at the hardware and software level to extend the standby time of the system.

** Bluetooth module: ** To achieve communication with mobile phones or other Bluetooth devices, for the control of curtains and other devices.

### ***\* Required modules: \****

**STM32F407ZE Development board **

**ESP8266 WiFi module **

3. ** Speech recognition module **

**OLED display **

5. ** Temperature and Humidity sensor **

6. ** Light sensor **

** Bluetooth module **

8. ** Buzzer **

** Power Management module **

** Steering gear module ** (for controlling lights, curtains, etc.)
