#include "ADC.h"

/**
 * @brief ADC初始化
 * @note 采用单次转换模式、单通道模式
 * @param adcn adc通道选择
 *              ADC_PA1
 *              ADC_PA2
 *              ADC_PC4
 *              ADC_PD2
 *              ADC_PD3
 *              ADC_PD4
 *              ADC_PD5
 *              ADC_PD6
 * @param sample 采样周期
 *              ADC_SAMPLE_3
 *              ADC_SAMPLE_9
 *              ADC_SAMPLE_15
 *              ADC_SAMPLE_30
 *              ADC_SAMPLE_43
 *              ADC_SAMPLE_57
 *              ADC_SAMPLE_73
 *              ADC_SAMPLE_241
 */

void CH32V_ADC_Init(ADC_Name adcn, uint32_t sample)
{
    ADC_InitTypeDef ADC_InitStructure;          //定义ADC结构体
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    if(adcn==ADC_PC4)RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    else RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);//ADC时钟设置
    if (adcn==ADC_PC4)
    {
        GPIO_InitTypeDef   GPIO_InitStructure = {0};
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
        GPIO_Init(GPIOC, &GPIO_InitStructure);
    }
    else if(adcn==ADC_PD2)
    {
        GPIO_InitTypeDef   GPIO_InitStructure = {0};
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
        GPIO_Init(GPIOD, &GPIO_InitStructure);
    }
    else if(adcn==ADC_PD3)
    {
        GPIO_InitTypeDef   GPIO_InitStructure = {0};
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
        GPIO_Init(GPIOD, &GPIO_InitStructure);
    }
    else if(adcn==ADC_PD4)
    {
        GPIO_InitTypeDef   GPIO_InitStructure = {0};
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
        GPIO_Init(GPIOD, &GPIO_InitStructure);
    }
    else if(adcn==ADC_PD5)
    {
        GPIO_InitTypeDef   GPIO_InitStructure = {0};
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
        GPIO_Init(GPIOD, &GPIO_InitStructure);
    }
    else if(adcn==ADC_PD6)
    {
        GPIO_InitTypeDef   GPIO_InitStructure = {0};
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
        GPIO_Init(GPIOD, &GPIO_InitStructure);
    }
    ADC1->SAMPTR2 = sample << (3 * adcn);

    ADC_DeInit(ADC1);                            //复位ADC1，重设为默认缺省值

    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                  //ADC工作模式:ADC1和ADC2工作在独立模式
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;                       //模数转换工作在单通道模式
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;                 //模数转换工作在单次转换模式
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //转换由软件而不是外部触发启动
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;              //ADC数据右对齐
    ADC_InitStructure.ADC_NbrOfChannel = 1;                             //顺序进行规则转换的ADC通道的数目
    ADC_Init(ADC1,&ADC_InitStructure);                                  //初始化ADC1

    ADC_Cmd(ADC1,ENABLE);                        //使能ADC1

    ADC_ResetCalibration(ADC1);                  //使能复位校准
    while(ADC_GetResetCalibrationStatus(ADC1));  //等待复位校准结束
    ADC_StartCalibration(ADC1);                  //开启AD校准
    while(ADC_GetCalibrationStatus(ADC1));       //等待校准结束
}

/**
 * @brief 读ADC值
 * @param adcn adc通道选择
 *              ADC_PA1
 *              ADC_PA2
 *              ADC_PC4
 *              ADC_PD2
 *              ADC_PD3
 *              ADC_PD4
 *              ADC_PD5
 *              ADC_PD6
 * @return 10位ADC值
 */
uint16_t ADC_Read(ADC_Name adcn)
{
    //配置ADCx，ADC通道，规则采样顺序，采样时间
    ADC_RegularChannelConfig(ADC1,adcn,1,ADC_SampleTime_241Cycles);
    ADC_SoftwareStartConvCmd(ADC1,ENABLE);          //使能ADC1的软件转换启动功能
    while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));   //等待转换结束
    return ADC_GetConversionValue(ADC1);            //返回最近一次ADC1规则组的转换结果
}
