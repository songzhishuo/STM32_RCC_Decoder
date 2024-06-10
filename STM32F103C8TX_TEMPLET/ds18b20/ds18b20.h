#ifndef __DS18B20_H
#define __DS18B20_H 

#include "main.h"

#if 0
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2))
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr))
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum))

#define GPIOA_ODR_Addr    (GPIOA_BASE+12)
#define GPIOB_ODR_Addr    (GPIOB_BASE+12)
#define GPIOC_ODR_Addr    (GPIOC_BASE+12)
#define GPIOD_ODR_Addr    (GPIOD_BASE+12)
#define GPIOE_ODR_Addr    (GPIOE_BASE+12)
#define GPIOF_ODR_Addr    (GPIOF_BASE+12)
#define GPIOG_ODR_Addr    (GPIOG_BASE+12)

#define GPIOA_IDR_Addr    (GPIOA_BASE+8)
#define GPIOB_IDR_Addr    (GPIOB_BASE+8)
#define GPIOC_IDR_Addr    (GPIOC_BASE+8)
#define GPIOD_IDR_Addr    (GPIOD_BASE+8)
#define GPIOE_IDR_Addr    (GPIOE_BASE+8)
#define GPIOF_IDR_Addr    (GPIOF_BASE+8)
#define GPIOG_IDR_Addr    (GPIOG_BASE+8)

#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)
#endif


#define DS18B20_PIN GPIO_PIN_12
#define DS18B20_GPIO_PORT GPIOB

//IO��������
#if 0
#define DS18B20_IO_IN()  {GPIOA->CRL&=0XFFFFFFF0;GPIOA->CRL|=8<<0;}
#define DS18B20_IO_OUT() {GPIOA->CRL&=0XFFFFFFF0;GPIOA->CRL|=3<<0;}
#endif
#define DS18B20_IO_IN()  {                                      \
    GPIO_InitTypeDef GPIO_InitStruct = {0};                     \
    GPIO_InitStruct.Pin = DS18B20_PIN;                          \
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;                     \
    GPIO_InitStruct.Pull = GPIO_NOPULL;                         \
    HAL_GPIO_Init(DS18B20_GPIO_PORT, &GPIO_InitStruct);         \
}

#define DS18B20_IO_OUT() {                              \
    GPIO_InitTypeDef GPIO_InitStruct = {0};             \
    GPIO_InitStruct.Pin = DS18B20_PIN;                  \
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;         \
    GPIO_InitStruct.Pull = GPIO_NOPULL;                 \
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;        \
    HAL_GPIO_Init(DS18B20_GPIO_PORT, &GPIO_InitStruct); \
}

#if 0
//IO��������
#define	DS18B20_DQ_OUT PAout(0) //���ݶ˿�	PA0
#define	DS18B20_DQ_IN  PAin(0)  //���ݶ˿�	PA0 
#endif
#define	DS18B20_DQ_OUT PAout(0) //���ݶ˿�	PA0         
#define	DS18B20_DQ_IN  PAin(0)  //���ݶ˿�	PA0 


uint8_t DS18B20_Init(void);			//��ʼ��DS18B20
short DS18B20_Get_Temp_s(void);		//��ȡ�¶�
float DS18B20_Get_Temp_f(void);       //��ȡ�¶�
void DS18B20_Start(void);			//��ʼ�¶�ת��
void DS18B20_Write_Byte(uint8_t dat);//д��һ���ֽ�
uint8_t DS18B20_Read_Byte(void);	//����һ���ֽ�
uint8_t DS18B20_Read_Bit(void);		//����һ��λ
uint8_t DS18B20_Check(void);		//����Ƿ����DS18B20
void DS18B20_Rst(void);				//��λDS18B20
#endif

