#ifndef _VFD_DRIVER__
#define _VFD_DRIVER__

#include "debug.h"

#define VFD_ALL_PIN_IN_GPIO_C RCC_APB2Periph_GPIOC
#define VFD_ALL_PIN_IN_GPIO_D RCC_APB2Periph_GPIOD
#define VFD_PIN_IN_GPIO_C_AND_GPIO_D RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD


#define VFD_DIN_GPIO GPIOC
#define VFD_CLK_GPIO GPIOC
#define VFD_CS_GPIO  GPIOC
#define VFD_RST_GPIO GPIOC


#define VFD_DIN_Pin GPIO_Pin_6
#define VFD_CLK_Pin GPIO_Pin_5
#define VFD_CS_Pin  GPIO_Pin_4
#define VFD_RST_Pin GPIO_Pin_3

#define VFD_DIN1    GPIO_SetBits(VFD_DIN_GPIO,VFD_DIN_Pin)
#define VFD_DIN0    GPIO_ResetBits(VFD_DIN_GPIO,VFD_DIN_Pin)

#define VFD_CLK1    GPIO_SetBits(VFD_CLK_GPIO,VFD_CLK_Pin)
#define VFD_CLK0    GPIO_ResetBits(VFD_CLK_GPIO,VFD_CLK_Pin)

#define VFD_CS1    GPIO_SetBits(VFD_CS_GPIO,VFD_CS_Pin)
#define VFD_CS0    GPIO_ResetBits(VFD_CS_GPIO,VFD_CS_Pin)

#define VFD_RST1    GPIO_SetBits(VFD_RST_GPIO,VFD_RST_Pin)
#define VFD_RST0    GPIO_ResetBits(VFD_RST_GPIO,VFD_RST_Pin)


void VFD_init();
void VFD_show(void);
void VFD_WriteOneChar(unsigned char x, unsigned char chr);
void VFD_WriteStr(unsigned char x, char *str);
void VFD_WriteOneStr(unsigned char x, unsigned char chr);
void VFD_light(u8 data);
void VFD_GPIO_INIT();


#endif
