#ifndef __SIMCOM_800_H__
#define __SIMCOM_800_H__

#include "main.h"

#define ONENET_DEVICE_ID "39278086"																	//ONENET 设备ID
#define ONENET_API_KEY "scdn9nuLhZEWsNawNzH7F7=Nlew="								//API KEY

/*此组件依赖串口UART2*/
uint8_t SIM800C_SendCmd(char *cmd,char *check_data);
uint8_t SIM800C_InitCheck(void);
uint8_t SIM800C_SetNoteTextMode(void);
uint8_t SIM800C_SendNote(uint8_t *num,uint8_t *text,uint16_t len);
void SIM800C_NtpUpdate(void);
uint8_t SIM800C_GPRS_Init(void);
uint8_t SIM800C_Connect_TCP_Server(char *ipaddr,char *port);
uint8_t SIIM800C_TCP_SendData(uint8_t *data,uint32_t len);
 
void OneNet_HTTP_GPS_DataUpdate(char *data_point,double Longitude,double latitude);
void OneNet_HTTP_DataUpdate(char *data_point,uint32_t data);

#endif  //__SIMCOM_800_H__