#ifndef __4_4_KEYBOARD_H__
#define __4_4_KEYBOARD_H__

#include "main.h"
extern char key_map[17];
extern volatile uint8_t key;

void Key_Bord_Init(void);
//��������ʽ��ȡ����������0��û�м�ֵ����0����ü�ֵ
//�ж��е���
uint8_t Get_KeyNum(uint16_t GPIO_Pin);

#define ReadPin_Port  GPIOB

#endif //__4_4_KEYBOARD_H__