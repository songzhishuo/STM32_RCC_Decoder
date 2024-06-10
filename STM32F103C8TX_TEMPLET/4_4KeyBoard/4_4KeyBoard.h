#ifndef __4_4_KEYBOARD_H__
#define __4_4_KEYBOARD_H__

#include "main.h"
extern char key_map[17];
extern volatile uint8_t key;

void Key_Bord_Init(void);
//非阻塞方式获取按键，返回0，没有键值，非0，获得键值
//中断中调用
uint8_t Get_KeyNum(uint16_t GPIO_Pin);

#define ReadPin_Port  GPIOB

#endif //__4_4_KEYBOARD_H__