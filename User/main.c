/**
  ******************************************************************************
  * @file    main.c
  * @author  dingxu
  * @version V1.0
  * @date    2017-11-14
  * @brief   WF-ESP8266 WiFiģ����Ƶ��
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� iSO STM32 ������ 
  * ��̳    :http://www.firebbs.cn
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
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main ( void )
{
	/* ��ʼ�� */
    USARTx_Config ();                                                        //��ʼ������1
	SysTick_Init ();                                                         //���� SysTick Ϊ 1ms �ж�һ�� 
	ESP8266_Init ();                                                         //��ʼ��WiFiģ��ʹ�õĽӿں�����
	
	printf ( "\r\n WF-ESP8266 WiFiģ����� \r\n" );                          //��ӡ����������ʾ��Ϣ

	LED_GPIO_Config();  

	EXTI_Key_Config();
	
    ESP8266_StaTcpClient_UnvarnishTest ();
 	
    while ( 1 );
		
}


/*********************************************END OF FILE**********************/
