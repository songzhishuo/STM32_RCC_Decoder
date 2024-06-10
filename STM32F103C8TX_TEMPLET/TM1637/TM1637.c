#include "TM1637.h"
#include "main.h"




//static inline void delay_ms(uint32_t delay)
//{
//	HAL_Delay(delay);
//}

#define CPU_FREQUENCY_MHZ 72				// CPU��Ƶ������ʵ�ʽ����޸�
static void delay_us(uint32_t delay)
{
	int last, curr, val;
	int temp;

	while (delay != 0)
	{
		temp = delay > 900 ? 900 : delay;
		last = SysTick->VAL;
		curr = last - CPU_FREQUENCY_MHZ * temp;
		if (curr >= 0)
		{
			do
			{
				val = SysTick->VAL;
			}
			while ((val < last) && (val >= curr));
		}
		else
		{
			curr += CPU_FREQUENCY_MHZ * 1000;
			do
			{
				val = SysTick->VAL;
			}
			while ((val <= last) || (val > curr));
		}
		delay -= temp;
	}
} 

unsigned char tab[] =
{
   0x3F,/*0*/
   0x06,/*1*/
   0x5B,/*2*/
   0x4F,/*3*/
   0x66,/*4*/
   0x6D,/*5*/
   0x7D,/*6*/
   0x07,/*7*/
   0x7F,/*8*/
   0x6F,/*9*/
   0x77,/*10 A*/
   0x7C,/*11 b*/
   0x58,/*12 c*/
   0x5E,/*13 d*/
   0x79,/*14 E*/
   0x71,/*15 F*/
   0x76,/*16 H*/
   0x38,/*17 L*/
   0x54,/*18 n*/
   0x73,/*19 P*/
   0x3E,/*20 U*/
   0x00,/*21 ����*/
};
// 
//// ���λ����Ϊ1ʱ��ʾ ������ϵ�":" ����
// char disp_num[6] = {0x3F, 0x06 | 0x80, 0x5B, 0x4F, 0x66, 0x6D};			//���6�������Ҫ��ʾ������

////�˿ڳ�ʼ��
//void TM1637_Init( void )
//{
//    GPIO_InitTypeDef GPIO_InitStruct = {0};

//    GPIO_InitStruct.Pin = TM1637_CLK_IO_PIN;
//    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//    HAL_GPIO_Init( TM1637_CLK_IO_PORT, &GPIO_InitStruct );

//    GPIO_InitStruct.Pin = TM1637_DIO_IO_PIN;
//    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//    HAL_GPIO_Init( TM1637_DIO_IO_PORT, &GPIO_InitStruct );

//}
// 
////��ʼλ CLKΪ�ߵ�ƽʱ��DIO�ɸ߱��
//static void TM1637_Start( void )
//{
//    TM1637_DIO_OUT();
//    // TM1637_CLK = 1;
//    HAL_GPIO_WritePin(TM1637_CLK_IO_PORT, TM1637_CLK_IO_PIN, 1);
//    //TM1637_DIO = 1;
//    HAL_GPIO_WritePin(TM1637_DIO_IO_PORT, TM1637_DIO_IO_PIN, 1);
//    delay_us( 2 );
//    // TM1637_DIO = 0;
//    HAL_GPIO_WritePin(TM1637_DIO_IO_PORT, TM1637_DIO_IO_PIN, 0);
//}
// 
////�ȴ�Ӧ�� ����������ȷʱ���ڵڰ˸�ʱ���½��أ�оƬ�ڲ������һ��ACK�źţ���DIO�ܽ����ͣ��ڵھŸ�ʱ�ӽ���֮���ͷ�DIO���ߡ�
//static void TM1637_Ack( void )
//{
//    TM1637_DIO_IN();
//    // TM1637_CLK = 0;
//    HAL_GPIO_WritePin(TM1637_CLK_IO_PORT, TM1637_CLK_IO_PIN, 0);
//    delay_us( 5 );													//�ڵڰ˸�ʱ���½���֮����ʱ 5us����ʼ�ж� ACK �ź�
//    while( HAL_GPIO_ReadPin(TM1637_DIO_IO_PORT, TM1637_DIO_IO_PIN) );								//�ȴ�Ӧ��λ  ��һ�д���Ҳ���Բ�Ҫ ��Ӱ��ʵ��ʹ��Ч�� ��ʹ����������ʱ����Ҫ���������룬�������ͻῨ�����
//    // TM1637_CLK = 1;
//    HAL_GPIO_WritePin(TM1637_CLK_IO_PORT, TM1637_CLK_IO_PIN, 1);
//    delay_us( 2 );
//    // TM1637_CLK = 0;
//    HAL_GPIO_WritePin(TM1637_CLK_IO_PORT, TM1637_CLK_IO_PIN, 0);
//}
// 
////ֹͣλ CLKΪ�ߵ�ƽʱ��DIO�ɵͱ��
//static void TM1637_Stop( void )
//{
//    TM1637_DIO_OUT();
//    // TM1637_CLK = 0;
//    HAL_GPIO_WritePin(TM1637_CLK_IO_PORT, TM1637_CLK_IO_PIN, 0);
//    delay_us( 2 );
//    // TM1637_DIO = 0;
//    HAL_GPIO_WritePin(TM1637_DIO_IO_PORT, TM1637_DIO_IO_PIN, 0);
//    delay_us( 2 );
//    // TM1637_CLK = 1;
//    HAL_GPIO_WritePin(TM1637_CLK_IO_PORT, TM1637_CLK_IO_PIN, 1);
//    delay_us( 2 );
//    // TM1637_DIO = 1;
//    HAL_GPIO_WritePin(TM1637_DIO_IO_PORT, TM1637_DIO_IO_PIN, 1);
//}
////����������CLK�ĵ͵�ƽ�仯����CLK�ĸߵ�ƽ�����䡣
////ÿ����һ���ֽڣ�оƬ�ڲ��ڵڰ˸�ʱ���½��ز���һ��ACK
//// дһ���ֽ�
//static void TM1637_WriteByte( unsigned char oneByte )
//{
//    unsigned char i;
//    TM1637_DIO_OUT();
//    for( i = 0; i < 8; i++ )
//    {
//        // TM1637_CLK = 0;
//        HAL_GPIO_WritePin(TM1637_CLK_IO_PORT, TM1637_CLK_IO_PIN, 0);
//        if( oneByte & 0x01 )									//��λ��ǰ
//        {
//            // TM1637_DIO = 1;
//            HAL_GPIO_WritePin(TM1637_DIO_IO_PORT, TM1637_DIO_IO_PIN, 1);
//        }
//        else
//        {
//            // TM1637_DIO = 0;
//            HAL_GPIO_WritePin(TM1637_DIO_IO_PORT, TM1637_DIO_IO_PIN, 0);
//        }
//        delay_us( 3 );
//        oneByte = oneByte >> 1;
//        // TM1637_CLK = 1;
//        HAL_GPIO_WritePin(TM1637_CLK_IO_PORT, TM1637_CLK_IO_PIN, 1);
//        delay_us( 3 );
//    }
//}
// 
////д��ʾ�Ĵ���  ��ַ����
//void TM1637_Display_INC( void )
//{
//    unsigned char i;
//    TM1637_Start();
//    TM1637_WriteByte( 0x40 );									//д���ݵ���ʾ�Ĵ��� 40H ��ַ�Զ���1 ģʽ,44H �̶���ַģʽ,����������Լ�1ģʽ
//    TM1637_Ack();
//    TM1637_Stop();
//    TM1637_Start();
//    TM1637_WriteByte( 0xC0 );									//��ַ�������� ��ʾ��ַ 00H
//    TM1637_Ack();
// 
//    for( i = 0; i < 6; i++ )									//��ַ�Լӣ�����ÿ�ζ�д��ַ
//    {
//        TM1637_WriteByte( disp_num[i] );			//��������   disp_num[]�д洢6�������Ҫ��ʾ������
//        TM1637_Ack();
//    }
//    TM1637_Stop();
//#if 0
//    TM1637_Start();
//    TM1637_WriteByte( 0x88 | 0x07 );					//����ʾ���������-----���������ȿ���0---7  ������14/16
//    TM1637_Ack();
//    TM1637_Stop();
//#endif
//}
// 
////д��ʾ�Ĵ���  ��ַ������
//// add ����ܵĵ�ַ 0--5
//// value Ҫ��ʾ������
//void TM1637_Display_NoINC(uint8_t add, uint8_t value)
//{
//    uint8_t i;
//    TM1637_Start();
//    TM1637_WriteByte( 0x44 );								//д���ݵ���ʾ�Ĵ��� 40H ��ַ�Զ���1 ģʽ,44H �̶���ַģʽ,����������Լ�1ģʽ
//    TM1637_Ack();
//    TM1637_Stop();
// 
//    TM1637_Start();
//    TM1637_WriteByte( 0xC0 | add );					//��ַ�������� ��ʾ��ַ C0H---C5H
//    TM1637_Ack();
// 
//    TM1637_WriteByte( value );			 				//��������   value�洢Ҫ��ʾ������
//    TM1637_Ack();
//    TM1637_Stop();
//#if 0
//    TM1637_Start();
//    TM1637_WriteByte( 0x88 | 0x07 );							//����ʾ���������-----���������ȿ���0---7  ������14/16
//    TM1637_Ack();
//    TM1637_Stop();
//#endif
//}
// 
//// level : �������ȵȼ�  0---7
//void TM1637_SetBrightness(unsigned char level)
//{
//    TM1637_Start();
//    TM1637_WriteByte( 0x88 | level );							//����ʾ���������-----���������ȿ���0---7  ������14/16
//    TM1637_Ack();
//    TM1637_Stop();
//}

////������  ������ʱ��ʱ��Ƶ��ӦС�� 250K���ȶ���λ�������λ��
//unsigned char TM1637_ScanKey(void)
//{
//    unsigned char reKey, i;
//    TM1637_Start();
//    TM1637_WriteByte( 0x42 );						//����ɨ����
//    TM1637_Ack();
//    // TM1637_DIO = 1;											//�ڶ�����֮ǰ����������
//    HAL_GPIO_WritePin(TM1637_DIO_IO_PORT, TM1637_DIO_IO_PIN, 1);
//    TM1637_DIO_IN();
//    for( i = 0; i < 8; i++ )						//�ӵ�λ��ʼ��
//    {
//        // TM1637_CLK = 0;
//        HAL_GPIO_WritePin(TM1637_CLK_IO_PORT, TM1637_CLK_IO_PIN, 0);
//        reKey = reKey >> 1;
//        delay_us( 30 );
//        // TM1637_CLK = 1;
//        HAL_GPIO_WritePin(TM1637_CLK_IO_PORT, TM1637_CLK_IO_PIN, 1);
//        if( HAL_GPIO_ReadPin(TM1637_DIO_IO_PORT, TM1637_DIO_IO_PIN) )
//        {
//            reKey = reKey | 0x80;
//        }
//        else
//        {
//            reKey = reKey | 0x00;
//        }
//        delay_us( 30 );
//    }
//    TM1637_Ack();
//    TM1637_Stop();
//    return( reKey );
//}
// 
// 
////�������������������ݵ�λ��ǰ��λ�ں�
//unsigned char TM1637_KeyProcess(void)
//{
//    unsigned char temp;
//    unsigned char keyNum = 0;
//    temp = TM1637_ScanKey();                                 //��ȡ��������ֵ
//    if( temp != 0xff )
//    {
//        switch( temp )
//        {
//        case 0xf7 :							//K1��SG1��Ӧ��������
//            keyNum = 1;
//            break;
//        case 0xf6 :						 	//K1��SG2��Ӧ��������
//            keyNum = 2;
//            break;
//        case 0xf5 :							//K1��SG3��Ӧ��������
//            keyNum = 3;
//            break;
//        case 0xf4 :							//K1��SG4��Ӧ��������
//            keyNum = 4;
//            break;
//        case 0xf3 :							//K1��SG5��Ӧ��������
//            keyNum = 5;
//            break;
//        case 0xf2 :						 	//K1��SG6��Ӧ��������
//            keyNum = 6;
//            break;
//        case 0xf1 :							//K1��SG7��Ӧ��������
//            keyNum = 7;
//            break;
//        case 0xf0 :							//K1��SG8��Ӧ��������
//            keyNum = 8;
//            break;
// 
//        case 0xef :						 	//K2��SG1��Ӧ��������
//            keyNum = 9;
//            break;
//        case 0xee :							//K2��SG2��Ӧ��������
//            keyNum = 10;
//            break;
//        case 0xed :							//K2��SG3��Ӧ��������
//            keyNum = 11;
//            break;
//        case 0xec :						 	//K2��SG4��Ӧ��������
//            keyNum = 12;
//            break;
//        case 0xeb :							//K2��SG5��Ӧ��������
//            keyNum = 13;
//            break;
//        case 0xea :							//K2��SG6��Ӧ��������
//            keyNum = 14;
//            break;
//        case 0xe9 :							//K2��SG7��Ӧ��������
//            keyNum = 15;
//            break;
//        case 0xe8 :							//K2��SG8��Ӧ��������
//            keyNum = 16;
//            break;
//        default   :
//            keyNum = 0;
//            break;
//        }
//    }
//    return keyNum;
//}
// 


unsigned char dt=20; //��ʱʱ�����
unsigned char  dpFlag = 0;//��������˵�� 
unsigned char  Data[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x00};
unsigned char  DataDp[]={0xbf,0x86,0xdb,0xcf,0xe6,0xed,0xfd,0x87,0xff,0xef,0x80};
struct {
	char shi;
	char ge;
}FB;
 
struct {
	char shi;
	char ge;
}SB;

void TM1637_DATA_Config(unsigned char fb,unsigned char fb_show_flag,unsigned char sb,unsigned char sb_show_flag,unsigned char dot)
{
	if(fb_show_flag==1)
	{
		FB.shi = fb/10;
		FB.ge =  fb%10;
	}
	else
	{
		FB.shi=FB.ge=10;
	}
	if(sb_show_flag==1)
	{
		SB.shi = sb/10;
		SB.ge =  sb%10;
	}
	else
	{
		SB.shi=SB.ge=10;
	}
	dpFlag = dot;
		
}

void I2C_Start(void)
{
    CLK_1;
	DIO_1;
	delay_us(dt);
	
	DIO_0;
	delay_us(dt);
	CLK_0;
	delay_us(dt);
}

void I2C_stop(void)
{
	
	CLK_0;
	delay_us(dt);
  DIO_0;
	delay_us(dt);
	
  CLK_1;
	delay_us(dt);	
	DIO_1;
	delay_us(dt);  	
}

void TM1637_WriteBit(unsigned char mBit)
{
	CLK_0;
	delay_us(dt);
	if(mBit)
		DIO_1;
	else
		DIO_0;
	delay_us(dt);
	CLK_1;
	delay_us(dt);	
}
  
void TM1637_WriteByte(unsigned char Byte)
{
	char loop = 0;
	for(loop = 0;loop<8;loop++)
	{
		 TM1637_WriteBit((Byte>>loop) & 0x01);//��д��λ	
	}
	CLK_0;
	delay_us(dt);
	DIO_1;	
	delay_us(dt);
	CLK_1;		
}
void TM1637_WriteCommand(unsigned char mData)
{
	I2C_Start();
	TM1637_WriteByte(mData);
	I2C_stop();
}


/*
*@note  д���ַ������
*/

void TM1637_WriteData(unsigned char addr,unsigned char mData)
{
	I2C_Start();
	TM1637_WriteByte(addr);
	TM1637_WriteByte(mData);
	I2C_stop();
}
void TM1637_DATA_Display(void)
{
	TM1637_WriteCommand(0x44);
	TM1637_WriteData(0xc0,Data[FB.shi]);
	if(dpFlag)
			TM1637_WriteData(0xc1,DataDp[FB.ge]);
	else
			TM1637_WriteData(0xc1,Data[FB.ge]);
	
	TM1637_WriteData(0xc2, Data[SB.shi]);
	TM1637_WriteData(0xc3, Data[SB.ge]);
	TM1637_WriteCommand(0x88);
 }


void TM1637_SetBrightness(unsigned char level)
{
   I2C_Start();
   TM1637_WriteByte( 0x88 | level );							//����ʾ���������-----���������ȿ���0---7  ������14/16
//    TM1637_Ack();
   I2C_stop();
}


//д��ʾ�Ĵ���  ��ַ������
// add ����ܵĵ�ַ 0--5
// value Ҫ��ʾ������
void TM1637_Display_NoINC(uint8_t addr, uint8_t value)
{
   uint8_t i;
   I2C_Start();
   TM1637_WriteByte( 0x44 );								//д���ݵ���ʾ�Ĵ��� 40H ��ַ�Զ���1 ģʽ,44H �̶���ַģʽ,����������Լ�1ģʽ
//    TM1637_Ack();
   I2C_stop();

   I2C_Start();
   TM1637_WriteByte( 0xC0 | addr );					//��ַ�������� ��ʾ��ַ C0H---C5H

   TM1637_WriteByte( value );			 				//��������   value�洢Ҫ��ʾ������
   I2C_stop();
#if 1
   I2C_Start();
   TM1637_WriteByte( 0x88 | 0x01 );							//����ʾ���������-----���������ȿ���0---7  ������14/16
   //TM1637_Ack();
   I2C_stop();
#endif
}

void TM1637_Display_NoINC_NUM(uint8_t addr, uint8_t value)
{

    TM1637_Display_NoINC(addr, tab[value]);
}