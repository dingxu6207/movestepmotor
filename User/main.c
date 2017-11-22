/**
  ******************************************************************************
  * @file    main.c
  * @author  dingxu
  * @version V1.0
  * @date    2017-11-14
  * @brief   WF-ESP8266 WiFi模块控制电机
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 iSO STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  *
  ******************************************************************************
  */ 
 
#include "stm32f10x.h"
#include "bsp_usart1.h"
#include "bsp_SysTick.h"
#include "bsp_esp8266.h"
#include "test.h"
#include "bsp_dht11.h"
#include "bsp_led.h"
#include "bsp_exti.h"
 

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main ( void )
{
	/* 初始化 */
    USARTx_Config ();                                                        //初始化串口1
	SysTick_Init ();                                                         //配置 SysTick 为 1ms 中断一次 
	ESP8266_Init ();                                                         //初始化WiFi模块使用的接口和外设
	
	printf ( "\r\n WF-ESP8266 WiFi模块测试 \r\n" );                          //打印测试例程提示信息

	LED_GPIO_Config();  

	EXTI_Key_Config();
	
    ESP8266_StaTcpClient_UnvarnishTest ();
 	
    while ( 1 );
		
}


/*********************************************END OF FILE**********************/
