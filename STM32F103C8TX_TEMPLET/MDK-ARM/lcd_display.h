#ifndef __LCD_DISPLAY_H__
#define __LCD_DISPLAY_H__

#include "main.h"
#include <stdio.h>
#include "u8x8.h" /*u8g图形库*/
#include "u8g2.h" /*u8g图形库*/
#include "spi.h"

#include "usart.h"

#define DEV_NUM 6

#define MAX_POWER_NUM 5

typedef struct time_info_s 
{
	uint16_t year;				//年份
	uint8_t month;				//月份
	uint8_t day;				//日期
	uint8_t week;				//周信息

	uint8_t hour;				//小时
	uint8_t minute;				//分钟
	uint8_t second;				//秒
} time_info_typeDef;


typedef struct lcd_display_data
{
	char dis_index; // 当前显示的页面

	uint8_t get_time_flag;		//获取时间标志
	time_info_typeDef time_now;	//当前时间
	time_info_typeDef time_set;	//定时时间
	uint8_t sleep_mode;	//休眠模式

	uint8_t dis_point_index;	//光标位置

	uint16_t key0_press_cnt;	//按键0 按下时间
	uint8_t sleep_flag;			//休眠标志
	float temp_val;				//当前温度值

} lcd_dis_data_typeDef;

void lcd_init(lcd_dis_data_typeDef *pdata);
void lcd_show();



extern lcd_dis_data_typeDef display_data;
#endif