#ifndef _BPC__
#define _BPC__

#include "debug.h"
#include "DS1302/DS1302.h"

#define BPC_PIN_IN_GPIO_C RCC_APB2Periph_GPIOC
#define BPC_PIN_IN_GPIO_D RCC_APB2Periph_GPIOD

#define BPC_GPIO GPIOD

#define BPC_Pin GPIO_Pin_0

#define BPC_IN_PIN GPIO_ReadInputDataBit(BPC_GPIO, BPC_Pin)



void BPC_GPIO_INIT(void);
void BPC_DataHandle(void);
void BPC_RX();
#endif
