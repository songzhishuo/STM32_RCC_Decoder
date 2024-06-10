#ifndef _TIM_H_
#define _TIM_H_
#include "ch32v00x_rcc.h"
#include "ch32v00x_tim.h"

void BEEP_init(u16 arr, u16 psc, u16 ccp);
void TIM_ARR_updata(unsigned int arr,unsigned int psc);//改变定时器频率(PWM蜂鸣器改变声音)


//========================================================================
// 描述: TIM初始化(定时器周期中断).
// 参数: 定时器(TIM1/2)、周期（us）、回调函数
// 返回: none.
//========================================================================
void TIM_attachInterrupt(TIM_TypeDef* TIMx, uint32_t period, void (*userFunc)(void));
//TIM_Cmd(TIMx, ENABLE);//初始化周期中断后请运行此函数




//BEEP_init(499,479,249);//蜂鸣器200HZ,占空比50%
//TIM_SetCompare1(TIM_TypeDef *TIMx, uint16_t Compare1);
//TIM_SetCompare1(TIM1,499);//调节定时器1的通道1的ccp寄存器为499
//TIM_PrescalerConfig(TIM_TypeDef *TIMx, uint16_t Prescaler, uint16_t TIM_PSCReloadMode);
//TIM_PrescalerConfig(TIM1,479,TIM_PSCReloadMode_Update);//调节定时器1的分频系数为479在下一次中断时更改
//TIM_PrescalerConfig(TIM1,479,TIM_PSCReloadMode_Immediate);//调节定时器1的分频系数为479现在立即更改


#endif
