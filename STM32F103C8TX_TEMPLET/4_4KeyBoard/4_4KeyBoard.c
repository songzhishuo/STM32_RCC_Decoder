#include "4_4KeyBoard.h"

#include "stm32f1xx_hal.h"

#define  KEY_DELAY  10  //中断去抖动间隔

volatile uint8_t  key;
volatile uint32_t  key_tic;


uint16_t normal_key_pin_map[4] = {
		GPIO_PIN_11,
		GPIO_PIN_12,
		GPIO_PIN_15,
		GPIO_PIN_3,
};

GPIO_TypeDef* normal_key_port_map[4] = {
		GPIOA,
		GPIOA,
		GPIOA,
		GPIOB,
};

static void Key_EXTI_Mode(void)// A4-7exti 0-3output
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  //__HAL_RCC_GPIOB_CLK_ENABLE();
  //__HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  //HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, GPIO_PIN_RESET);				//普通IO
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_15, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
	
  /*Configure GPIO pins : PA4 PA5 PA6 PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;					//中断管脚处理
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

//  /*Configure GPIO pins : PA0 PA1 PA2 PA3 */
//  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;					//普通管脚处理
//  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_3;					//普通管脚处理
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);	

  GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_15;					//普通管脚处理
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);	

	  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI4_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}

static void Key_Input_Mode(void)// A4-7output 0-3input
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

 
  /*Configure GPIO pins : PA4 PA5 PA6 PA7 */				//中断管脚处理
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

//  /*Configure GPIO pins : PA0 PA1 PA2 PA3 */				//普通管脚处理
//  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
//  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//  GPIO_InitStruct.Pull = GPIO_PULLUP;
//  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	
	GPIO_InitStruct.Pin = GPIO_PIN_3;					//普通管脚处理
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_15;					//普通管脚处理
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	
}

//static uint8_t Get_Key(uint8_t num)//按键两引脚值相加（引脚只用到了前八个所以只有低 八位）
//{
//	printf("key num:%02x\r\n", num);
//	switch(num)
//	{
//		case 0x2C:key='0';break;
//		case 0x15:key='1';break;
//		case 0x25:key='2';break;
//		case 0x45:key='3';break;
//		case 0x16:key='4';break;			'1','2','3','4','5','6','7','8','9','A','B','C','D','*','#'
//		case 0x26:key='5';break;
//		case 0x46:key='6';break;
//		case 0x14:key='7';break;
//		case 0x24:key='8';break;
//		case 0x44:key='9';break;
//		case 0x85:key='A';break;
//		case 0x86:key='B';break;
//		case 0x84:key='C';break;
//		case 0x8C:key='D';break;
//		case 0x1C:key='*';break;		
//		case 0x4C:key='#';break;	
//	}
//	return key;
//}

char key_map[] = {'N' ,'1','2','3','4','5','6','7','8','9','A','B','C','D','*','#','0' };

static uint8_t Get_Key(uint8_t num)//按键两引脚值相加（引脚只用到了前八个所以只有低 八位）
{
	printf("key num:%02x\r\n", num);
	switch(num)
	{
		case 0x15:key= 16 ;break;
		case 0x89:key= 1 ;break;
		case 0x85:key= 2 ;break;
		case 0x83:key= 3 ;break;
		case 0x49:key= 4 ;break;
		case 0x45:key= 5 ;break;
		case 0x43:key= 6 ;break;
		case 0x29:key= 7 ;break;
		case 0x25:key= 8 ;break;
		case 0x23:key= 9 ;break;
		case 0x81:key= 10 ;break;
		case 0x41:key= 11 ;break;
		case 0x21:key= 12 ;break;
		case 0x11:key= 13 ;break;
		case 0x19:key= 14 ;break;		
		case 0x13:key= 15 ;break;	
	}
	return key;
}
//非阻塞方式获取按键，返回0，没有键值，非0，获得键值
//该函数在中断中调用
uint8_t Get_KeyNum(uint16_t GPIO_Pin)
{
	uint8_t i=0,num=0;

	num = (uint8_t)GPIO_Pin;
	
	if(HAL_GetTick() - key_tic < KEY_DELAY) //中断间隔过小，放弃
	{
		key_tic = HAL_GetTick();
		return 0;
	}
	HAL_Delay(1);
	key_tic = HAL_GetTick();
	//略微延时后，读取管脚状态，去除上升沿抖动
	if( HAL_GPIO_ReadPin(GPIOB,GPIO_Pin) != GPIO_PIN_RESET ) 					//中断管脚处理
		return 0;
	
	if(num)
	{
		Key_Input_Mode();
		for(i=0;i<4;i++)
		{
			//if(HAL_GPIO_ReadPin(GPIOB,(1<<i)) == GPIO_PIN_RESET)				//普通管脚扫描
			if(HAL_GPIO_ReadPin(normal_key_port_map[i], normal_key_pin_map[i]) == GPIO_PIN_RESET)				//普通管脚扫描
					num |= (1<<i);
				
		}
		Key_EXTI_Mode();	
		return Get_Key(num);
	}
	else
	{
		Key_EXTI_Mode();	
		return 0;
	}
}

void Key_Bord_Init(void)
{
	Key_EXTI_Mode();
	key = 0;
	key_tic = HAL_GetTick();
}

