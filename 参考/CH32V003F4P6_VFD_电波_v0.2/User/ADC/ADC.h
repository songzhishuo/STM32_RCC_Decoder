#ifndef _ADC_H_
#define _ADC_H_

#include "ch32v00x_gpio.h"
#include "ch32v00x_rcc.h"
#include "ch32v00x_adc.h"

#define  ADC_SAMPLE_3      ADC_SampleTime_3Cycles       //3采样周期
#define  ADC_SAMPLE_9      ADC_SampleTime_9Cycles       //9采样周期
#define  ADC_SAMPLE_15     ADC_SampleTime_15Cycles      //15采样周期
#define  ADC_SAMPLE_30     ADC_SampleTime_30Cycles      //30采样周期
#define  ADC_SAMPLE_43     ADC_SampleTime_43Cycles      //43采样周期
#define  ADC_SAMPLE_57     ADC_SampleTime_57Cycles      //57采样周期
#define  ADC_SAMPLE_73     ADC_SampleTime_73Cycles      //73采样周期
#define  ADC_SAMPLE_241    ADC_SampleTime_241Cycles     //241采样周期

typedef enum
{
    ADC_PA2 = 0,
    ADC_PA1,
    ADC_PC4,
    ADC_PD2,
    ADC_PD3,
    ADC_PD5,
    ADC_PD6,
    ADC_PD4
} ADC_Name;


void CH32V_ADC_Init(ADC_Name adcn, uint32_t sample);
uint16_t ADC_Read(ADC_Name adcn);
#endif
