#include "BPC.h"


unsigned char Second=19;  //��ͬ���ź�
unsigned int  BPC_BUF[25];   //BPC���ջ���
unsigned char BPC_Time[3];   //BPDʱ��
unsigned char BPC_Date[4];   //BPC����
unsigned int  BPC_Con;         //BPC���ռ���
unsigned int  Total_time;     //�ܽ���ʱ��
unsigned int  HigTime;         //�ߵ�ƽ����
unsigned int  LowTime;         //�͵�ƽ����
unsigned char TimeFlag=0;        //0������ʱ�� 1����͵�ƽʱ�� 2����ߵ�ƽʱ��
unsigned char BPC_Step;        //����״̬����־
unsigned char BPC_RX_DONE;     //BPC������ɱ�־ 1������ɵȴ�����
unsigned char BPC_RX_CON;      //���ݽ��ռ���
unsigned char Afternoon;       //����־  1��� 0��ǰ
unsigned char Year_msb;       //����־  1��� 0��ǰ
unsigned char BPC_calibration_MOOD=0;//BPCУ׼ģʽ 0�ϸ�У׼ 1����У׼

void BPC_GPIO_INIT()
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_APB2PeriphClockCmd(BPC_PIN_IN_GPIO_D, ENABLE);

    GPIO_InitStructure.GPIO_Pin = BPC_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(BPC_GPIO, &GPIO_InitStructure);

}

void BPC_BufClr(void)
{
  u8 i=0;
  for(i=0;i<21;i++)//��4λ��Ҫ��� ��¼������һ�εĽ�����������ʱ �뵱ǰ����Ƚ� ��ͬ����Ϊ��Ϣ��Ч(������żУ�����ķ�֮һ���ʽ��յ�������Ϣ)
  {
    BPC_BUF[i]=0;
  }
}

//�����Ľ�����ת����2���ƺ��1�ĸ���
unsigned char NumOf1(unsigned char four_data)
{
  if(four_data==0)
    return 0;
  else if(four_data==1)
    return 1;
  else if(four_data==2)
    return 1;
  else if(four_data==3)
    return 2;
  else return 0;
}

//PCB �����У��
//
//�����������0.1 0.2 0.3  0.4�� �ֱ��Ӧ �Ľ�������� 0123
unsigned char  BPC_DataChange(void)
{
  unsigned char i=0;
  unsigned char result1=0,result2=0;   //����У��ɹ�����1  У��ʧ�ܷ���0
  unsigned char num=0;   //1�ĸ���
  unsigned char p3_check=0; //P3У���־
  unsigned char p4_check=0; //P4У���־
  for(i=0;i<20;i++)    //����ת��
  {
    if((BPC_BUF[i]>=50)&&(BPC_BUF[i]<=150))
    {
      BPC_BUF[i]=0;
    }
    else if((BPC_BUF[i]>=150)&&(BPC_BUF[i]<=250))
    {
      BPC_BUF[i]=1;
    }
    else if((BPC_BUF[i]>=250)&&(BPC_BUF[i]<=350))
    {
      BPC_BUF[i]=2;
    }
    else if((BPC_BUF[i]>=350)&&(BPC_BUF[i]<=450))
    {
      BPC_BUF[i]=3;
    }
    else
    {
      BPC_BUF[i]=0;
    }
  }

/*--------------------P1 P2ʱ���� ����У��-----------------------------*/
    p3_check=0;
    num=0;
    for(i=1;i<10;i++)
    {
      num+=NumOf1(BPC_BUF[i]);
    }

    if((BPC_BUF[10]==0)||(BPC_BUF[10]==2))   //ż��У��
    {
      if(num%2==0)  //ż��
        p3_check=1;
    }
    else if((BPC_BUF[10]==1)||(BPC_BUF[10]==3)) //����У��
    {
      if(num%2==1)
        p3_check=1;
    }

    if(p3_check)   //ʱ��У����ȷ
    {
      if((BPC_BUF[10]==0)||(BPC_BUF[10]==1))  //��ǰ
        Afternoon=0;
      else
        Afternoon=1;  //���
      result1=1;
    }
/*-----------------����У��-------------------------------------------*/
    p4_check=0;
    num=0;
    for(i=11;i<19;i++)
    {
      num+=NumOf1(BPC_BUF[i]);
    }

    if((BPC_BUF[19]==0)||(BPC_BUF[19]==2))   //ż��У��
    {
      if(num%2==0)  //ż��
        p4_check=1;
    }
    else if((BPC_BUF[19]==1)||(BPC_BUF[19]==3)) //����У��
    {
      if(num%2==1)
        p4_check=1;
    }

    if(p4_check && result1)   //ʱ��&����У����ȷ
    {
      if((BPC_BUF[19]==0)||(BPC_BUF[19]==1))  //�����λΪ0
        Year_msb=0;
      else
        Year_msb=1;  //�����λΪ1
      result2=1;
    }
  return result2;
}

//����BPCʱ��/������DS1302
void BPC_DataHandle(void)
{
  if(BPC_RX_DONE==1)       //һ֡���ݽ������֮��
  {
    if(BPC_DataChange())  //����У����ȷ
    {
        if(Afternoon)  //����
        {
          BPC_Time[0]=4*BPC_BUF[3]+BPC_BUF[4]+12;//ʱ
          if(BPC_Time[0]==24)BPC_Time[0]=12;
        }
        else    //����
          BPC_Time[0]=4*BPC_BUF[3]+BPC_BUF[4];//ʱ

        BPC_Time[1]=16*BPC_BUF[5]+4*BPC_BUF[6]+BPC_BUF[7];   //����
        BPC_Time[2]=Second+20*BPC_BUF[1];                     //��

        if(Year_msb==1)  //+64��
            BPC_Date[0]=16*BPC_BUF[16]+4*BPC_BUF[17]+BPC_BUF[18]+64;   //��
        else    //��+64��
            BPC_Date[0]=16*BPC_BUF[16]+4*BPC_BUF[17]+BPC_BUF[18];   //��

        BPC_Date[1]=4*BPC_BUF[14]+BPC_BUF[15];   //��
        BPC_Date[2]=16*BPC_BUF[11]+4*BPC_BUF[12]+BPC_BUF[13];   //��
        BPC_Date[3]=4*BPC_BUF[8]+BPC_BUF[9];   //����

        if(BPC_calibration_MOOD==0&&BPC_Date[0]==BPC_BUF[21]&&BPC_Date[1]==BPC_BUF[22]&&BPC_Date[2]==BPC_BUF[23]&&BPC_Time[0]==BPC_BUF[24])//�������ݴ�����ͬ
        {
           // ds1302_time_set(BPC_Date[0],BPC_Date[1],BPC_Date[2],BPC_Time[0],BPC_Time[1],BPC_Time[2],BPC_Date[3]);
            BPC_Con++;
        }
        else if(BPC_calibration_MOOD)//����ģʽ
        {
           // ds1302_time_set(BPC_Date[0],BPC_Date[1],BPC_Date[2],BPC_Time[0],BPC_Time[1],BPC_Time[2],BPC_Date[3]);
            BPC_Con++;
        }
        BPC_BUF[21]=BPC_Date[0];
        BPC_BUF[22]=BPC_Date[1];
        BPC_BUF[23]=BPC_Date[2];
        BPC_BUF[24]=BPC_Time[0];

           printf("---\r\n");
           printf("")
    }
    BPC_BufClr();  //�������
    BPC_RX_DONE=0;
  }
}

void BPC_RX()
{
    if(BPC_RX_DONE==0)
    {
        Total_time++;
        if(BPC_IN_PIN)
        {
            HigTime++;
            LowTime=0;
            TimeFlag=1;
        }
        else
        {
            LowTime++;
            TimeFlag=2;
        }
        if(LowTime>1400 && TimeFlag==2)//֡��ʼ
        {
            BPC_RX_CON=1;
            HigTime=0;
            BPC_Step=1;//��ʼ����
            Total_time=0;
            if(LowTime>2500)//ԶԶ����ʱ�� ��Ч����
            {
                BPC_RX_CON=0;
                LowTime=0;
                HigTime=0;
                BPC_Step=0;//ֹͣ����
                Total_time=0;
                BPC_BufClr();
            }
        }
        if(BPC_Step && TimeFlag==2 && HigTime)
        {
            BPC_BUF[BPC_RX_CON]=HigTime;
            BPC_RX_CON++;
            HigTime=0;
            if(BPC_RX_CON>19)
            {
                if(Total_time>18000&&Total_time<22000&&BPC_calibration_MOOD==0)//�ϸ�ģʽ
                {
                    BPC_RX_DONE=1;

                    BPC_RX_CON=0;
                    LowTime=0;
                    HigTime=0;
                    BPC_Step=0;//ֹͣ����
                }
                else if(BPC_calibration_MOOD)//����ģʽ
                {
                    BPC_RX_DONE=1;

                    BPC_RX_CON=0;
                    LowTime=0;
                    HigTime=0;
                    BPC_Step=0;//ֹͣ����
                }
                else
                {
                    BPC_RX_CON=0;
                    LowTime=0;
                    HigTime=0;
                    BPC_Step=0;//ֹͣ����
                    BPC_BufClr();  //�������
                }
            }
        }
    }
}

