#include "simcom800.h"
 
#include "stdio.h"
#include "string.h"
#include "usart.h"
/*
函数功能:向SIM800C模块发送指令
函数参数:
				char *cmd  发送的命令
			  char *check_data 检测返回的数据
返回值: 0表示成功 1表示失败
*/
uint8_t SIM800C_SendCmd(char *cmd,char *check_data)
{
   uint16_t i,j;
   for(i=0;i<5;i++) //测试的总次数
   {
		 if(check_data != 0)
		 {
				uart2_rx_data.check_len = strlen(check_data);
		 }
		 else
		 {
				uart2_rx_data.check_len = 0;
		 }
			
			uart2_rx_data.rx_flag = 0;
			uart2_rx_data.rx_cnt = 0;
			memset(uart2_rx_data.rx_buf, 0, sizeof(uart2_rx_data.rx_buf));
			HAL_UART_Transmit(&huart2, (uint8_t *)cmd, strlen(cmd), 0xFFFF);			//发送指令
//      USART2_RX_FLAG=0;
//      USART2_RX_CNT=0;
//			memset(USART2_RX_BUFF,0,sizeof(USART2_RX_BUFF));
//			USART_X_SendString(USART2,cmd); //发送指令
		 
		 
      for(j=0;j<500;j++) //等待的时间(ms单位)
      {
//          if(USART2_RX_FLAG)
					if(uart2_rx_data.rx_flag)
          {
              //USART2_RX_BUFF[USART2_RX_CNT]='\0';
							uart2_rx_data.rx_buf[uart2_rx_data.rx_cnt] = '\0';
						
//              if(strstr((char*)USART2_RX_BUFF,check_data))
							if(strstr((char*)uart2_rx_data.rx_buf,check_data))
              {
                  return 0;
              }
              else 
							{
								//printf("rec:	%s\r\n", uart2_rx_data.rx_buf);
								break;
							}
          }
          HAL_Delay(20); //一次的时间
      }
   }
   return 1;
}
 
 
/*
函数  功能：GSM模块初始化检测
函数返回值：1表示模块检测失败，0表示成功
*/
uint8_t SIM800C_InitCheck(void)
{
	  if(SIM800C_SendCmd("AT\r\n","OK"))return 1;
	  else printf("SIM800模块正常!\r\n");
	  
		if(SIM800C_SendCmd("ATE0\r\n","OK"))return 2;
	  else printf("设置模块不回显成功!\r\n");
	
		if(SIM800C_SendCmd("AT+CGMI\r\n","OK"))return 3;
		else printf("查询制造商名称成功!%s\r\n", uart2_rx_data.rx_buf);
	
		if(SIM800C_SendCmd("AT+CGMM\r\n","OK"))return 4;
		else printf("查询模块型号成功!%s\r\n", uart2_rx_data.rx_buf);
		
	  HAL_Delay(1000);
		HAL_Delay(1000);
		if(SIM800C_SendCmd("AT+CNUM\r\n","+CNUM:"))
			return 5;
		else 
			printf("获取本机号码成功!%s\r\n", uart2_rx_data.rx_buf);
	  /* 返回格式如下:
		+CNUM: "","+8613086989413",145,7,4
		OK
		*/
		return 0;
}
 
/*
函数  功能：GSM模块短信模式设置
函数返回值：0表示模块设置成功
*/
uint8_t SIM800C_SetNoteTextMode(void)
{
		if(SIM800C_SendCmd("AT+CSCS=\"GSM\"\r\n","OK"))return 1;// "GSM"字符集
		else printf("短信GSM字符集设置成功!\r\n");
	
	  if(SIM800C_SendCmd("AT+CMGF=1\r\n","OK"))return 2; //文本模式
		else printf("短信文本模式设置成功!\r\n");
		return 0;
}
 
/*
函数功能：发送短信
函数参数：
					num:电话号码
					text:短信内容
函数返回值：0表示发送成功
*/
uint8_t SIM800C_SendNote(uint8_t *num,uint8_t *text,uint16_t len)
{
		char data[50];
		char send_buf[2];
		sprintf(data,"AT+CMGS=\"%s\"\r\n",num);
		if(SIM800C_SendCmd(data,">"))return 1; //设置发送的手机号
		//USART_X_SendData(USART2,text,len);     //发送短信内容
		HAL_UART_Transmit(&huart2, (uint8_t *)text, len, 0xFFFF);			//发送指令
	
		send_buf[0] = 0x1a;
		send_buf[1] = '\0';
	  if(SIM800C_SendCmd(send_buf,"+CMGS"))return 2; //发送结束符号
		return 0;
}
 
 
/*
函数功能：NTP网络同步时间
*/
void SIM800C_NtpUpdate(void)
{  
	SIM800C_SendCmd("AT+SAPBR=3,1,\"Contype\",\"GPRS\"\r\n","OK");//配置承载场景1
	SIM800C_SendCmd("AT+SAPBR=3,1,\"APN\",\"CMNET\"\r\n","OK");
	SIM800C_SendCmd("AT+SAPBR=1,1\r\n","OK");                     //激活一个GPRS上下文
	HAL_Delay(5);
	SIM800C_SendCmd("AT+CNTPCID=1\r\n","OK");                     //设置CNTP使用的CID
	SIM800C_SendCmd("AT+CNTP=\"202.120.2.101\",32\r\n","OK");     //设置NTP服务器和本地时区(32时区 时间最准确)
	SIM800C_SendCmd("AT+CNTP\r\n","+CNTP: 1");                    //同步网络时间
//	printf("同步网络时间:%s\r\n",USART2_RX_BUFF);
	printf("同步网络时间:%s\r\n", uart2_rx_data.rx_buf);
}
 
 
/*
函数功能：GPRS数据通信初始化
返 回 值： 0表示成功
*/
uint8_t SIM800C_GPRS_Init(void)
{
	 SIM800C_SendCmd("AT+CIPCLOSE=1\r\n","CLOSE OK");	//关闭连接
	 SIM800C_SendCmd("AT+CIPSHUT\r\n","SHUT OK");		//关闭移动场景 
	 if(SIM800C_SendCmd("AT+CGCLASS=\"B\"\r\n","OK"))return 1;				//设置GPRS移动台类别为B,支持包交换和数据交换 
	 if(SIM800C_SendCmd("AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n","OK"))return 2;//设置PDP上下文,互联网接协议,接入点等信息
	 if(SIM800C_SendCmd("AT+CGATT=1\r\n","OK"))return 3;					//附着GPRS业务
	 if(SIM800C_SendCmd("AT+CIPCSGP=1,\"CMNET\"\r\n","OK"))return 4;	 	//设置为GPRS连接模式
	 if(SIM800C_SendCmd("AT+CIPHEAD=1\r\n","OK"))return 5;	 				//设置接收数据显示IP头(方便判断数据来源)
	 return 0;
}
 
/*
函数功能: 连接TCP服务器
函数参数: 
				ipaddr:ip地址
				port:端口 
返 回 值: 0表示成功,其他值表示失败
*/
uint8_t SIM800C_Connect_TCP_Server(char *ipaddr,char *port)
{
	 char cmd_buff[100];
	 SIM800C_SendCmd("AT+CIPCLOSE=1\r\n","CLOSE OK");	//关闭连接
	 SIM800C_SendCmd("AT+CIPSHUT\r\n","SHUT OK");		//关闭移动场景 
	 sprintf(cmd_buff,"AT+CIPSTART=\"TCP\",\"%s\",\"%s\"\r\n",ipaddr,port);
	 if(SIM800C_SendCmd(cmd_buff,"OK"))return 1;		//发起连接
	 return 0;
}
 
 
/*
函数功能: TCP客户端模式下发送数据
返 回 值： 0表示成功,其他值表示失败
*/
uint8_t SIIM800C_TCP_SendData(uint8_t *data,uint32_t len)
{
	 char send_buf[2];
	//准备发送数据
	 if(SIM800C_SendCmd("AT+CIPSEND\r\n",">")==0)
	 {
		  //发送数据
//			USART_X_SendData(USART2,data,len);
			HAL_UART_Transmit(&huart2, (uint8_t *)data, len, 0xFFFF);			//发送指令
		  //发送结束符号
		  HAL_Delay(50);
		  send_buf[0] = 0x1a;
		  send_buf[1] = '\0';
		  if(SIM800C_SendCmd(send_buf,"SEND OK"))
				return 2;
		  else 	
				return 0;
	 }
	 return 1;
}
 
/*
函数功能: 采用HTTP协议方式向OneNet服务器上报数据
函数参数：
				char *data_point：数据点的名称
				u32 data ：上传的数据
*/
char OneNet_HTTP_CMD[1024];
char OneNet_HTTP_BUFF[300];

 
void OneNet_HTTP_DataUpdate(char *data_point,uint32_t data)
{
		uint8_t ret = 0;
		//拼接数据
	  snprintf(OneNet_HTTP_BUFF,sizeof(OneNet_HTTP_BUFF),
			"{\"datastreams\":[{\"id\":\"%s\",\"datapoints\":[{\"value\":%d}]}]}",data_point,data);
 
		//拼接数据
		snprintf(OneNet_HTTP_CMD,sizeof(OneNet_HTTP_CMD),
			"POST /devices/%s/datapoints HTTP/1.1\r\n" \
			"api-key:%s\r\n" \
			"Host:api.heclouds.com\r\n" \
			"Connection:close\r\n" \
			"Content-Length:%d\r\n" \
			"\r\n" \
			"%s",
	    ONENET_DEVICE_ID,ONENET_API_KEY,strlen(OneNet_HTTP_BUFF),OneNet_HTTP_BUFF
		);
		printf("OneNet_HTTP_CMD=%s\r\n", OneNet_HTTP_CMD);
		
		while(1)
		{
			ret = SIM800C_Connect_TCP_Server("183.230.40.33","80");
			
			if(ret == 0)				//数据接收完成
			{
				break;
			}
			else
			{
				HAL_Delay(300);
				printf("连接服务器的状态:%d\r\n", ret);
			}
		}
		
		HAL_Delay(50);
		
		
		
		while(1)
		{
			ret = SIIM800C_TCP_SendData((uint8_t*) OneNet_HTTP_CMD, strlen(OneNet_HTTP_CMD));
			
			if(ret == 0)				//数据接收完成
			{
				break;
			}
			else
			{
				HAL_Delay(300);
				printf("数据发送的状态:%d\r\n", ret);
			}
			
		}
}
 
/*
函数功能: 采用HTTP协议方式向OneNet服务器上报GPS经纬度信息
函数参数：
				char *data_point：数据点的名称
				double Longitude,double latitude：上传的经纬度数据
*/
void OneNet_HTTP_GPS_DataUpdate(char *data_point,double Longitude,double latitude)
{
		//拼接数据
	  snprintf(OneNet_HTTP_BUFF,sizeof(OneNet_HTTP_BUFF),
			"{\"datastreams\":[{\"id\":\"%s\",\"datapoints\":[{\"value\":{\"lon\":%lf,\"lat\":%lf}}]}]}",
		   data_point,Longitude,latitude);
 
		//拼接数据
		snprintf(OneNet_HTTP_CMD,sizeof(OneNet_HTTP_CMD),
			"POST /devices/%s/datapoints HTTP/1.1\r\n" \
			"api-key:%s\r\n" \
			"Host:api.heclouds.com\r\n" \
			"Connection:close\r\n" \
			"Content-Length:%d\r\n" \
			"\r\n" \
			"%s",
	    ONENET_DEVICE_ID,ONENET_API_KEY,strlen(OneNet_HTTP_BUFF),OneNet_HTTP_BUFF
		);
		printf("GPS_OneNet_HTTP_CMD=%s\r\n",OneNet_HTTP_CMD);
		
		printf("连接服务器的状态:%d\r\n", SIM800C_Connect_TCP_Server("183.230.40.33","80"));
		HAL_Delay(500);
		printf("数据发送的状态:%d\r\n", SIIM800C_TCP_SendData((uint8_t* )OneNet_HTTP_CMD, strlen(OneNet_HTTP_CMD)));
}