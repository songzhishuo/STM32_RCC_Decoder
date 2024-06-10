/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : 0x49181f36
 * Version            : V0.2.0
 * Date               : 2023/02/19
 * Description        : Main program body.
 * SPDX-License-Identifier: Apache-2.0
 *******************************************************************************/


#include "debug.h"
#include "VFD_Driver/VFD_Driver.h"
#include "DS1302/DS1302.h"
#include "TIM/TIM.h"
#include "BPC/BPC.h"
#include "KEY/key.h"
#include "ADC/ADC.h"
#include "DS18B20/DS18B20.h"

#define KEYTIME 200

unsigned char ds1302_time[6];//������ʱ����
unsigned char wendu[3];//�¶�ʮλ,�¶ȸ�λ,�¶�С�����һλ
unsigned char vfd_display_buf[8]="<<<<<<<<";//VFD�Դ�/��������
unsigned char auto_or_hand_tag=0;//�Զ����ֶ�ģʽ��ǩ 0�Զ� 1�ֶ�
unsigned char light_tag;//���ȵ��ڱ�ǩ 0(A�Զ�),1,2,3 ������������ݹ��������Զ�����
unsigned char MOOD=0;
//0ʱ����� 1���ڽ��� 2�¶Ƚ���
//3�粨���ճɹ������������� 4�粨����ģʽ�������� 5�Զ����ֶ����Ƶ������� 6���ȵ�������

unsigned int TIM2_time_count;
unsigned int key_time;

unsigned long temp;

extern unsigned char BPC_calibration_MOOD;//BPCУ׼ģʽ 0�ϸ�У׼ 1����У׼
extern unsigned int  BPC_Con;         //BPC���ռ���
extern unsigned int ds18b20_time; //18b20ʱ�����

void TIM2_IQR(void);
void key_scan();

void get_time_buf();//��ȡʱ��
void get_day_buf();//��ȡ����
void get_temp_buf();

void VFD_write_time_buf();//mood0
void VFD_write_day_buf();//mood1
void VFD_write_temp_buf();//mood2

void bpc_count_ui_buf();//mood3
void bpc_mood_ui_buf();//mood4
void auto_or_hand_ui_buf();//mood5
void light_ui_buf();//mood6

void VFD_buf_updata();
void VFD_art_buf_show(unsigned char speed,unsigned char *BUF);


int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    Delay_Init();
    USART_Printf_Init(115200);
    printf("SystemClk:%d\r\n",SystemCoreClock);
    /*****************���������ų�ʼ��******************/
    DS1302_GPIO_INIT();
    BPC_GPIO_INIT();
//    KEY_GPIO_INIT();
//    CH32V_ADC_Init(ADC_PD4, ADC_SAMPLE_241);
    /**************��õ�һ���ϵ�ʱ��ʱ��**************/
//    get_time_buf();
//    get_day_buf();
//    /***ʹDS18B20����һ���¶Ȳɼ��Ա������ʼ����85��******/
//    get_ds18b20();
//    ds18b20_time=749;//ֱ��Ĭ���Ѿ������ת�� �����һ�ν�ȥ��ʾ0��
    /******************��ʱ��2��ʼ��*******************/
    TIM_attachInterrupt(TIM2,1000,TIM2_IQR);            //1ms
    TIM_Cmd(TIM2,ENABLE);
    /********************��Ļ��ʼ��********************/
//    VFD_init();
//    if(ADC_Read(ADC_PD4)<40)VFD_light(10);
//    else VFD_light(ADC_Read(ADC_PD4)/4);
    /*******************��Ļ��ʼ��ʾ*******************/
//    VFD_art_buf_show(125,vfd_display_buf);//��������
//    VFD_write_time_buf();//��ʱ��д�뻺��
//    VFD_art_buf_show(125,vfd_display_buf);//ʱ�䶯��
    while(1)
    {
        BPC_DataHandle();//����BPCʱ��
    }
}


void TIM2_IQR()
{
    BPC_RX();//������1ms��ʱ�� ���ݽ���˺��������жϸߵ�ƽ����ʱ��
    TIM2_time_count++;
    if (TIM2_time_count%500==0)
    {

    }



     if(TIM2_time_count>28000)TIM2_time_count=0;
}





