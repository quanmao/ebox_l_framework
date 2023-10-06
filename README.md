# E_BOX_L

#### 介绍

​		这是一个C++的STM32库，来源于ebox，链接地址：https://github.com/eboxmaker/eBox_Framework，因为有些理念和原作者不一致，所以自己开个坑，L即是个人姓的首字母，也代表HAL库的LL层。

1. 在STM32的HAL库文件LL的基础上封装一层类似于Arduino的API，使用MDK作为开发工具，支持调试。
2. 使得STM32也可以使用arduino的大部分驱动。驱动程序从github上下载后稍作修改就可以使用。
3. 快速实现底层驱动代码，减少STM32开发人员编写、调试器件驱动的工作量，提高驱动的重复利用率。
4. 对于库中未实现的部分，可以直接使用STM官方的HAL库。

#### 支持芯片

1. STM32F0系列
2. STM32F1系列



#### 可用外设

1.  GPIO，支持单IO，PORT，IO组操作
2.  I2C主, 硬件，软件
3.  SPI主，硬件，软件
4.  USART
5.  TIM，输入捕获，PWM输出，TIME
6.  RTC
7.  FLASH
8.  EXTI





