#include "DS18B20.h"

unsigned int ds18b20_time=0;

void DS18B20_GPIO_SET(unsigned char num)//1为输出 0为输入
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

//DS28B20 总线复位
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

//18B20读1个字节
unsigned char ds18b20_read_byte()
{
    unsigned char i;
    unsigned char dat = 0;

    for (i=0; i<8; i++)             //8位计数器
    {
        dat >>= 1;
        DS18B20_GPIO_SET(1);
        DS18B20_OUT_0;                     //开始时间片
        Delay_Us(2);               //延时等待
        DS18B20_OUT_1;                     //准备接收
        DS18B20_GPIO_SET(0);                //接收延时
        if (DS18B20_IN) dat |= 0x80;        //读取数据
        Delay_Us(60);             //等待时间片结束
    }

    return dat;
}

//18b20写1个字节
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
得到DS18B20温度计的数值
使用时应在主函数中定义一个变量来存储得到的温度数值 或者直接运用在if判断或switch

用例:
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
    if(get_ds18b20(1)>19)   //如果温度大于19度
    {

    }
注意:需要时间     ?最短时间待测?    在此期间主函数不运行
num=0:返回六位数 ***至少需要long型来存储***  前两位为整数部分    后四位为小数部分
num=1:返回两位数 需要char型存储   为整数部分
如果出现错误(获取温度数值为0) 尝试对开发板进行完全断电重启 或加长采样时间再完全断电重启
*/
unsigned long get_ds18b20()
{
    unsigned char low=0,high=0;
    unsigned long temp=0;
    ds18b20_time++;
    if(ds18b20_time==1)
    {
        if(ds18b20_reset()==1)//开始温度转换
        {
            ds18b20_reset();
            Write_DS18B20(0xcc);
            Write_DS18B20(0x44);
        }
    }
    else if(ds18b20_time==750)//读取温度预备
    {
        ds18b20_reset();
        Write_DS18B20(0xcc);
        Write_DS18B20(0xbe);
        return 254;
    }
    else if(ds18b20_time==751)//读取温度
    {
        low=ds18b20_read_byte();
        high=ds18b20_read_byte();
        ds18b20_reset();
        temp=(high<<4)|(low>>4);
        low=low&0x0f;                       //六位数温度转换
        temp=temp*10000+low*625;
        return temp;
    }
    else if(ds18b20_time>800)//等待复位
    {
        ds18b20_time=0;
    }
    return 255;
}
