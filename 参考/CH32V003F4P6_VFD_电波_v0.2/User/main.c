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

unsigned char ds1302_time[6];//年月日时分秒
unsigned char wendu[3];//温度十位,温度个位,温度小数点后一位
unsigned char vfd_display_buf[8]="<<<<<<<<";//VFD显存/开机动画
unsigned char auto_or_hand_tag=0;//自动与手动模式标签 0自动 1手动
unsigned char light_tag;//亮度调节标签 0(A自动),1,2,3 三级亮度与根据光敏电阻自动调节
unsigned char MOOD=0;
//0时间界面 1日期界面 2温度界面
//3电波接收成功次数计数界面 4电波接收模式调整界面 5自动与手动控制调整界面 6亮度调整界面

unsigned int TIM2_time_count;
unsigned int key_time;

unsigned long temp;

extern unsigned char BPC_calibration_MOOD;//BPC校准模式 0严格校准 1宽松校准
extern unsigned int  BPC_Con;         //BPC接收计数
extern unsigned int ds18b20_time; //18b20时间计数

void TIM2_IQR(void);
void key_scan();

void get_time_buf();//获取时间
void get_day_buf();//获取日期
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
    /*****************各部分引脚初始化******************/
    DS1302_GPIO_INIT();
    BPC_GPIO_INIT();
//    KEY_GPIO_INIT();
//    CH32V_ADC_Init(ADC_PD4, ADC_SAMPLE_241);
    /**************获得第一次上电时的时间**************/
//    get_time_buf();
//    get_day_buf();
//    /***使DS18B20进行一次温度采集以避免读初始化的85度******/
//    get_ds18b20();
//    ds18b20_time=749;//直接默认已经完成了转化 避免第一次进去显示0度
    /******************定时器2初始化*******************/
    TIM_attachInterrupt(TIM2,1000,TIM2_IQR);            //1ms
    TIM_Cmd(TIM2,ENABLE);
    /********************屏幕初始化********************/
//    VFD_init();
//    if(ADC_Read(ADC_PD4)<40)VFD_light(10);
//    else VFD_light(ADC_Read(ADC_PD4)/4);
    /*******************屏幕开始显示*******************/
//    VFD_art_buf_show(125,vfd_display_buf);//开机动画
//    VFD_write_time_buf();//将时间写入缓存
//    VFD_art_buf_show(125,vfd_display_buf);//时间动画
    while(1)
    {
        BPC_DataHandle();//解析BPC时间
    }
}


void TIM2_IQR()
{
    BPC_RX();//必须是1ms定时器 根据进入此函数次数判断高电平持续时间
    TIM2_time_count++;
    if (TIM2_time_count%500==0)
    {

    }



     if(TIM2_time_count>28000)TIM2_time_count=0;
}





