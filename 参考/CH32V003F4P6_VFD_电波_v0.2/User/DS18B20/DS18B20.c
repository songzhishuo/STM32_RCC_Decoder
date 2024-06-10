#include "DS18B20.h"

unsigned int ds18b20_time=0;

void DS18B20_GPIO_SET(unsigned char num)//1Ϊ��� 0Ϊ����
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    if(num)
    {
        GPIO_InitStructure.GPIO_Pin = DS18B20_PIN;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(DS18B20_GPIO, &GPIO_InitStructure);
    }
    else
    {
        GPIO_InitStructure.GPIO_Pin = DS18B20_PIN;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_Init(DS18B20_GPIO, &GPIO_InitStructure);
    }
}

//DS28B20 ���߸�λ
unsigned char ds18b20_reset()
{
    unsigned char date;

    DS18B20_GPIO_SET(1);
    DS18B20_OUT_1;
    Delay_Us(120);
    DS18B20_OUT_0;
    Delay_Us(400);
    DS18B20_OUT_1;
    Delay_Us(100);
    DS18B20_GPIO_SET(0);
    date=DS18B20_IN;
    Delay_Us(50);

    return date;
}

//18B20��1���ֽ�
unsigned char ds18b20_read_byte()
{
    unsigned char i;
    unsigned char dat = 0;

    for (i=0; i<8; i++)             //8λ������
    {
        dat >>= 1;
        DS18B20_GPIO_SET(1);
        DS18B20_OUT_0;                     //��ʼʱ��Ƭ
        Delay_Us(2);               //��ʱ�ȴ�
        DS18B20_OUT_1;                     //׼������
        DS18B20_GPIO_SET(0);                //������ʱ
        if (DS18B20_IN) dat |= 0x80;        //��ȡ����
        Delay_Us(60);             //�ȴ�ʱ��Ƭ����
    }

    return dat;
}

//18b20д1���ֽ�
void Write_DS18B20(unsigned char dat)
{
    unsigned char i;
    for(i=0;i<8;i++)
    {
        DS18B20_GPIO_SET(1);
        DS18B20_OUT_0;
        Delay_Us(2);
        if(dat&0x01)DS18B20_OUT_1;
        else DS18B20_OUT_0;
        Delay_Us(60);
        DS18B20_OUT_1;
        dat >>= 1;
    }
}
/*
�õ�DS18B20�¶ȼƵ���ֵ
ʹ��ʱӦ���������ж���һ���������洢�õ����¶���ֵ ����ֱ��������if�жϻ�switch

����:
1.
    unsigned long wendu;
    wendu=get_ds18b20(0);
2.
    unsigned long wendu;
    unsigned char yi,er,san,si,wu,liu;

    wendu=get_ds18b20(0);

    liu=wendu/100000;
    wu=wendu/10000%10;
    si=wendu/1000%10;
    san=wendu/100%10;
    er=wendu/10%10;
    yi=wendu%10;
    display(liu,wu+16,si,san,er,yi,34,34);
3.
    if(get_ds18b20(1)>19)   //����¶ȴ���19��
    {

    }
ע��:��Ҫʱ��     ?���ʱ�����?    �ڴ��ڼ�������������
num=0:������λ�� ***������Ҫlong�����洢***  ǰ��λΪ��������    ����λΪС������
num=1:������λ�� ��Ҫchar�ʹ洢   Ϊ��������
������ִ���(��ȡ�¶���ֵΪ0) ���ԶԿ����������ȫ�ϵ����� ��ӳ�����ʱ������ȫ�ϵ�����
*/
unsigned long get_ds18b20()
{
    unsigned char low=0,high=0;
    unsigned long temp=0;
    ds18b20_time++;
    if(ds18b20_time==1)
    {
        if(ds18b20_reset()==1)//��ʼ�¶�ת��
        {
            ds18b20_reset();
            Write_DS18B20(0xcc);
            Write_DS18B20(0x44);
        }
    }
    else if(ds18b20_time==750)//��ȡ�¶�Ԥ��
    {
        ds18b20_reset();
        Write_DS18B20(0xcc);
        Write_DS18B20(0xbe);
        return 254;
    }
    else if(ds18b20_time==751)//��ȡ�¶�
    {
        low=ds18b20_read_byte();
        high=ds18b20_read_byte();
        ds18b20_reset();
        temp=(high<<4)|(low>>4);
        low=low&0x0f;                       //��λ���¶�ת��
        temp=temp*10000+low*625;
        return temp;
    }
    else if(ds18b20_time>800)//�ȴ���λ
    {
        ds18b20_time=0;
    }
    return 255;
}
