/*
 * DS1302.c
 *
 *  Created on: 2023��2��4��
 *      Author: 12263
 */
#include "DS1302.h"


void DS1302_GPIO_INIT()
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_APB2PeriphClockCmd(DS1302_ALL_PIN_IN_GPIO_C, ENABLE);

    GPIO_InitStructure.GPIO_Pin = DS1302_SCK_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DS1302_SCK_GPIO, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = DS1302_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DS1302_SDA_GPIO, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = DS1302_RST_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DS1302_RST_GPIO, &GPIO_InitStructure);

    DS1302_SCK1; //SCL
    DS1302_SDA1; //SDA
    DS1302_RST0; //RST
}


/*
дds1302�Ĵ���              //��Ҫ���е���
num:д������
*/
void Write_Ds1302_Byte(unsigned  char w_data)
{
    unsigned char temp;
    for (temp=0;temp<8;temp++)
    {
        DS1302_SCK0;
        if((w_data&0x01) == 0x01)
        {
            DS1302_SDA1;
        }
        else
        {
            DS1302_SDA0;
        }
        w_data>>=1;
        DS1302_SCK1;
    }
}


/*
дds1302�Ĵ���              //��Ҫ���е���
num1:д�Ĵ�����ַ
num2:д������
*/
void Write_Ds1302( unsigned char address,unsigned char dat )
{
    DS1302_RST0;Delay_Us(5);
    DS1302_SCK0;Delay_Us(5);
    DS1302_RST1;Delay_Us(5);
    Write_Ds1302_Byte(address);
    Write_Ds1302_Byte((dat/10<<4)|(dat%10));
    DS1302_RST0;
}

/*
��ds1302�Ĵ���              //��Ҫ���е���
num:���Ĵ�����ַ
*/
unsigned char Read_Ds1302 ( unsigned char address )
{
    unsigned char i,temp=0x00,dat1,dat2;
    DS1302_RST0;
    Delay_Us(5);
    DS1302_SCK0;
    Delay_Us(5);
    DS1302_RST1;
    Delay_Us(5);
    Write_Ds1302_Byte(address);
    for (i=0;i<8;i++)
    {
        DS1302_SCK0;
        temp>>=1;
        if(DS1302_READ_SDA)temp|=0x80;
        DS1302_SCK1;
    }
    DS1302_RST0;
    Delay_Us(5);
    DS1302_SCK0;
    Delay_Us(5);
    DS1302_SCK1;
    Delay_Us(5);
    DS1302_SDA0;
    Delay_Us(5);
    DS1302_SDA1;
    Delay_Us(5);

    dat1=temp/16;
    dat2=temp%16;
    temp=dat1*10+dat2;

    return (temp);
}

/*
��ʼ��ds1302ʱ��
Ϊds1302���г�ʼʱ������
����:
1.
    ds1302_time_set(19,2,10,23,59,50,1);//����ds1302��ʼʱ��Ϊ 2019��2��10��23��59��50�� ����һ

num1:��
num2:��
num3:ʱ
num4:��
num5:��
num6:����
num7:��(��-2000)
*/
void ds1302_time_set(unsigned char num1,unsigned char num2,unsigned char num3,unsigned char num4,unsigned char num5,unsigned char num6,unsigned char num7)
{
    Write_Ds1302(0x8e,0x00);Delay_Us(100);
    Write_Ds1302(0x80,num6);Delay_Us(100);
    Write_Ds1302(0x82,num5);Delay_Us(100);
    Write_Ds1302(0x84,num4);Delay_Us(100);
    Write_Ds1302(0x86,num3);Delay_Us(100);
    Write_Ds1302(0x88,num2);Delay_Us(100);
    Write_Ds1302(0x8a,num7);Delay_Us(100);
    Write_Ds1302(0x8c,num1);Delay_Us(100);
    Write_Ds1302(0x8e,0x80);Delay_Us(100);
}

/*
����ֵΪ��λ���� ��235950 23��59��50�� 190210 19��2��10��
num0:����ʱ��
num1:��������
����
1.
        unsigned char yi,er,san=33,si,wu,liu=33,qi,ba;
        unsigned long time;
        time=get_ds1302_time(0);
        delay(500);
        yi=time/100000;
        er=time/10000%10;
        si=time/1000%10;
        wu=time/100%10;
        qi=time/10%10;
        ba=time%10;

ע��:������Ҫ������������(���ױ���ʱ����ϵ������ݴ���) ��Ҫ����ĵ��� 500-1000msһ�μ��� ����Ƚ�ռ����Դ
*/
unsigned long get_ds1302_time(unsigned char num)
{
    unsigned long dat;
    unsigned int temp;
    unsigned char add;
    if(num)add=0x87;
    else add=0x81;
    Write_Ds1302(0x8e,0x00);
    if(num)dat=Read_Ds1302(add+6);
    else dat=Read_Ds1302(add+4);
    dat*=10000;
    temp=Read_Ds1302(add+2);
    dat+=temp*=100;
    dat+=Read_Ds1302(add);
    Write_Ds1302(0x8e,0x80);

    return dat;
}

//��ȡ���� ����ֵΪ1-7
unsigned char get_ds1302_day(void)
{
    return Read_Ds1302(0x8B);
}
