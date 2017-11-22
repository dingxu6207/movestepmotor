#include "test.h"
#include "bsp_esp8266.h"
#include "bsp_SysTick.h"
#include <stdio.h>  
#include <string.h>  
#include <stdbool.h>
#include "bsp_dht11.h"
#include "Bsp_led.h"
#include "stm32f10x_it.h"

volatile uint8_t ucTcpClosedFlag = 0;

char cStr [ 1500 ] = { 0 };
u16 Angle16Time = 0;
u16 StepCounter = 0;
char DivideCounter = 0;
u8 StopFlag = 0;

/**
  * @brief  ESP8266 （Sta Tcp Client）透传
  * @param  无
  * @retval 无
  */
/* 正转 */
void Movestep(void)
{
    
	/* 开启电机 */
	GPIO_SetBits(GPIOA, GPIO_Pin_4);
	
	/* 00 */
	GPIO_ResetBits(GPIOA, GPIO_Pin_2);
	GPIO_ResetBits(GPIOA, GPIO_Pin_3);
	Delay_ms(100);
	if (1 == StopFlag)
		return;

	/* 01 */
	GPIO_ResetBits(GPIOA, GPIO_Pin_2);
	GPIO_SetBits(GPIOA, GPIO_Pin_3);
	Delay_ms(100);
	if (1 == StopFlag)
		return;

	/* 11 */
	GPIO_SetBits(GPIOA, GPIO_Pin_2);
	GPIO_SetBits(GPIOA, GPIO_Pin_3);
	Delay_ms(100);
	if (1 == StopFlag)
		return;

	/* 10 */
	GPIO_SetBits(GPIOA, GPIO_Pin_2);
	GPIO_ResetBits(GPIOA, GPIO_Pin_3);
	Delay_ms(100);
	if (1 == StopFlag)
		return;

	/* 关闭电机 */
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);
	return;
	
}

/* 反转 */
void BackMovestep( void )
{
    /* 开启电机 */
	GPIO_SetBits(GPIOA, GPIO_Pin_4);
	
	/* 10 */
	GPIO_SetBits(GPIOA, GPIO_Pin_2);
	GPIO_ResetBits(GPIOA, GPIO_Pin_3);
	Delay_ms(100);
	if (1 == StopFlag)
		return;
	
	/* 11 */
	GPIO_SetBits(GPIOA, GPIO_Pin_2);
	GPIO_SetBits(GPIOA, GPIO_Pin_3);
	Delay_ms(100);
	if (1 == StopFlag)
		return;

	/* 01 */
	GPIO_ResetBits(GPIOA, GPIO_Pin_2);
	GPIO_SetBits(GPIOA, GPIO_Pin_3);
	Delay_ms(100);
	if (1 == StopFlag)
		return;

    /* 00 */
	GPIO_ResetBits(GPIOA, GPIO_Pin_2);
	GPIO_ResetBits(GPIOA, GPIO_Pin_3);
	Delay_ms(100);
	if (1 == StopFlag)
		return;

	/* 关闭电机 */
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);
	return;
	
}

void ESP8266_StaTcpClient_UnvarnishTest ( void )
{
	uint8_t ucStatus;
	char cStr [ 100 ] = { 0 };
	char CmdMotor;
		
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);//关闭所有电机
	
	printf ( "\r\n正在配置 ESP8266 ......\r\n" );

	macESP8266_CH_ENABLE();
	
	ESP8266_AT_Test ();
	
	ESP8266_Net_Mode_Choose ( STA );

  	while ( ! ESP8266_JoinAP ( macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd ) );	
	
	ESP8266_Enable_MultipleId ( DISABLE );
	
	while ( !	ESP8266_Link_Server ( enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port, Single_ID_0 ) );
	
	while ( ! ESP8266_UnvarnishSend () );
	
	printf ( "\r\n配置 ESP8266 完毕\r\n" );
	macPC_Usart ( "%s", strEsp8266_Fram_Record .Data_RX_BUF );

	memset(strEsp8266_Fram_Record .Data_RX_BUF, 0, strEsp8266_Fram_Record .InfBit .FramLength);
	
	while ( 1)
	{		
		sprintf ( cStr, "StepCounter = %d\n", StepCounter);	    
		
		ESP8266_SendString ( ENABLE, cStr, 0, Single_ID_0 );               //发送数据
		
		Delay_ms ( 100 );
		
		CmdMotor = strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength - 1 ];
		
		if (('5' == CmdMotor) && (16 != StepCounter))
		{
			LED3(0);
			
			macPC_Usart ( "%c\n", CmdMotor );
			
			strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength - 1 ] = '\0';
		
			while ( DivideCounter < 10)    //一圈5
			{		
				Movestep();

				if (1 == StopFlag)
				{
					/* 关闭电机 */
					GPIO_ResetBits(GPIOA, GPIO_Pin_4);
					
					break;
				}
					
				DivideCounter++;
			}
			
			DivideCounter = 0;
			
			StepCounter++;		
		}

		
		if (('6' == CmdMotor) && (0 != StepCounter))
		{
			LED3(0);
				
			macPC_Usart ( "%c\n", CmdMotor );
				
			strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength - 1 ] = '\0';
			
			while ( DivideCounter < 10)	  //一圈5
			{		
				BackMovestep();	

				if (1 == StopFlag)
				{
					/* 关闭电机 */
					GPIO_ResetBits(GPIOA, GPIO_Pin_4);
					
					break;
				}
				
				DivideCounter++;
			}
				
			DivideCounter = 0;
				
			StepCounter--;		
		}

		if ( (StepCounter >= 16) || (1 == StopFlag))
		{
			DivideCounter = 0;

			StepCounter = 0;

			StopFlag = 0;
			
			LED3(1);
		}		
				
		if ( ucTcpClosedFlag )                                             //检测是否失去连接
		{
			ESP8266_ExitUnvarnishSend ();                                    //退出透传模式
			
			do ucStatus = ESP8266_Get_LinkStatus ();                         //获取连接状态
			while ( ! ucStatus );
			
			if ( ucStatus == 4 )                                             //确认失去连接后重连
			{
				printf ( "\r\n正在重连热点和服务器 ......\r\n" );
				
				while ( ! ESP8266_JoinAP ( macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd ) );
				
				while ( !	ESP8266_Link_Server ( enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port, Single_ID_0 ) );
				
				printf ( "\r\n重连热点和服务器成功\r\n" );

			}
			
			while ( ! ESP8266_UnvarnishSend () );		
			
		}
	}		
}
