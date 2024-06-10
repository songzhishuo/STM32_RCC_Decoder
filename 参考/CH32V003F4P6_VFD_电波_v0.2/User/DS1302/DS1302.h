/*
 * DS1302.h
 *
 *  Created on: 2023Äê2ÔÂ4ÈÕ
 *      Author: 12263
 */

#ifndef USER_DS1302_DS1302_H_
#define USER_DS1302_DS1302_H_

#include "debug.h"

#define DS1302_ALL_PIN_IN_GPIO_C RCC_APB2Periph_GPIOC
#define DS1302_ALL_PIN_IN_GPIO_D RCC_APB2Periph_GPIOD
#define DS1302_PIN_IN_GPIO_C_AND_GPIO_D RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD

#define DS1302_SCK_GPIO GPIOC
#define DS1302_SDA_GPIO GPIOC
#define DS1302_RST_GPIO GPIOC

#define DS1302_SCK_PIN GPIO_Pin_2
#define DS1302_SDA_PIN GPIO_Pin_1
#define DS1302_RST_PIN GPIO_Pin_0

#define DS1302_SCK1    GPIO_SetBits(DS1302_SCK_GPIO,DS1302_SCK_PIN)
#define DS1302_SCK0    GPIO_ResetBits(DS1302_SCK_GPIO,DS1302_SCK_PIN)

#define DS1302_SDA1    GPIO_SetBits(DS1302_SDA_GPIO,DS1302_SDA_PIN)
#define DS1302_SDA0    GPIO_ResetBits(DS1302_SDA_GPIO,DS1302_SDA_PIN)

#define DS1302_RST1    GPIO_SetBits(DS1302_RST_GPIO,DS1302_RST_PIN)
#define DS1302_RST0    GPIO_ResetBits(DS1302_RST_GPIO,DS1302_RST_PIN)

#define DS1302_READ_SDA   GPIO_ReadInputDataBit(DS1302_SDA_GPIO,DS1302_SDA_PIN)

void DS1302_GPIO_INIT();
void ds1302_time_set(unsigned char num1,unsigned char num2,unsigned char num3,unsigned char num4,unsigned char num5,unsigned char num6,unsigned char num7);
unsigned long get_ds1302_time(unsigned char num);
unsigned char get_ds1302_day(void);

#endif /* USER_DS1302_DS1302_H_ */
