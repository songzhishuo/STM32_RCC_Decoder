#ifndef _BPC__
#define _BPC__

#include "main.h"

//#define BPC_PIN_IN_GPIO_C RCC_APB2Periph_GPIOC
//#define BPC_PIN_IN_GPIO_D RCC_APB2Periph_GPIOD

//////>BPCÊ±ÖÓ Òý½Å
#define BPC_GPIO GPIOA					

#define BPC_Pin GPIO_PIN_0

#define BPC_IN_PIN HAL_GPIO_ReadPin(BPC_GPIO, BPC_Pin)


void BPC_TIMER_IQR(void);
void BPC_GPIO_INIT(void);
void BPC_DataHandle(void);
void BPC_RX();
#endif
