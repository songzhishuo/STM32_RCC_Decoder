#ifndef _ADC_H_
#define _ADC_H_

#include "ch32v00x_gpio.h"
#include "ch32v00x_rcc.h"
#include "ch32v00x_adc.h"

#define  ADC_SAMPLE_3      ADC_SampleTime_3Cycles       //3��������
#define  ADC_SAMPLE_9      ADC_SampleTime_9Cycles       //9��������
#define  ADC_SAMPLE_15     ADC_SampleTime_15Cycles      //15��������
#define  ADC_SAMPLE_30     ADC_SampleTime_30Cycles      //30��������
#define  ADC_SAMPLE_43     ADC_SampleTime_43Cycles      //43��������
#define  ADC_SAMPLE_57     ADC_SampleTime_57Cycles      //57��������
#define  ADC_SAMPLE_73     ADC_SampleTime_73Cycles      //73��������
#define  ADC_SAMPLE_241    ADC_SampleTime_241Cycles     //241��������

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
