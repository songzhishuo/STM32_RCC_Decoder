#include "ADC.h"

/**
 * @brief ADC��ʼ��
 * @note ���õ���ת��ģʽ����ͨ��ģʽ
 * @param adcn adcͨ��ѡ��
 *              ADC_PA1
 *              ADC_PA2
 *              ADC_PC4
 *              ADC_PD2
 *              ADC_PD3
 *              ADC_PD4
 *              ADC_PD5
 *              ADC_PD6
 * @param sample ��������
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
    ADC_InitTypeDef ADC_InitStructure;          //����ADC�ṹ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    if(adcn==ADC_PC4)RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    else RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);//ADCʱ������
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

    ADC_DeInit(ADC1);                            //��λADC1������ΪĬ��ȱʡֵ

    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                  //ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;                       //ģ��ת�������ڵ�ͨ��ģʽ
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;                 //ģ��ת�������ڵ���ת��ģʽ
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //ת��������������ⲿ��������
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;              //ADC�����Ҷ���
    ADC_InitStructure.ADC_NbrOfChannel = 1;                             //˳����й���ת����ADCͨ������Ŀ
    ADC_Init(ADC1,&ADC_InitStructure);                                  //��ʼ��ADC1

    ADC_Cmd(ADC1,ENABLE);                        //ʹ��ADC1

    ADC_ResetCalibration(ADC1);                  //ʹ�ܸ�λУ׼
    while(ADC_GetResetCalibrationStatus(ADC1));  //�ȴ���λУ׼����
    ADC_StartCalibration(ADC1);                  //����ADУ׼
    while(ADC_GetCalibrationStatus(ADC1));       //�ȴ�У׼����
}

/**
 * @brief ��ADCֵ
 * @param adcn adcͨ��ѡ��
 *              ADC_PA1
 *              ADC_PA2
 *              ADC_PC4
 *              ADC_PD2
 *              ADC_PD3
 *              ADC_PD4
 *              ADC_PD5
 *              ADC_PD6
 * @return 10λADCֵ
 */
uint16_t ADC_Read(ADC_Name adcn)
{
    //����ADCx��ADCͨ�����������˳�򣬲���ʱ��
    ADC_RegularChannelConfig(ADC1,adcn,1,ADC_SampleTime_241Cycles);
    ADC_SoftwareStartConvCmd(ADC1,ENABLE);          //ʹ��ADC1�����ת����������
    while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));   //�ȴ�ת������
    return ADC_GetConversionValue(ADC1);            //�������һ��ADC1�������ת�����
}
