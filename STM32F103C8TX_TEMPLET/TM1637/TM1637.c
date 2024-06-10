#include "TM1637.h"
#include "main.h"




//static inline void delay_ms(uint32_t delay)
//{
//	HAL_Delay(delay);
//}

#define CPU_FREQUENCY_MHZ 72				// CPU主频，根据实际进行修改
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
   0x00,/*21 黑屏*/
};
// 
//// 最高位设置为1时显示 数码管上的":" 符号
// char disp_num[6] = {0x3F, 0x06 | 0x80, 0x5B, 0x4F, 0x66, 0x6D};			//存放6个数码管要显示的内容

////端口初始化
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
////起始位 CLK为高电平时，DIO由高变低
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
////等待应答 传输数据正确时，在第八个时钟下降沿，芯片内部会产生一个ACK信号，将DIO管脚拉低，在第九个时钟结束之后释放DIO总线。
//static void TM1637_Ack( void )
//{
//    TM1637_DIO_IN();
//    // TM1637_CLK = 0;
//    HAL_GPIO_WritePin(TM1637_CLK_IO_PORT, TM1637_CLK_IO_PIN, 0);
//    delay_us( 5 );													//在第八个时钟下降沿之后延时 5us，开始判断 ACK 信号
//    while( HAL_GPIO_ReadPin(TM1637_DIO_IO_PORT, TM1637_DIO_IO_PIN) );								//等待应答位  这一行代码也可以不要 不影响实际使用效果 在使用软件仿真的时候需要屏蔽这句代码，否则程序就会卡在这里。
//    // TM1637_CLK = 1;
//    HAL_GPIO_WritePin(TM1637_CLK_IO_PORT, TM1637_CLK_IO_PIN, 1);
//    delay_us( 2 );
//    // TM1637_CLK = 0;
//    HAL_GPIO_WritePin(TM1637_CLK_IO_PORT, TM1637_CLK_IO_PIN, 0);
//}
// 
////停止位 CLK为高电平时，DIO由低变高
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
////输入数据在CLK的低电平变化，在CLK的高电平被传输。
////每传输一个字节，芯片内部在第八个时钟下降沿产生一个ACK
//// 写一个字节
//static void TM1637_WriteByte( unsigned char oneByte )
//{
//    unsigned char i;
//    TM1637_DIO_OUT();
//    for( i = 0; i < 8; i++ )
//    {
//        // TM1637_CLK = 0;
//        HAL_GPIO_WritePin(TM1637_CLK_IO_PORT, TM1637_CLK_IO_PIN, 0);
//        if( oneByte & 0x01 )									//低位在前
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
////写显示寄存器  地址自增
//void TM1637_Display_INC( void )
//{
//    unsigned char i;
//    TM1637_Start();
//    TM1637_WriteByte( 0x40 );									//写数据到显示寄存器 40H 地址自动加1 模式,44H 固定地址模式,本程序采用自加1模式
//    TM1637_Ack();
//    TM1637_Stop();
//    TM1637_Start();
//    TM1637_WriteByte( 0xC0 );									//地址命令设置 显示地址 00H
//    TM1637_Ack();
// 
//    for( i = 0; i < 6; i++ )									//地址自加，不必每次都写地址
//    {
//        TM1637_WriteByte( disp_num[i] );			//发送数据   disp_num[]中存储6个数码管要显示的内容
//        TM1637_Ack();
//    }
//    TM1637_Stop();
//#if 0
//    TM1637_Start();
//    TM1637_WriteByte( 0x88 | 0x07 );					//开显示，最大亮度-----调节脉冲宽度控制0---7  脉冲宽度14/16
//    TM1637_Ack();
//    TM1637_Stop();
//#endif
//}
// 
////写显示寄存器  地址不自增
//// add 数码管的地址 0--5
//// value 要显示的内容
//void TM1637_Display_NoINC(uint8_t add, uint8_t value)
//{
//    uint8_t i;
//    TM1637_Start();
//    TM1637_WriteByte( 0x44 );								//写数据到显示寄存器 40H 地址自动加1 模式,44H 固定地址模式,本程序采用自加1模式
//    TM1637_Ack();
//    TM1637_Stop();
// 
//    TM1637_Start();
//    TM1637_WriteByte( 0xC0 | add );					//地址命令设置 显示地址 C0H---C5H
//    TM1637_Ack();
// 
//    TM1637_WriteByte( value );			 				//发送数据   value存储要显示的内容
//    TM1637_Ack();
//    TM1637_Stop();
//#if 0
//    TM1637_Start();
//    TM1637_WriteByte( 0x88 | 0x07 );							//开显示，最大亮度-----调节脉冲宽度控制0---7  脉冲宽度14/16
//    TM1637_Ack();
//    TM1637_Stop();
//#endif
//}
// 
//// level : 设置亮度等级  0---7
//void TM1637_SetBrightness(unsigned char level)
//{
//    TM1637_Start();
//    TM1637_WriteByte( 0x88 | level );							//开显示，最大亮度-----调节脉冲宽度控制0---7  脉冲宽度14/16
//    TM1637_Ack();
//    TM1637_Stop();
//}

////读按键  读按键时，时钟频率应小于 250K，先读低位，后读高位。
//unsigned char TM1637_ScanKey(void)
//{
//    unsigned char reKey, i;
//    TM1637_Start();
//    TM1637_WriteByte( 0x42 );						//读键扫数据
//    TM1637_Ack();
//    // TM1637_DIO = 1;											//在读按键之前拉高数据线
//    HAL_GPIO_WritePin(TM1637_DIO_IO_PORT, TM1637_DIO_IO_PIN, 1);
//    TM1637_DIO_IN();
//    for( i = 0; i < 8; i++ )						//从低位开始读
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
////按键处理函数，按键数据低位在前高位在后
//unsigned char TM1637_KeyProcess(void)
//{
//    unsigned char temp;
//    unsigned char keyNum = 0;
//    temp = TM1637_ScanKey();                                 //读取按键返回值
//    if( temp != 0xff )
//    {
//        switch( temp )
//        {
//        case 0xf7 :							//K1与SG1对应按键按下
//            keyNum = 1;
//            break;
//        case 0xf6 :						 	//K1与SG2对应按键按下
//            keyNum = 2;
//            break;
//        case 0xf5 :							//K1与SG3对应按键按下
//            keyNum = 3;
//            break;
//        case 0xf4 :							//K1与SG4对应按键按下
//            keyNum = 4;
//            break;
//        case 0xf3 :							//K1与SG5对应按键按下
//            keyNum = 5;
//            break;
//        case 0xf2 :						 	//K1与SG6对应按键按下
//            keyNum = 6;
//            break;
//        case 0xf1 :							//K1与SG7对应按键按下
//            keyNum = 7;
//            break;
//        case 0xf0 :							//K1与SG8对应按键按下
//            keyNum = 8;
//            break;
// 
//        case 0xef :						 	//K2与SG1对应按键按下
//            keyNum = 9;
//            break;
//        case 0xee :							//K2与SG2对应按键按下
//            keyNum = 10;
//            break;
//        case 0xed :							//K2与SG3对应按键按下
//            keyNum = 11;
//            break;
//        case 0xec :						 	//K2与SG4对应按键按下
//            keyNum = 12;
//            break;
//        case 0xeb :							//K2与SG5对应按键按下
//            keyNum = 13;
//            break;
//        case 0xea :							//K2与SG6对应按键按下
//            keyNum = 14;
//            break;
//        case 0xe9 :							//K2与SG7对应按键按下
//            keyNum = 15;
//            break;
//        case 0xe8 :							//K2与SG8对应按键按下
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


unsigned char dt=20; //延时时间参数
unsigned char  dpFlag = 0;//主函数有说明 
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
		 TM1637_WriteBit((Byte>>loop) & 0x01);//先写低位	
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
*@note  写入地址和数据
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
   TM1637_WriteByte( 0x88 | level );							//开显示，最大亮度-----调节脉冲宽度控制0---7  脉冲宽度14/16
//    TM1637_Ack();
   I2C_stop();
}


//写显示寄存器  地址不自增
// add 数码管的地址 0--5
// value 要显示的内容
void TM1637_Display_NoINC(uint8_t addr, uint8_t value)
{
   uint8_t i;
   I2C_Start();
   TM1637_WriteByte( 0x44 );								//写数据到显示寄存器 40H 地址自动加1 模式,44H 固定地址模式,本程序采用自加1模式
//    TM1637_Ack();
   I2C_stop();

   I2C_Start();
   TM1637_WriteByte( 0xC0 | addr );					//地址命令设置 显示地址 C0H---C5H

   TM1637_WriteByte( value );			 				//发送数据   value存储要显示的内容
   I2C_stop();
#if 1
   I2C_Start();
   TM1637_WriteByte( 0x88 | 0x01 );							//开显示，最大亮度-----调节脉冲宽度控制0---7  脉冲宽度14/16
   //TM1637_Ack();
   I2C_stop();
#endif
}

void TM1637_Display_NoINC_NUM(uint8_t addr, uint8_t value)
{

    TM1637_Display_NoINC(addr, tab[value]);
}