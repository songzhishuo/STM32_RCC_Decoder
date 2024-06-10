#include "KEY.h"

void KEY_GPIO_INIT()
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

        RCC_APB2PeriphClockCmd(KEY_ALL_PIN_IN_GPIO_D, ENABLE);

        GPIO_InitStructure.GPIO_Pin = KEY1_PIN;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_Init(KEY1_GPIO, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = KEY2_PIN;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_Init(KEY2_GPIO, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = KEY3_PIN;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_Init(KEY3_GPIO, &GPIO_InitStructure);
}
