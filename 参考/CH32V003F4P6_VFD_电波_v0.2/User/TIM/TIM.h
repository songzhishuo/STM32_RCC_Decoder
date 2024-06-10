#ifndef _TIM_H_
#define _TIM_H_
#include "ch32v00x_rcc.h"
#include "ch32v00x_tim.h"

void BEEP_init(u16 arr, u16 psc, u16 ccp);
void TIM_ARR_updata(unsigned int arr,unsigned int psc);//�ı䶨ʱ��Ƶ��(PWM�������ı�����)


//========================================================================
// ����: TIM��ʼ��(��ʱ�������ж�).
// ����: ��ʱ��(TIM1/2)�����ڣ�us�����ص�����
// ����: none.
//========================================================================
void TIM_attachInterrupt(TIM_TypeDef* TIMx, uint32_t period, void (*userFunc)(void));
//TIM_Cmd(TIMx, ENABLE);//��ʼ�������жϺ������д˺���




//BEEP_init(499,479,249);//������200HZ,ռ�ձ�50%
//TIM_SetCompare1(TIM_TypeDef *TIMx, uint16_t Compare1);
//TIM_SetCompare1(TIM1,499);//���ڶ�ʱ��1��ͨ��1��ccp�Ĵ���Ϊ499
//TIM_PrescalerConfig(TIM_TypeDef *TIMx, uint16_t Prescaler, uint16_t TIM_PSCReloadMode);
//TIM_PrescalerConfig(TIM1,479,TIM_PSCReloadMode_Update);//���ڶ�ʱ��1�ķ�Ƶϵ��Ϊ479����һ���ж�ʱ����
//TIM_PrescalerConfig(TIM1,479,TIM_PSCReloadMode_Immediate);//���ڶ�ʱ��1�ķ�Ƶϵ��Ϊ479������������


#endif
