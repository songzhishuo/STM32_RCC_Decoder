#ifndef __LCD_DISPLAY_H__
#define __LCD_DISPLAY_H__

#include "main.h"
#include <stdio.h>
#include "u8x8.h" /*u8gͼ�ο�*/
#include "u8g2.h" /*u8gͼ�ο�*/
#include "spi.h"

#include "usart.h"

#define DEV_NUM 6

#define MAX_POWER_NUM 5

typedef struct time_info_s 
{
	uint16_t year;				//���
	uint8_t month;				//�·�
	uint8_t day;				//����
	uint8_t week;				//����Ϣ

	uint8_t hour;				//Сʱ
	uint8_t minute;				//����
	uint8_t second;				//��
} time_info_typeDef;


typedef struct lcd_display_data
{
	char dis_index; // ��ǰ��ʾ��ҳ��

	uint8_t get_time_flag;		//��ȡʱ���־
	time_info_typeDef time_now;	//��ǰʱ��
	time_info_typeDef time_set;	//��ʱʱ��
	uint8_t sleep_mode;	//����ģʽ

	uint8_t dis_point_index;	//���λ��

	uint16_t key0_press_cnt;	//����0 ����ʱ��
	uint8_t sleep_flag;			//���߱�־
	float temp_val;				//��ǰ�¶�ֵ

} lcd_dis_data_typeDef;

void lcd_init(lcd_dis_data_typeDef *pdata);
void lcd_show();



extern lcd_dis_data_typeDef display_data;
#endif