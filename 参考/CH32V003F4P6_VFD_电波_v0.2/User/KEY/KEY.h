#ifndef _KEY__
#define _KEY__

#include "debug.h"

#define KEY_ALL_PIN_IN_GPIO_C RCC_APB2Periph_GPIOC
#define KEY_ALL_PIN_IN_GPIO_D RCC_APB2Periph_GPIOD
#define KEY_PIN_IN_GPIOC_AND_GPIOD RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD

#define KEY1_GPIO GPIOD
#define KEY2_GPIO GPIOD
#define KEY3_GPIO GPIOD

#define KEY1_PIN GPIO_Pin_1
#define KEY2_PIN GPIO_Pin_2
#define KEY3_PIN GPIO_Pin_3

#define KEY1_IN_PIN GPIO_ReadInputDataBit(KEY1_GPIO, KEY1_PIN)
#define KEY2_IN_PIN GPIO_ReadInputDataBit(KEY2_GPIO, KEY2_PIN)
#define KEY3_IN_PIN GPIO_ReadInputDataBit(KEY3_GPIO, KEY3_PIN)

void KEY_GPIO_INIT();

#endif
