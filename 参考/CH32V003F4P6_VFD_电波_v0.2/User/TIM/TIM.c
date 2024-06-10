#include "TIM.h"

#define BEEP GPIO_Pin_2

/* PWM Output Mode Definition */
#define PWM_MODE1   0
#define PWM_MODE2   1

/* PWM Output Mode Selection */
//#define PWM_MODE PWM_MODE1
#define PWM_MODE PWM_MODE2

void BEEP_init(u16 arr, u16 psc, u16 ccp)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    TIM_OCInitTypeDef TIM_OCInitStructure = {0};
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_TIM1, ENABLE);

    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    TIM_TimeBaseInitStructure.TIM_Period = arr;
    TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);

#if (PWM_MODE == PWM_MODE1)
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;

#elif (PWM_MODE == PWM_MODE2)
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;

#endif

    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = ccp;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);

    TIM_CtrlPWMOutputs(TIM1, ENABLE);
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Disable);
    TIM_ARRPreloadConfig(TIM1, ENABLE);
    TIM_Cmd(TIM1, ENABLE);

}

void TIM_ARR_updata(unsigned int arr,unsigned int psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure = {0};

    TIM_TimeBaseInitStructure.TIM_Period = arr;
    TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);
}

typedef void (*voidFuncPtr)(void);
volatile static voidFuncPtr tim_fun[2] = { NULL };

//========================================================================
// ����: TIM��ʼ��.
// ����: ��ʱ��(TIM1/2)�����ڣ�us�����ص�����
// ����: none.
//========================================================================
void TIM_attachInterrupt(TIM_TypeDef* TIMx, uint32_t period, void (*userFunc)(void))
{
    // //��ʼ��TIM NVIC�������ж����ȼ�����
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

    //1.ʹ��ʱ�ӣ���¼�ص�����
    if(TIMx == TIM1)
    {
        tim_fun[0] = userFunc;
        RCC_APB2PeriphClockCmd( RCC_APB2Periph_TIM1, ENABLE );
        NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;           //TIM1�ж�
        TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE );                  //ʹ��TIM2�жϣ���������ж�
    }
    else if(TIMx == TIM2)
    {
        tim_fun[1] = userFunc;
        RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM2, ENABLE );
        NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;           //TIM2�ж�
        TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //ʹ��TIM2�жϣ���������ж�
    }

    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //������ռ���ȼ�0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        //������Ӧ���ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //ʹ��ͨ���ж�
    NVIC_Init(&NVIC_InitStructure); //��ʼ��NVIC

    //2.���ö�ʱ����
    if (period > 50000)
    {
        TIM_TimeBaseInitStructure.TIM_Prescaler = 4799;     //Ԥ��Ƶ������
        TIM_TimeBaseInitStructure.TIM_Period = period / 100-1;      //�趨�������Զ���װֵ
    }
    else
    {
        TIM_TimeBaseInitStructure.TIM_Prescaler = 47;       //Ԥ��Ƶ������
        TIM_TimeBaseInitStructure.TIM_Period = period-1;    //�趨�������Զ���װֵ
    }

    if(TIMx == TIM1)
    {
        TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    }

    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit( TIMx, &TIM_TimeBaseInitStructure);
}

void TIM1_UP_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void TIM1_UP_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)   //ȷ����TIM1�����ж�
    {
        TIM_ClearITPendingBit(TIM1,TIM_IT_Update);    //���ж�
        if(tim_fun[0]!=NULL)tim_fun[0]();
    }
}

void TIM2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void TIM2_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)       //ȷ����TIM2�����ж�
    {
        TIM_ClearITPendingBit(TIM2,TIM_IT_Update);    //����ж�
        if(tim_fun[1]!=NULL)tim_fun[1]();
    }
}

