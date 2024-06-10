#include "VFD_Driver.h"


void VFD_GPIO_INIT()
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_APB2PeriphClockCmd(VFD_ALL_PIN_IN_GPIO_C, ENABLE);

    GPIO_InitStructure.GPIO_Pin = VFD_DIN_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(VFD_DIN_GPIO, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = VFD_CLK_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(VFD_CLK_GPIO, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = VFD_CS_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(VFD_CS_GPIO, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = VFD_RST_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(VFD_RST_GPIO, &GPIO_InitStructure);
}


/******************************
*用于8位数据/命令传输
*******************************/
void write_8(u8 w_data)
{
    u8 temp;
    for(temp=0;temp<8;temp++)
    {
        VFD_CLK0;
        if((w_data&0x01) == 0x01)
        {
            VFD_DIN1;
        }
        else
        {
            VFD_DIN0;
        }
        w_data>>=1;
        Delay_Us(2);
        VFD_CLK1;
        Delay_Us(2);
    }
}


/******************************
*初始化
*******************************/
void VFD_init()
{
    VFD_GPIO_INIT();
    VFD_RST0;
    Delay_Ms(10);
    VFD_RST1;

    VFD_CS0;
    write_8(0xe0);
    Delay_Us(2);
    write_8(0x07);
    VFD_CS1;
    Delay_Us(2);

    VFD_CS0;
    write_8(0xe4);
    Delay_Us(2);
    write_8(0xff);
    VFD_CS1;
    Delay_Us(2);

}

/******************************
*用于更新当前屏幕显示
*******************************/
void VFD_show(void)
{
  VFD_CS0; //开始传输
  Delay_Us(1);    //延时300ns
  write_8(0xe8);//地址寄存器起始位置
  VFD_CS1; //停止传输
}

/******************************
*在指定位置打印一个字符(用户自定义,所有CG-ROM中的)
*x:0~11;chr:要显示的字符编码
*******************************/
void VFD_WriteOneChar(unsigned char x, unsigned char chr)
{
  VFD_CS0; //开始传输
  Delay_Us(1);     //延时300ns
  write_8(0x20+x);//地址寄存器起始位置
  Delay_Us(1);
  write_8(chr+0x30);
  VFD_CS1; //停止传输
 // VFD_show();
}

/******************************
*在指定位置打印字符串
*(仅适用于英文,标点,数字)
*x:0~11;str:要显示的字符串
*******************************/
void VFD_WriteStr(unsigned char x, char *str)
{
  VFD_CS0; //开始传输
  Delay_Us(1);    //延时300ns
  write_8(0x20+x);//地址寄存器起始位置
  while (*str)
  {
    write_8(*str); //ascii与对应字符表转换
    str++;
  }
  VFD_CS1; //停止传输
  //VFD_show();
}

/******************************
*在指定位置打印单个字符
*(仅适用于英文,标点,数字)
*x:0~11;str:要显示的字符串
*******************************/
void VFD_WriteOneStr(unsigned char x, unsigned char chr)
{
  VFD_CS0; //开始传输
  Delay_Us(1);    //延时300ns
  write_8(0x20+x);//地址寄存器起始位置
  write_8(chr); //ascii与对应字符表转换
  VFD_CS1; //停止传输
  //VFD_show();
}

/******************************
*更改屏幕亮度
*******************************/
void VFD_light(u8 data)
{
    //亮度//
    VFD_CS0;
    write_8(0xe4);
    Delay_Us(1);
    write_8(data);//bright 0-255
    VFD_CS1;
    Delay_Us(1);
}
