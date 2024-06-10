#include "lcd_display.h"

lcd_dis_data_typeDef display_data;


static u8g2_t u8g2; //首先定义u8g2的对象
static lcd_dis_data_typeDef *lcd_data;
static uint8_t u8x8_stm32_gpio_and_delay(U8X8_UNUSED u8x8_t *u8x8,
    U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int,
    U8X8_UNUSED void *arg_ptr);
		
static uint8_t u8x8_byte_4wire_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int,
    void *arg_ptr);		



void dis_src_master(u8g2_t *u8g2) ;
void dis_src_2th(u8g2_t *u8g2)  ;
void dis_src_3th(u8g2_t *u8g2)  ;

void lcd_init(lcd_dis_data_typeDef *pdata)
{
	u8g2_Setup_st7565_jlx12864_1(&u8g2, U8G2_R0, u8x8_byte_4wire_hw_spi, u8x8_stm32_gpio_and_delay); 
	u8g2_InitDisplay(&u8g2);
	u8g2_SetPowerSave(&u8g2, 0);
    /*获取数据显示源*/
    if(pdata != NULL)
    {
       lcd_data = pdata; 
    }  
}

void lcd_show()
{
	u8g2_FirstPage(&u8g2);
  do
  {
    if(display_data.sleep_mode == 1)
    {
      dis_src_3th(&u8g2);
    }
    else if(lcd_data->dis_index == 0)
    {
      dis_src_master(&u8g2);
    }
    else if(lcd_data->dis_index == 1)
    {
      dis_src_2th(&u8g2);
    }

  } while (u8g2_NextPage(&u8g2));
}
#define DEBUG	1
/**
 * @brief 
 * 
 * @param u8g2 
 */
void dis_src_master(u8g2_t *u8g2)           /*电池检测 7号 5号*/
{
    uint8_t str_buf0[32] = {0};
    uint8_t str_buf1[32] = {0};    
    uint8_t str_buf2[32] = {0};     

    uint8_t x = 10, y = 0;

    u8g2_SetFontMode(u8g2, 1);  // Transparent
    u8g2_SetFontDirection(u8g2, 0);

    /*光标显示*/

    u8g2_DrawStr(u8g2, 0, 12 * (1+lcd_data->dis_point_index%4), (const char *)">"); 

			 
    u8g2_SetFont(u8g2, u8g2_font_fur11_tr);

    if(lcd_data->get_time_flag == 0)      //没有获取到BPC时间
    {
      sprintf(str_buf0, "Receiving BPC...");

    }
    else
    {
      sprintf(str_buf0, "%04d-%02d-%02d", lcd_data->time_now.year, lcd_data->time_now.month, lcd_data->time_now.day);
      sprintf(str_buf1, "%02d:%02d:%02d", lcd_data->time_now.hour, lcd_data->time_now.minute, lcd_data->time_now.second);
    }
    
  
		sprintf(str_buf2, "temp:%02.02f", lcd_data->temp_val);				//温度显示
		

	  y = y + 12; 
    u8g2_DrawStr(u8g2, x, y, (const char *)str_buf0);       

    
		y = y + 12; 
		u8g2_DrawStr(u8g2, x, y, (const char *)str_buf1);       
    
    
    y = y + 12; 	
		u8g2_DrawStr(u8g2, x, y, (const char *)str_buf2);       

    y = y + 12; 	
		u8g2_DrawStr(u8g2, x, y, (const char *)"sleep mode"); 
}

void dis_src_2th(u8g2_t *u8g2)           /*副屏显示  检测电池电压 容量 温度*/
{
    uint8_t str_buf0[32] = {0};
    uint8_t str_buf1[32] = {0};    
    uint8_t str_buf2[32] = {0};     

    uint8_t x = 10, y = 0;

    u8g2_SetFontMode(u8g2, 1);  // Transparent
    u8g2_SetFontDirection(u8g2, 0);

    /*光标显示*/

    u8g2_DrawStr(u8g2, 0, 12 * (1+lcd_data->dis_point_index%4), (const char *)">"); 

    u8g2_SetFont(u8g2, u8g2_font_fur11_tr);

    sprintf(str_buf0, "hour: %02d", lcd_data->time_set.hour);
    sprintf(str_buf1, "minute: %02d", lcd_data->time_set.minute);
    sprintf(str_buf2, "second: %02d", lcd_data->time_set.second);
    


    y = y + 12; 
    u8g2_DrawStr(u8g2, x, y, (const char *)str_buf0);       

    
		y = y + 12; 
		u8g2_DrawStr(u8g2, x, y, (const char *)str_buf1);       
    
    
    y = y + 12; 	
		u8g2_DrawStr(u8g2, x, y, (const char *)str_buf2);  
}

void dis_src_3th(u8g2_t *u8g2)        /*电能回收*/
{

  u8g2_ClearBuffer(u8g2);     /*清屏*/
//    uint8_t str_buf0[32] = {0};
//    uint8_t str_buf1[32] = {0};    
//    //uint8_t str_buf2[32] = {0};     

//    uint8_t x = 0, y = 0;

//    u8g2_SetFontMode(u8g2, 1);  // Transparent
//    u8g2_SetFontDirection(u8g2, 0);
//    u8g2_SetFont(u8g2, u8g2_font_fub17_tf);    

//    /*显示标题*/
//    y = y+18;
//    u8g2_DrawStr(u8g2, 0, y, (const char *)"Energy");

//    u8g2_SetFont(u8g2, u8g2_font_fub11_tf);
//    u8g2_DrawStr(u8g2, 65, y, (const char *)" reclaim");


//		
//#if DEBUG
//    //u8g2_SetFont(u8g2, u8g2_font_fur11_tr);
//		u8g2_SetFont(u8g2, test_u8g2_12); 
//    sprintf(str_buf0, " 温度 : %.04f 度 ", lcd_data->temp_val);
//    
//    if(lcd_data->temp_val > 29)
//      sprintf(str_buf1, " 温度报警");
//    else 
//      sprintf(str_buf1, " 温度正常");     
//		//sprintf(str_buf2, " 等待更新...");
//		x = 0;
//		
//	  y = y + 16; 
//    u8g2_DrawUTF8(u8g2, x, y, (const char *)str_buf0);       //显示电压
//    y = y + 16; 
//    u8g2_DrawUTF8(u8g2, x, y, (const char *)str_buf1);       //显示容量

//		
//#else		
//	u8g2_SetFont(u8g2, u8g2_font_fur11_tr);
////		u8g2_SetFont(u8g2, u8g2_font_unifont_t_chinese3); 
//    sprintf(str_buf0, " temp : %.04f V", lcd_data->temp_val);
//    
//    if(lcd_data->temp_val > 29)
//      sprintf(str_buf1, " alarm");
//    else 
//      sprintf(str_buf1, " normal");     

//		x = 0;
//		
//	  y = y + 14; 
//    u8g2_DrawUTF8(u8g2, x, y, (const char *)str_buf0);       //显示电压
//    y = y + 14; 
//    u8g2_DrawUTF8(u8g2, x, y, (const char *)str_buf1);       //显示容量
//#endif


}
/*
  -----------?x
  |
  |
  ?
  y
*/
/*U8g2 hook*/
static uint8_t u8x8_byte_4wire_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int,
    void *arg_ptr)
{
    uint8_t *data = NULL;
  switch (msg)
  {
  case U8X8_MSG_BYTE_SEND:
        data = (uint8_t*)arg_ptr;
        do
        {
            //while(LL_SPI_IsActiveFlag_TXE(SPI1) == RESET);
						//HAL_SPI_IsActiveFlag_TXE
            //LL_SPI_TransmitData8(hspi1, *(data++));
						HAL_SPI_Transmit(&hspi1, (data++), 1, 0x00FF);
            //while(LL_SPI_IsActiveFlag_BSY(SPI1) == SET){}
            arg_int--;
        }while(arg_int > 0);
    break;
  case U8X8_MSG_BYTE_INIT:
    break;
  case U8X8_MSG_BYTE_SET_DC:
        HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, arg_int);
    break;
  case U8X8_MSG_BYTE_START_TRANSFER:
    break;
  case U8X8_MSG_BYTE_END_TRANSFER:
    break;
  default:
    return 0;
  }
  return 1;
}

//#define LCD_RES_Pin GPIO_PIN_0
//#define LCD_RES_GPIO_Port GPIOB
//#define LCD_DC_Pin GPIO_PIN_1
//#define LCD_DC_GPIO_Port GPIOB


static uint8_t u8x8_stm32_gpio_and_delay(U8X8_UNUSED u8x8_t *u8x8,
    U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int,
    U8X8_UNUSED void *arg_ptr)
{
  switch (msg)
  {
  case U8X8_MSG_GPIO_AND_DELAY_INIT:
    HAL_Delay(1);
//        LL_mDelay(1);
    break;
  case U8X8_MSG_DELAY_MILLI:
		HAL_Delay(arg_int);

    break;
    case U8X8_MSG_GPIO_CS:
        //GPIO_WriteBit(OLED_CS_GPIO_Port, OLED_CS_Pin, arg_int);
			HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, arg_int);	
  case U8X8_MSG_GPIO_DC:
			HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, arg_int);
        //GPIO_WriteBit(OLED_DC_GPIO_Port, OLED_DC_Pin, arg_int);
    break;
  case U8X8_MSG_GPIO_RESET:
			HAL_GPIO_WritePin(LCD_RES_GPIO_Port, LCD_RES_Pin, arg_int);
        //GPIO_WriteBit(OLED_RST_GPIO_Port, OLED_RST_Pin, arg_int);
    break;
  }
  return 1;
}