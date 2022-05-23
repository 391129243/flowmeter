#include "msp430x54x.h"
#include "functions.h"
#include "timerB0.h"
#include "params.h"
#include "string.h"
#include "ir.h"
#include "timerA0.h"
#include "timerA1.h"

unsigned char IR_Rec_First_Flag = 0;  //��⵽�����������½��ر�־
unsigned char IR_Rec_Start_Flag = 0;  //������������ʼ��־
unsigned char IR_Rec_PulseIndex = 0; //������յ��ڼ���λ��־
//���������
unsigned int IR_Rec_Code = 0;//���յ��ĺ����ֽ���

//�����Դ����
void powerOnIR(void)
{
  IR_POWER_ON;
  delay_ms(5);
  IR_INT_ENABLE;
  Reset_IR_Param();
}

//�����Դ�ر�
void powerCloseIR(void)
{
  Reset_IR_Param();
  
  IR_POWER_OFF;
  IR_INT_DISABLE;
  TB0CTL &= ~TASSEL1;                           // ACLKΪʱ��Դ,32768
  TB0CTL |= TASSEL0; 
}

//�����ʼ��
void initIR(void)
{
  IR_INT_SIGN_DOWN;
  IR_INT_DISABLE;
  IR_CTL_INPUT;
  IR_POWER_CTL_OUTPUT;
  powerCloseIR();
  //powerOnIR();
  
  P4SEL &= ~BIT0;                              
  P4DIR &= ~BIT0; 
  
  TB0CTL &= ~(CNTL0 + CNTL1);                   //16TBR
  TB0CTL |= TBCLR;                              //���������� 
  TB0CTL &= ~TASSEL1;                           // ACLKΪʱ��Դ,32768
  TB0CTL |= TASSEL0; 
  TB0CTL &= ~(ID0 + ID1);                       //ʱ��Դ����Ƶ,8MHz
  TB0CTL |= TBCLR;                              //����������    
  TB0CTL &= ~MC1;                             // ��ʱ������ģʽΪ��ģʽ
  TB0CTL |= MC0; 
  
  TB0CCTL0 |= CCIE;
  TB0CCR0 = 0;                                //��ʱ��TBֹͣ
}

//����������������ش���
void IRRecPulseDown(void)
{
  if(!IR_Rec_First_Flag)//������ʼ����⵽�����������½���
  {
    IR_Rec_First_Flag = 1;
    TB0CCR0 = 1718;  //��ʱ��TB���¿�������ʱ���պ������ݵİ��λ��ʱ��
  }
}

//����������
void IRDecodeError(void)
{ 
  IR_Rec_First_Flag = 0;
  IR_Rec_Start_Flag = 0;
  IR_Rec_PulseIndex = 0;
  IR_Rec_Code = 0;
}

//��ʼ����ʱ��TB0ר�������������������ղ�����2400
void Init_TimerB0_IRRecive(void)
{
  P4SEL &= ~BIT0;  //������Ϊ�ⲿ�ź��½����ж�����                            
  P4DIR &= ~BIT0; 
  
  TB0CTL &= ~TASSEL0;                           // MCLKΪʱ��Դ,8M
  TB0CTL |= TASSEL1; 
  TB0CCTL0 |= CCIE;  //����ʱ��TB0�ж�
  TB0CCTL0 &= ~CCIFG;  //��CCR0�жϱ�־
  TB0CCR0 = 0;    //��ʱ��TB0ֹͣ
}

//��ͣ��ʱ��TB
void Stop_TimerB0(void)                         
{
  TB0CCR0 = 0;
  TB0CCR1 = 0;
}

//�����Ǻ��ⷢ����
unsigned char IR_Send_First_Flag = 0;  //���������뷢��λ��־
unsigned char IR_Send_Start_Flag = 0;  //��������λ���Ϳ�ʼ��־
unsigned char IR_Send_PulseIndex = 0;  //���ⷢ�͵��ڼ���λ��־

unsigned char IR_Send_Data_Count = 0;//���ⷢ�͵��ڼ����ֽڱ�־
unsigned char IR_Send_Count_38KHz = 0;//38KHzPWM������
unsigned char IR_Send_OneByte_OK_Flag = 0;//һ���ֽڷ�����ɱ�־
unsigned char IR_Sending_Byte = 0;//��ǰ���ڷ��͵�һ���ֽں���������

unsigned int IR_SendOneByte_Delay10ms_Count = 0;//���ϻ�����ÿ������һ���ֽ�������Ҫ��ʱ10ms,���ܷ�����һ���ֽ�����

//��ʼ����ʱ��TB0ר�������ⷢ�䣬���ⷢ�Ͳ�����2400
void Init_TimerB0_IRSend(void)
{   
    P4SEL |= BIT0;                              //��P4.0���
    P4DIR |= BIT0;  

    TB0CTL &= ~(CNTL0 + CNTL1);                   //16TBR 
    TB0CTL |= TBCLR;                              //���������� 
    TB0CTL |= TASSEL1;                           // SCLKΪʱ��Դ8MHz
    TB0CTL &= ~TASSEL0; 
    TB0CTL &= ~(ID0 + ID1);                       //ʱ��Դ����Ƶ8MHz
    TB0CTL |= MC0;                             // ��ʱ����ģʽ
    TB0CTL &= ~MC1;

    TB0CCR0 = 238;                                //��ʱ��TBֹͣ ��38KHz�ز�   
    TB0CCTL0 |= CCIE;  
    TB0CCTL0 &= ~CCIFG;                         //��CCR0�жϱ�־
      
    
    TB0CCR1 = 119;  
    TB0CCTL1 &= ~CAP;                            //�Ƚ�
}


//��ʼ���ⷢ��ģʽ
void Start_IRSend_PWM(void)
{
  _NOP();
  _NOP();
  
  TB0CCR0 = 0;//��ͣ��ʱ��TB0
  TB0CCR1 = 0;
  Init_TimerB0_IRSend();//���ⷢ�ͳ�ʼ��
  g_irReceiveOrSendFlag = 1;//����ת����ģʽ
  
  IR_Send_OneByte_OK_Flag = 1;
  IR_Send_Data_Count = 0;
  IR_Sending_Byte = g_irSendBuffer[IR_Send_Data_Count];//IR���͵ĵ�һ���ֽ�
}


//�������ⷢ��ģʽ��תΪ���������ģʽ
void Stop_IRSend_PWM(void)
{
  TB0CCR0 = 0;//��ͣ��ʱ��TB0
  TB0CCR1 = 0;
  
  //IR_Send_OneByte_OK_Flag = 0;
  //IR_Send_Data_Count = 0;
  g_irSendByteNumberCount = 0;
  memset(g_irSendBuffer,0,IR_MAX_SEND_BUFFER);//���ⷢ�ͻ��������
  Init_TimerB0_IRRecive();//������ճ�ʼ��
  g_irReceiveOrSendFlag = 0;//����ת����ģʽ
  
  _NOP();
  _NOP();
}

//�򿪺��ⷢ��PWM����
void Open_IRSend_PWM(void)
{
  TB0CCTL1 |= OUTMOD0;   //�Ƚ����¿���
  TB0CCTL1 |= OUTMOD1;
  TB0CCTL1 |= OUTMOD2;
}

//�رպ��ⷢ��PWM����
void Close_IRSend_PWM(void)
{
  TB0CCTL1 &= ~OUTMOD0;   //�ȽϹر�
  TB0CCTL1 &= ~OUTMOD1;
  TB0CCTL1 &= ~OUTMOD2;
}

//���ⷢ�ʹ���
void IRSendError(void)
{
  IR_Send_First_Flag = 0;
  IR_Send_Start_Flag = 0;
  IR_Send_PulseIndex = 0;
}

//���������λ
void Reset_IR_Param(void)
{
  Init_TimerB0_IRRecive();
  g_irReceiveOrSendFlag = 0;//����ת����ģʽ
  g_irReceiveByteOkFlag = 1;
  
  IR_Rec_First_Flag = 0;
  IR_Rec_Start_Flag = 0;
  IR_Rec_PulseIndex = 0;
  IR_Rec_Code = 0;
  memset(g_irReceiveBuffer,0,IR_MAX_RECEIVE_BUFFER);//������ջ��������
  g_irReceiveByteNumberCount = 0;
  
  IR_Send_First_Flag = 0;
  IR_Send_Start_Flag = 0;
  IR_Send_PulseIndex = 0;
  IR_Send_Data_Count = 0;//���ⷢ�͵��ڼ����ֽڱ�־
  IR_Send_Count_38KHz = 0;//38KHzPWM������
  IR_Send_OneByte_OK_Flag = 1;//һ���ֽڷ�����ɱ�־
  IR_Sending_Byte = 0;//��ǰ���ڷ��͵�һ���ֽں���������
  IR_SendOneByte_Delay10ms_Count = 0;//���ϻ�����ÿ������һ���ֽ�������Ҫ��ʱ10ms,���ܷ�����һ���ֽ�����
  memset(g_irSendBuffer,0,IR_MAX_SEND_BUFFER);//���ⷢ�ͻ��������
  g_irSendByteNumberCount = 0;  
  TB0CCR1 = 0;
}

//���ⷢ��һ���ֽ�
unsigned char IR_Send_Byte(void)
{
  if(!IR_Send_First_Flag)//�����ֽ���ʼλ���͵�ƽ
  {
    if(!IR_Send_Count_38KHz)//��ʼ������ʼλ����PWM
    {
      Open_IRSend_PWM();//�����ⷢ��PWM
    }
    IR_Send_Count_38KHz++;
    if(IR_Send_Count_38KHz >= 14)//14��38KHz���壬�պ��Ǻ��ⷢ��һλ��ʱ��
    {
      IR_Send_Count_38KHz = 0;
      IR_Send_First_Flag = 1;
    }
  }
  else//���������ֽ�8λ
  {
    if(!IR_Send_Start_Flag) //�ж�һλ�Ƿ������
    {
      unsigned char temp = IR_Sending_Byte & 0x01;
      if(temp)//��λΪ1
      {
        Close_IRSend_PWM();//�غ��ⷢ��PWM
      }
      else//��λΪ0
      {
        Open_IRSend_PWM();//�����ⷢ��PWM
      }
      IR_Sending_Byte >>= 1; //���͵��ֽ�����1λ     
      IR_Send_Start_Flag = 1;
    }
    IR_Send_Count_38KHz++;
    if(IR_Send_Count_38KHz >= 14)//һλ�������
    {
      IR_Send_Count_38KHz = 0;
      IR_Send_Start_Flag = 0;//һλ������ɱ�־
      IR_Send_PulseIndex ++;
      if(IR_Send_PulseIndex >= 8)//ֹͣλ�������
      {
        //״̬��λ
        IRSendError();
        Close_IRSend_PWM();//�غ��ⷢ��PWM
        return 1;
      }
    }  
  }
  return 0;
}


//Timer0_B0 �жϷ������
#pragma vector=TIMERB0_VECTOR
__interrupt void Timer0_B0 (void) //TB0CCR0
{
  if(!g_irReceiveOrSendFlag)//IR_Rec
  {
    if(IR_Rec_First_Flag)
    {
      if(!IR_Rec_Start_Flag) //�жϽ����Ƿ��Ѿ���ʼ
      {
        if((IR_IN_PORT & IR_PIN_NUM) != IR_PIN_NUM) //IR_IN==0,������ʼλ
        {
          TB0CCR0 = 3436;  //��ʱ��TB���¿�������ʱ���պ������ݵ�һ��λ��ʱ��
          IR_Rec_Start_Flag = 1;
        }
        else//�����ź�
        {
          g_irReceiveByteOkFlag = 1;
          IRDecodeError();
          Stop_TimerB0();//ͣ��ʱ��
          return; 
        }
      }
      else
      {
        TB0CCR0 = 3436;  //��ʱ��TB���¿�������ʱ���պ������ݵ�һ��λ��ʱ��
        IR_Rec_PulseIndex++;      
        if(IR_Rec_PulseIndex < 9)
        {
          //�������һλ
          IR_Rec_Code <<= 1;
          if((IR_IN_PORT & IR_PIN_NUM) == IR_PIN_NUM) //IR_IN==1
          {
            IR_Rec_Code |= 1;
          }
        }                    
        else if(IR_Rec_PulseIndex > 10)//һ���ֽڽ��������������Ϊ�ߵ�ƽ
        {
          if((IR_IN_PORT & IR_PIN_NUM) == IR_PIN_NUM) //IR_IN==1
          {
            g_irReceiveCode = reverseHighLowBit(IR_Rec_Code); //�ߵ�λ����       
            //��������¼�
            g_externalTriggerEvent |= TRI_IR;  
          } 
          g_irReceiveByteOkFlag = 1;
          Stop_TimerB0();//ͣ��ʱ��
          //״̬��λ
          IRDecodeError();
        }
      }
    }
    else//IR_rec error
    { 
      return;
    } 
  }
  else//IR_Send
  {
    if(IR_Send_OneByte_OK_Flag)//һ���ֽڷ������
    {
      IR_SendOneByte_Delay10ms_Count++;
      if(IR_SendOneByte_Delay10ms_Count >= 336)//������10ms����һ���ֽ�
      {
        IR_SendOneByte_Delay10ms_Count = 0;
        IR_Send_OneByte_OK_Flag = 0;
        return;
      }
    }
    else//һ���ֽ�û�з������
    {
      if(IR_Send_Byte())//���ⷢ�����
      {
        IR_Send_OneByte_OK_Flag = 1; 
        IR_Send_Data_Count++;
        if(IR_Send_Data_Count >= g_irSendByteNumberCount) //һ֡���ݷ������
        {
          Stop_IRSend_PWM();
          return;
        }
        IR_Sending_Byte = g_irSendBuffer[IR_Send_Data_Count];    
      }   
    }
  }
  
  return;
} 
