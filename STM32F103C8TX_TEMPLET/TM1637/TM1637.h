#include "main.h"


#ifndef __TM1637_H__
#define __TM1637_H__

//extern char disp_num[6];

/*时钟引脚配置*/
#define TM1637_CLK_IO_PIN           TM1637_CLK_Pin                      
#define TM1637_CLK_IO_PORT          TM1637_CLK_GPIO_Port


/*数据引脚配置*/
#define TM1637_DIO_IO_PIN           TM1637_DIO_Pin
#define TM1637_DIO_IO_PORT          TM1637_DIO_GPIO_Port    

//#define TM1637_DIO_IN()  {                                      \
//    GPIO_InitTypeDef GPIO_InitStruct = {0};                     \
//    GPIO_InitStruct.Pin = TM1637_DIO_IO_PIN;                          \
//    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;                     \
//    GPIO_InitStruct.Pull = GPIO_NOPULL;                         \
//    HAL_GPIO_Init(TM1637_DIO_IO_PORT, &GPIO_InitStruct);         \
//}

//#define TM1637_DIO_OUT() {                              \
//    GPIO_InitTypeDef GPIO_InitStruct = {0};             \
//    GPIO_InitStruct.Pin = TM1637_DIO_IO_PIN;                  \
//    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;         \
//    GPIO_InitStruct.Pull = GPIO_NOPULL;                 \
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;        \
//    HAL_GPIO_Init(TM1637_DIO_IO_PORT, &GPIO_InitStruct); \
//}

//void TM1637_Init( void );
//void TM1637_Display_NoINC(uint8_t add, uint8_t value);
//void TM1637_SetBrightness(unsigned char level);
//unsigned char TM1637_ScanKey(void);
//void TM1637_Display_INC(void);
//// HAL_GPIO_WritePin(TM1637_DIO_IO_PORT, TM1637_DIO_IO_PIN, GPIO_PIN_RESET);
//// HAL_GPIO_WritePin(TM1637_CLK_IO_PORT, TM1637_CLK_IO_PIN, GPIO_PIN_RESET);
//// HAL_GPIO_ReadPin(TM1637_DIO_IO_PORT, TM1637_DIO_IO_PIN)



#define CLK_0 	HAL_GPIO_WritePin(TM1637_CLK_IO_PORT,TM1637_CLK_IO_PIN,GPIO_PIN_RESET)
#define CLK_1 	HAL_GPIO_WritePin(TM1637_CLK_IO_PORT,TM1637_CLK_IO_PIN,GPIO_PIN_SET)

#define DIO_0 	HAL_GPIO_WritePin(TM1637_DIO_IO_PORT, TM1637_DIO_IO_PIN, GPIO_PIN_RESET)
#define DIO_1 	HAL_GPIO_WritePin(TM1637_DIO_IO_PORT, TM1637_DIO_IO_PIN, GPIO_PIN_SET)

extern unsigned char tab[];


void TM1637_DATA_Config(unsigned char fb,unsigned char fb_show_flag,unsigned char sb,unsigned char sb_show_flag,unsigned char dot);
void I2C_Start(void);
void I2C_stop(void);
void TM1637_WriteBit(unsigned char mBit);
void TM1637_WriteByte(unsigned char Byte);
void TM1637_WriteCommand(unsigned char mData);
void TM1637_WriteData(unsigned char addr,unsigned char mData);
void TM1637_DATA_Display(void);
void TM1637_SetBrightness(unsigned char level);
void TM1637_Display_NoINC(uint8_t addr, uint8_t value);
void TM1637_Display_NoINC_NUM(uint8_t addr, uint8_t value);
#endif /* __TM1637_H__ */