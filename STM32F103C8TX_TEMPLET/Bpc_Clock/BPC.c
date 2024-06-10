#include "BPC.h"
#include "lcd_display.h"

static void time_run_server();


uint32_t TIM2_time_count; // 定时器计数

unsigned char Second = 19;              // 秒同步信号
unsigned int BPC_BUF[25];               // BPC接收缓存
unsigned char BPC_Time[3];              // BPD时间
unsigned char BPC_Date[4];              // BPC日期
unsigned int BPC_Con;                   // BPC接收计数
unsigned int Total_time;                // 总接收时间
unsigned int HigTime;                   // 高电平计数
unsigned int LowTime;                   // 低电平计数
unsigned char TimeFlag = 0;             // 0不计算时间 1计算低电平时间 2计算高电平时间
unsigned char BPC_Step;                 // 接收状态机标志
unsigned char BPC_RX_DONE;              // BPC接收完成标志 1接收完成等待解析
unsigned char BPC_RX_CON;               // 数据接收计数
unsigned char Afternoon;                // 午后标志  1午后 0午前
unsigned char Year_msb;                 // 午后标志  1午后 0午前
unsigned char BPC_calibration_MOOD = 0; // BPC校准模式 0严格校准 1宽松校准

void BPC_GPIO_INIT()
{
  //    GPIO_InitTypeDef GPIO_InitStructure = {0};

  //    RCC_APB2PeriphClockCmd(BPC_PIN_IN_GPIO_D, ENABLE);

  //    GPIO_InitStructure.GPIO_Pin = BPC_Pin;
  //    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
  //    GPIO_Init(BPC_GPIO, &GPIO_InitStructure);

  //	    GPIO_InitTypeDef GPIO_InitStruct = {0};
  //  //  GPIO_InitStruct.Pin = BPC_Pin;
  //    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  //    GPIO_InitStruct.Pull = GPIO_NOPULL;
  //    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  //  //  HAL_GPIO_Init(BPC_PIN_IN_GPIO_D, &GPIO_InitStruct);
  GPIO_InitTypeDef GPIO_InitStruct = {0}; ////输入模式
  GPIO_InitStruct.Pin = BPC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BPC_GPIO, &GPIO_InitStruct);
}

void BPC_BufClr(void)
{
  uint8_t i = 0;
  for (i = 0; i < 21; i++) // 后4位不要清除 记录的是上一次的解码后的年月日时 与当前解码比较 相同则认为信息有效(仅靠奇偶校验有四分之一概率接收到错误信息)
  {
    BPC_BUF[i] = 0;
  }
}

// 计算四进制数转换成2进制后的1的个数
unsigned char NumOf1(unsigned char four_data)
{
  if (four_data == 0)
    return 0;
  else if (four_data == 1)
    return 1;
  else if (four_data == 2)
    return 1;
  else if (four_data == 3)
    return 2;
  else
    return 0;
}

// PCB 解码加校验
//
// 方波宽度量化0.1 0.2 0.3  0.4秒 分别对应 四进制里面的 0123
unsigned char BPC_DataChange(void)
{
  unsigned char i = 0;
  unsigned char result1 = 0, result2 = 0; // 数据校验成功返回1  校验失败返回0
  unsigned char num = 0;                  // 1的个数
  unsigned char p3_check = 0;             // P3校验标志
  unsigned char p4_check = 0;             // P4校验标志
  for (i = 0; i < 20; i++)                // 数据转换
  {
    if ((BPC_BUF[i] >= 50) && (BPC_BUF[i] <= 150))
    {
      BPC_BUF[i] = 0;
    }
    else if ((BPC_BUF[i] >= 150) && (BPC_BUF[i] <= 250))
    {
      BPC_BUF[i] = 1;
    }
    else if ((BPC_BUF[i] >= 250) && (BPC_BUF[i] <= 350))
    {
      BPC_BUF[i] = 2;
    }
    else if ((BPC_BUF[i] >= 350) && (BPC_BUF[i] <= 450))
    {
      BPC_BUF[i] = 3;
    }
    else
    {
      BPC_BUF[i] = 0;
    }
  }

  /*--------------------P1 P2时分秒 星期校验-----------------------------*/
  p3_check = 0;
  num = 0;
  for (i = 1; i < 10; i++)
  {
    num += NumOf1(BPC_BUF[i]);
  }

  if ((BPC_BUF[10] == 0) || (BPC_BUF[10] == 2)) // 偶数校验
  {
    if (num % 2 == 0) // 偶数
      p3_check = 1;
  }
  else if ((BPC_BUF[10] == 1) || (BPC_BUF[10] == 3)) // 奇数校验
  {
    if (num % 2 == 1)
      p3_check = 1;
  }

  if (p3_check) // 时间校验正确
  {
    if ((BPC_BUF[10] == 0) || (BPC_BUF[10] == 1)) // 午前
      Afternoon = 0;
    else
      Afternoon = 1; // 午后
    result1 = 1;
  }
  /*-----------------日期校验-------------------------------------------*/
  p4_check = 0;
  num = 0;
  for (i = 11; i < 19; i++)
  {
    num += NumOf1(BPC_BUF[i]);
  }

  if ((BPC_BUF[19] == 0) || (BPC_BUF[19] == 2)) // 偶数校验
  {
    if (num % 2 == 0) // 偶数
      p4_check = 1;
  }
  else if ((BPC_BUF[19] == 1) || (BPC_BUF[19] == 3)) // 奇数校验
  {
    if (num % 2 == 1)
      p4_check = 1;
  }

  if (p4_check && result1) // 时间&日期校验正确
  {
    if ((BPC_BUF[19] == 0) || (BPC_BUF[19] == 1)) // 年最高位为0
      Year_msb = 0;
    else
      Year_msb = 1; // 年最高位为1
    result2 = 1;
  }
  return result2;
}

// 解析BPC时间/并设置DS1302
void BPC_DataHandle(void)
{
  if (1 == BPC_RX_DONE) // 一帧数据接收完成之后
  {
    if (BPC_DataChange()) // 数据校验正确
    {
      if (Afternoon) // 下午
      {
        BPC_Time[0] = 4 * BPC_BUF[3] + BPC_BUF[4] + 12; // 时
        if (BPC_Time[0] == 24)
          BPC_Time[0] = 12;
      }
      else                                         // 上午
        BPC_Time[0] = 4 * BPC_BUF[3] + BPC_BUF[4]; // 时

      BPC_Time[1] = 16 * BPC_BUF[5] + 4 * BPC_BUF[6] + BPC_BUF[7]; // 分钟
      BPC_Time[2] = Second + 20 * BPC_BUF[1];                      // 秒

      if (Year_msb == 1)                                                     //+64年
        BPC_Date[0] = 16 * BPC_BUF[16] + 4 * BPC_BUF[17] + BPC_BUF[18] + 64; // 年
      else                                                                   // 不+64年
        BPC_Date[0] = 16 * BPC_BUF[16] + 4 * BPC_BUF[17] + BPC_BUF[18];      // 年

      BPC_Date[1] = 4 * BPC_BUF[14] + BPC_BUF[15];                    // 月
      BPC_Date[2] = 16 * BPC_BUF[11] + 4 * BPC_BUF[12] + BPC_BUF[13]; // 日
      BPC_Date[3] = 4 * BPC_BUF[8] + BPC_BUF[9];                      // 星期

      if (BPC_calibration_MOOD == 0 && BPC_Date[0] == BPC_BUF[21] && BPC_Date[1] == BPC_BUF[22] && BPC_Date[2] == BPC_BUF[23] && BPC_Time[0] == BPC_BUF[24]) // 两次数据大致相同
      {
        // ds1302_time_set(BPC_Date[0],BPC_Date[1],BPC_Date[2],BPC_Time[0],BPC_Time[1],BPC_Time[2],BPC_Date[3]);
        BPC_Con++;
      }
      else if (BPC_calibration_MOOD) // 宽松模式
      {
        // ds1302_time_set(BPC_Date[0],BPC_Date[1],BPC_Date[2],BPC_Time[0],BPC_Time[1],BPC_Time[2],BPC_Date[3]);
        BPC_Con++;
      }
      BPC_BUF[21] = BPC_Date[0];
      BPC_BUF[22] = BPC_Date[1];
      BPC_BUF[23] = BPC_Date[2];
      BPC_BUF[24] = BPC_Time[0];

      printf("---\r\n");
      printf("");
      printf("year :20%02d-%02d-%02d   %02d\r\n", BPC_Date[0], BPC_Date[1], BPC_Date[2], BPC_Date[3]);
      printf("time: %02d:%02d:%02d\r\n", BPC_Time[0], BPC_Time[1], BPC_Time[2]);

      display_data.get_time_flag = 1;
      display_data.time_now.year = 2000 + BPC_Date[0];
      display_data.time_now.month = BPC_Date[1];
      display_data.time_now.day = BPC_Date[2];
      display_data.time_now.week = BPC_Date[3];

      display_data.time_now.hour = BPC_Time[0];
      display_data.time_now.minute = BPC_Time[1];
      display_data.time_now.second = BPC_Time[2];
    }
    BPC_BufClr(); // 数据清空
    BPC_RX_DONE = 0;
  }
}

void BPC_RX()
{
  if (BPC_RX_DONE == 0)
  {
    Total_time++;
    if (BPC_IN_PIN)
    {
      HigTime++;
      LowTime = 0;
      TimeFlag = 1;
    }
    else
    {
      LowTime++;
      TimeFlag = 2;
    }
    if (LowTime > 1400 && TimeFlag == 2) // 帧起始
    {
      BPC_RX_CON = 1;
      HigTime = 0;
      BPC_Step = 1; // 开始接收
      Total_time = 0;
      if (LowTime > 2500) // 远远超出时间 无效数据
      {
        BPC_RX_CON = 0;
        LowTime = 0;
        HigTime = 0;
        BPC_Step = 0; // 停止接收
        Total_time = 0;
        BPC_BufClr();
      }
    }
    if (BPC_Step && TimeFlag == 2 && HigTime)
    {
      BPC_BUF[BPC_RX_CON] = HigTime;
      BPC_RX_CON++;
      HigTime = 0;
      if (BPC_RX_CON > 19)
      {
        if (Total_time > 18000 && Total_time < 22000 && BPC_calibration_MOOD == 0) // 严格模式
        {
          BPC_RX_DONE = 1;

          BPC_RX_CON = 0;
          LowTime = 0;
          HigTime = 0;
          BPC_Step = 0; // 停止接收
        }
        else if (BPC_calibration_MOOD) // 宽松模式
        {
          BPC_RX_DONE = 1;

          BPC_RX_CON = 0;
          LowTime = 0;
          HigTime = 0;
          BPC_Step = 0; // 停止接收
        }
        else
        {
          BPC_RX_CON = 0;
          LowTime = 0;
          HigTime = 0;
          BPC_Step = 0; // 停止接收
          BPC_BufClr(); // 数据清空
        }
      }
    }
  }
}

/**
 * @brief 1ms走一帧
 * 
 */
void BPC_TIMER_IQR()
{
  // HAL_GPIO_TogglePin(BLINK_GPIO_Port, BLINK_Pin);						//中断输出测试 频率2KHZ
  time_run_server();
  BPC_RX(); // 必须是1ms定时器 根据进入此函数次数判断高电平持续时间
  // TIM2_time_count++;
  // if (TIM2_time_count % 500 == 0)
  // {
  // }

  // if (TIM2_time_count > 28000)
  //   TIM2_time_count = 0;
}

/**
 * @brief 时钟走时服务
 * 
 */
static void time_run_server()
{
  static uint16_t time_cnt = 0;

  if(display_data.get_time_flag == 1)     /*获取到时间*/
  {
    time_cnt++;

    if(time_cnt >= 1000)
    {
      time_cnt = 0;
      display_data.time_now.second ++;
      if(display_data.time_now.second >= 60)    /*60 秒*/
      {
        display_data.time_now.second = 0;
        display_data.time_now.minute ++;

        if(display_data.time_now.minute >= 60)  /*60 分钟*/
        {
          display_data.time_now.minute = 0;

          display_data.time_now.hour++;

          if(display_data.time_now.hour >= 24)
          {
            display_data.time_now.hour = 0;

            display_data.time_now.day ++;
          }
        }
      }
    }

  }


}