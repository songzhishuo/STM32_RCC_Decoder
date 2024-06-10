#ifndef _DS18B20__
#define _DS18B20__

#include "debug.h"

#define DS18B20_PIN_IN_GPIOD RCC_APB2Periph_GPIOD
#define DS18B20_PIN_IN_GPIOC RCC_APB2Periph_GPIOC

#define DS18B20_GPIO GPIOC

#define DS18B20_PIN GPIO_Pin_7

#define DS18B20_OUT_1 GPIO_SetBits(DS18B20_GPIO,DS18B20_PIN)
#define DS18B20_OUT_0 GPIO_ResetBits(DS18B20_GPIO,DS18B20_PIN)

#define DS18B20_IN GPIO_ReadInputDataBit(DS18B20_GPIO,DS18B20_PIN)

unsigned long get_ds18b20();

#endif
