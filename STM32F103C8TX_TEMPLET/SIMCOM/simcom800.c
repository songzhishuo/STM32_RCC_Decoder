#include "simcom800.h"
 
#include "stdio.h"
#include "string.h"
#include "usart.h"
/*
��������:��SIM800Cģ�鷢��ָ��
��������:
				char *cmd  ���͵�����
			  char *check_data ��ⷵ�ص�����
����ֵ: 0��ʾ�ɹ� 1��ʾʧ��
*/
uint8_t SIM800C_SendCmd(char *cmd,char *check_data)
{
   uint16_t i,j;
   for(i=0;i<5;i++) //���Ե��ܴ���
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
			HAL_UART_Transmit(&huart2, (uint8_t *)cmd, strlen(cmd), 0xFFFF);			//����ָ��
//      USART2_RX_FLAG=0;
//      USART2_RX_CNT=0;
//			memset(USART2_RX_BUFF,0,sizeof(USART2_RX_BUFF));
//			USART_X_SendString(USART2,cmd); //����ָ��
		 
		 
      for(j=0;j<500;j++) //�ȴ���ʱ��(ms��λ)
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
          HAL_Delay(20); //һ�ε�ʱ��
      }
   }
   return 1;
}
 
 
/*
����  ���ܣ�GSMģ���ʼ�����
��������ֵ��1��ʾģ����ʧ�ܣ�0��ʾ�ɹ�
*/
uint8_t SIM800C_InitCheck(void)
{
	  if(SIM800C_SendCmd("AT\r\n","OK"))return 1;
	  else printf("SIM800ģ������!\r\n");
	  
		if(SIM800C_SendCmd("ATE0\r\n","OK"))return 2;
	  else printf("����ģ�鲻���Գɹ�!\r\n");
	
		if(SIM800C_SendCmd("AT+CGMI\r\n","OK"))return 3;
		else printf("��ѯ���������Ƴɹ�!%s\r\n", uart2_rx_data.rx_buf);
	
		if(SIM800C_SendCmd("AT+CGMM\r\n","OK"))return 4;
		else printf("��ѯģ���ͺųɹ�!%s\r\n", uart2_rx_data.rx_buf);
		
	  HAL_Delay(1000);
		HAL_Delay(1000);
		if(SIM800C_SendCmd("AT+CNUM\r\n","+CNUM:"))
			return 5;
		else 
			printf("��ȡ��������ɹ�!%s\r\n", uart2_rx_data.rx_buf);
	  /* ���ظ�ʽ����:
		+CNUM: "","+8613086989413",145,7,4
		OK
		*/
		return 0;
}
 
/*
����  ���ܣ�GSMģ�����ģʽ����
��������ֵ��0��ʾģ�����óɹ�
*/
uint8_t SIM800C_SetNoteTextMode(void)
{
		if(SIM800C_SendCmd("AT+CSCS=\"GSM\"\r\n","OK"))return 1;// "GSM"�ַ���
		else printf("����GSM�ַ������óɹ�!\r\n");
	
	  if(SIM800C_SendCmd("AT+CMGF=1\r\n","OK"))return 2; //�ı�ģʽ
		else printf("�����ı�ģʽ���óɹ�!\r\n");
		return 0;
}
 
/*
�������ܣ����Ͷ���
����������
					num:�绰����
					text:��������
��������ֵ��0��ʾ���ͳɹ�
*/
uint8_t SIM800C_SendNote(uint8_t *num,uint8_t *text,uint16_t len)
{
		char data[50];
		char send_buf[2];
		sprintf(data,"AT+CMGS=\"%s\"\r\n",num);
		if(SIM800C_SendCmd(data,">"))return 1; //���÷��͵��ֻ���
		//USART_X_SendData(USART2,text,len);     //���Ͷ�������
		HAL_UART_Transmit(&huart2, (uint8_t *)text, len, 0xFFFF);			//����ָ��
	
		send_buf[0] = 0x1a;
		send_buf[1] = '\0';
	  if(SIM800C_SendCmd(send_buf,"+CMGS"))return 2; //���ͽ�������
		return 0;
}
 
 
/*
�������ܣ�NTP����ͬ��ʱ��
*/
void SIM800C_NtpUpdate(void)
{  
	SIM800C_SendCmd("AT+SAPBR=3,1,\"Contype\",\"GPRS\"\r\n","OK");//���ó��س���1
	SIM800C_SendCmd("AT+SAPBR=3,1,\"APN\",\"CMNET\"\r\n","OK");
	SIM800C_SendCmd("AT+SAPBR=1,1\r\n","OK");                     //����һ��GPRS������
	HAL_Delay(5);
	SIM800C_SendCmd("AT+CNTPCID=1\r\n","OK");                     //����CNTPʹ�õ�CID
	SIM800C_SendCmd("AT+CNTP=\"202.120.2.101\",32\r\n","OK");     //����NTP�������ͱ���ʱ��(32ʱ�� ʱ����׼ȷ)
	SIM800C_SendCmd("AT+CNTP\r\n","+CNTP: 1");                    //ͬ������ʱ��
//	printf("ͬ������ʱ��:%s\r\n",USART2_RX_BUFF);
	printf("ͬ������ʱ��:%s\r\n", uart2_rx_data.rx_buf);
}
 
 
/*
�������ܣ�GPRS����ͨ�ų�ʼ��
�� �� ֵ�� 0��ʾ�ɹ�
*/
uint8_t SIM800C_GPRS_Init(void)
{
	 SIM800C_SendCmd("AT+CIPCLOSE=1\r\n","CLOSE OK");	//�ر�����
	 SIM800C_SendCmd("AT+CIPSHUT\r\n","SHUT OK");		//�ر��ƶ����� 
	 if(SIM800C_SendCmd("AT+CGCLASS=\"B\"\r\n","OK"))return 1;				//����GPRS�ƶ�̨���ΪB,֧�ְ����������ݽ��� 
	 if(SIM800C_SendCmd("AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n","OK"))return 2;//����PDP������,��������Э��,��������Ϣ
	 if(SIM800C_SendCmd("AT+CGATT=1\r\n","OK"))return 3;					//����GPRSҵ��
	 if(SIM800C_SendCmd("AT+CIPCSGP=1,\"CMNET\"\r\n","OK"))return 4;	 	//����ΪGPRS����ģʽ
	 if(SIM800C_SendCmd("AT+CIPHEAD=1\r\n","OK"))return 5;	 				//���ý���������ʾIPͷ(�����ж�������Դ)
	 return 0;
}
 
/*
��������: ����TCP������
��������: 
				ipaddr:ip��ַ
				port:�˿� 
�� �� ֵ: 0��ʾ�ɹ�,����ֵ��ʾʧ��
*/
uint8_t SIM800C_Connect_TCP_Server(char *ipaddr,char *port)
{
	 char cmd_buff[100];
	 SIM800C_SendCmd("AT+CIPCLOSE=1\r\n","CLOSE OK");	//�ر�����
	 SIM800C_SendCmd("AT+CIPSHUT\r\n","SHUT OK");		//�ر��ƶ����� 
	 sprintf(cmd_buff,"AT+CIPSTART=\"TCP\",\"%s\",\"%s\"\r\n",ipaddr,port);
	 if(SIM800C_SendCmd(cmd_buff,"OK"))return 1;		//��������
	 return 0;
}
 
 
/*
��������: TCP�ͻ���ģʽ�·�������
�� �� ֵ�� 0��ʾ�ɹ�,����ֵ��ʾʧ��
*/
uint8_t SIIM800C_TCP_SendData(uint8_t *data,uint32_t len)
{
	 char send_buf[2];
	//׼����������
	 if(SIM800C_SendCmd("AT+CIPSEND\r\n",">")==0)
	 {
		  //��������
//			USART_X_SendData(USART2,data,len);
			HAL_UART_Transmit(&huart2, (uint8_t *)data, len, 0xFFFF);			//����ָ��
		  //���ͽ�������
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
��������: ����HTTPЭ�鷽ʽ��OneNet�������ϱ�����
����������
				char *data_point�����ݵ������
				u32 data ���ϴ�������
*/
char OneNet_HTTP_CMD[1024];
char OneNet_HTTP_BUFF[300];

 
void OneNet_HTTP_DataUpdate(char *data_point,uint32_t data)
{
		uint8_t ret = 0;
		//ƴ������
	  snprintf(OneNet_HTTP_BUFF,sizeof(OneNet_HTTP_BUFF),
			"{\"datastreams\":[{\"id\":\"%s\",\"datapoints\":[{\"value\":%d}]}]}",data_point,data);
 
		//ƴ������
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
			
			if(ret == 0)				//���ݽ������
			{
				break;
			}
			else
			{
				HAL_Delay(300);
				printf("���ӷ�������״̬:%d\r\n", ret);
			}
		}
		
		HAL_Delay(50);
		
		
		
		while(1)
		{
			ret = SIIM800C_TCP_SendData((uint8_t*) OneNet_HTTP_CMD, strlen(OneNet_HTTP_CMD));
			
			if(ret == 0)				//���ݽ������
			{
				break;
			}
			else
			{
				HAL_Delay(300);
				printf("���ݷ��͵�״̬:%d\r\n", ret);
			}
			
		}
}
 
/*
��������: ����HTTPЭ�鷽ʽ��OneNet�������ϱ�GPS��γ����Ϣ
����������
				char *data_point�����ݵ������
				double Longitude,double latitude���ϴ��ľ�γ������
*/
void OneNet_HTTP_GPS_DataUpdate(char *data_point,double Longitude,double latitude)
{
		//ƴ������
	  snprintf(OneNet_HTTP_BUFF,sizeof(OneNet_HTTP_BUFF),
			"{\"datastreams\":[{\"id\":\"%s\",\"datapoints\":[{\"value\":{\"lon\":%lf,\"lat\":%lf}}]}]}",
		   data_point,Longitude,latitude);
 
		//ƴ������
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
		
		printf("���ӷ�������״̬:%d\r\n", SIM800C_Connect_TCP_Server("183.230.40.33","80"));
		HAL_Delay(500);
		printf("���ݷ��͵�״̬:%d\r\n", SIIM800C_TCP_SendData((uint8_t* )OneNet_HTTP_CMD, strlen(OneNet_HTTP_CMD)));
}