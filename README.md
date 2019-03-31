# UAV-Track-The-Black-Lines-Indoor
  * This is a source code of system of UAV-Track-The-Black-Lines-Indoor using C language based on STM32F4 MCU
  Including three parts:
    * Camera - using OV7670 to detect the black line and give the navigation information, eg. the relative x,y and yaw
    * Remote Control - a control system based on Bluetooth, which is used to control the UAV remotely to ensure safety
    * UAV System - including attitude algorithm、cascade PID control algorithm (Three cascade closed Loops)、reding sensor data and communicating with other parts
 * Building - Open the USER/LED.uvprojx using Keil Uvision5
 * This is also a library for the development based on STM32F4, which can drive the modules of UART,I2C,QEI and so on. It also supplies the driven code of MPU6050,NRF24L01...
