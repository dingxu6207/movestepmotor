#ifndef  __TEST_H
#define	 __TEST_H



#include "stm32f10x.h"



/********************************** �û���Ҫ���õĲ���**********************************/
#define      macUser_ESP8266_ApSsid                       "ynao"                //Ҫ���ӵ��ȵ������Netcoreasd
#define      macUser_ESP8266_ApPwd                        "ynao246135"           //Ҫ���ӵ��ȵ����Կdyc1102004097

#define      macUser_ESP8266_TcpServer_IP                 "192.168.80.89"      //Ҫ���ӵķ������� IP
#define      macUser_ESP8266_TcpServer_Port               "8080"               //Ҫ���ӵķ������Ķ˿�



/********************************** �ⲿȫ�ֱ��� ***************************************/
extern volatile uint8_t ucTcpClosedFlag;



/********************************** ���Ժ������� ***************************************/
void    ESP8266_StaTcpClient_UnvarnishTest  ( void );

void    Movestep( void );
void    BackMovestep( void );

#endif

