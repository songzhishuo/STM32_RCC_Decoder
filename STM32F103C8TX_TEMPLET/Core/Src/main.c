/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd_display.h"

// #include "4_4KeyBoard.h"
#include "BPC.h"
#include "ds18b20.h"
//#include "simcom800.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

// static uint8_t min = 0;
// static uint8_t hour = 0;
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */




void key_scan();
void uart2_data_analyze();
void uart2_send_data(uint8_t num);
void alarm_server();
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  int8_t ret = -1;
  uint16_t system_tick = 0;
	uint32_t i = 0;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
	
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_SPI1_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
	DS18B20_Init();         //温度传感器初始化

	BPC_GPIO_INIT();
	
	HAL_TIM_Base_Start_IT(&htim1);					///开启定时器


	lcd_init(&display_data);	
  lcd_show();


  display_data.time_now.year = 2023;
  display_data.time_now.month = 2;
  display_data.time_now.day = 21;
  display_data.time_now.hour = 11;
  display_data.time_now.minute = 30;
  display_data.time_now.second = 00;
//	
//	memset(display_data.post_pin_num, '0', 4);

//	Key_Bord_Init();
	
	printf("hello \r\n");

/*
  1. BPC 获取时间
  2. 按键休眠 关闭屏幕
  3. LCD 显示 时分秒
  4. 闹钟设置 蜂鸣器
  5. 测量温度显示


*/
 /// display_data.get_time_flag = 1;     //测试标志


  display_data.temp_val = DS18B20_Get_Temp_f();
  
  HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, 1);
  
  while(!display_data.get_time_flag)      //没有获取到时间
  {
    BPC_DataHandle();
  }
  
  // display_data.get_time_flag = 0;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

		alarm_server();
		
		BPC_DataHandle();
		key_scan();

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    system_tick++;
		if(system_tick % 500 == 0)				///100毫秒处理一次
    {
      display_data.temp_val = DS18B20_Get_Temp_f();
			lcd_show();
     
    }
		
    if (system_tick >= 65535)
    {
      system_tick = 0;
    }
    ////HAL_Delay(1);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
/*用户添加的功能函数*/

void uart2_send_data(uint8_t num)
{
	uint8_t buf[3] = {0xAA, 0x00, 0xff};
	buf[1] = num;
	
	HAL_UART_Transmit(&huart2, buf, 3, 0xFFFF);
}

void uart2_data_analyze()
{
	if(uart2_rx_data.rx_flag == 1)			//接收到数据
	{
		if((uart2_rx_data.rx_buf[0] == 0xAA) && (uart2_rx_data.rx_buf[2] == 0xFF))		//校验头尾
		{
//			display_data.dis_num = uart2_rx_data.rx_buf[1];
		}
//		uart2_rx_data.rx_flag = 0;
	}
}

void key_scan()
{
#if 1
  if(HAL_GPIO_ReadPin(KEY0_GPIO_Port, KEY0_Pin) == GPIO_PIN_RESET)        //切换页面
  {
    HAL_Delay(5);
    //if(HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) == GPIO_PIN_RESET)
    {
      while(HAL_GPIO_ReadPin(KEY0_GPIO_Port, KEY0_Pin) == GPIO_PIN_RESET);
      
      if(display_data.sleep_mode == 1)     //第一页面
      {
        display_data.sleep_mode = 0;
      }
      else if(display_data.dis_index == 1)
      {
        if(display_data.dis_point_index % 4 == 0)     /*小时*/
        {
          display_data.time_set.hour --;
					if(display_data.time_set.hour <= 0)
					{
						display_data.time_set.hour = 0;
					}
        }
        if(display_data.dis_point_index % 4 == 1)     /*分钟*/
        {
          display_data.time_set.minute --;
					if(display_data.time_set.minute < 0)
					{
						display_data.time_set.minute = 0;
					}
        }
        if(display_data.dis_point_index % 4 == 2)     /*秒钟*/
        {
          display_data.time_set.second --;
					if(display_data.time_set.second < 0)
					{
						display_data.time_set.second = 0;
					}					
        }        
      }

    }
  }



  if(HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) == GPIO_PIN_RESET)
  {
    HAL_Delay(5);
    //if(HAL_GPIO_ReadPin(KEY3_GPIO_Port, KEY3_Pin) == GPIO_PIN_RESET)
    {
      while(HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) == GPIO_PIN_RESET);
      if(display_data.dis_index == 0)     //第一页面
      {
        if(display_data.dis_point_index % 4 == 3)     //休眠模式
        {
          display_data.sleep_mode = 1;
        }
      }
      else if(display_data.dis_index == 1)
      {
        if(display_data.dis_point_index % 4 == 0)     /*小时*/
        {
          display_data.time_set.hour ++;
					if(display_data.time_set.hour >= 60)
					{
						display_data.time_set.hour = 60;
					}
        }
        if(display_data.dis_point_index % 4 == 1)     /*分钟*/
        {
          display_data.time_set.minute ++;
					if(display_data.time_set.minute >= 60)
					{
						display_data.time_set.minute = 60;
					}
        }
        if(display_data.dis_point_index % 4 == 2)     /*秒钟*/
        {
          display_data.time_set.second ++;
					if(display_data.time_set.second >= 60)
					{
						display_data.time_set.second = 60;
					}
        }        
      }   

    }
  }


  if(HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin) == GPIO_PIN_RESET)
  {
    HAL_Delay(5);
    //if(HAL_GPIO_ReadPin(KEY3_GPIO_Port, KEY3_Pin) == GPIO_PIN_RESET)
    {
      while(HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin) == GPIO_PIN_RESET)
      {

        display_data.key0_press_cnt++;
        if(display_data.key0_press_cnt >= 300)
        {
          display_data.key0_press_cnt = 0;

          return ;
        }

      }

      display_data.dis_point_index++;

			
			//if()
      if(display_data.dis_point_index >= 8)
      {
        display_data.dis_point_index = 0;
      }

      display_data.dis_index = display_data.dis_point_index/4;			//计算显示的页面
    }
  }

#endif
}

/*报警服务*/
void alarm_server()
{
  if(display_data.time_now.hour ==  display_data.time_set.hour && \
    display_data.time_now.minute ==  display_data.time_set.minute && \
    display_data.time_now.second >= display_data.time_set.second  && \
    display_data.time_now.second < display_data.time_set.second + 6 && \
    display_data.dis_index != 1)        //触发报警
  {
      HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, 0);

  }
  else      ///关闭报警
  {
      HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, 1);

  }


}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
