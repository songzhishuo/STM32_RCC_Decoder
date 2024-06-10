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
*����8λ����/�����
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
*��ʼ��
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
*���ڸ��µ�ǰ��Ļ��ʾ
*******************************/
void VFD_show(void)
{
  VFD_CS0; //��ʼ����
  Delay_Us(1);    //��ʱ300ns
  write_8(0xe8);//��ַ�Ĵ�����ʼλ��
  VFD_CS1; //ֹͣ����
}

/******************************
*��ָ��λ�ô�ӡһ���ַ�(�û��Զ���,����CG-ROM�е�)
*x:0~11;chr:Ҫ��ʾ���ַ�����
*******************************/
void VFD_WriteOneChar(unsigned char x, unsigned char chr)
{
  VFD_CS0; //��ʼ����
  Delay_Us(1);     //��ʱ300ns
  write_8(0x20+x);//��ַ�Ĵ�����ʼλ��
  Delay_Us(1);
  write_8(chr+0x30);
  VFD_CS1; //ֹͣ����
 // VFD_show();
}

/******************************
*��ָ��λ�ô�ӡ�ַ���
*(��������Ӣ��,���,����)
*x:0~11;str:Ҫ��ʾ���ַ���
*******************************/
void VFD_WriteStr(unsigned char x, char *str)
{
  VFD_CS0; //��ʼ����
  Delay_Us(1);    //��ʱ300ns
  write_8(0x20+x);//��ַ�Ĵ�����ʼλ��
  while (*str)
  {
    write_8(*str); //ascii���Ӧ�ַ���ת��
    str++;
  }
  VFD_CS1; //ֹͣ����
  //VFD_show();
}

/******************************
*��ָ��λ�ô�ӡ�����ַ�
*(��������Ӣ��,���,����)
*x:0~11;str:Ҫ��ʾ���ַ���
*******************************/
void VFD_WriteOneStr(unsigned char x, unsigned char chr)
{
  VFD_CS0; //��ʼ����
  Delay_Us(1);    //��ʱ300ns
  write_8(0x20+x);//��ַ�Ĵ�����ʼλ��
  write_8(chr); //ascii���Ӧ�ַ���ת��
  VFD_CS1; //ֹͣ����
  //VFD_show();
}

/******************************
*������Ļ����
*******************************/
void VFD_light(u8 data)
{
    //����//
    VFD_CS0;
    write_8(0xe4);
    Delay_Us(1);
    write_8(data);//bright 0-255
    VFD_CS1;
    Delay_Us(1);
}
