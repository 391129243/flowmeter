#include "msp430x54x.h"
#include "functions.h"
#include "timerB0.h"
#include "params.h"
#include "string.h"
#include "ir.h"
#include "timerA0.h"
#include "timerA1.h"

unsigned char IR_Rec_First_Flag = 0;  //检测到红外引导码下降沿标志
unsigned char IR_Rec_Start_Flag = 0;  //红外编码解析开始标志
unsigned char IR_Rec_PulseIndex = 0; //红外接收到第几个位标志
//红外解码结果
unsigned int IR_Rec_Code = 0;//接收到的红外字节码

//红外电源开启
void powerOnIR(void)
{
  IR_POWER_ON;
  delay_ms(5);
  IR_INT_ENABLE;
  Reset_IR_Param();
}

//红外电源关闭
void powerCloseIR(void)
{
  Reset_IR_Param();
  
  IR_POWER_OFF;
  IR_INT_DISABLE;
  TB0CTL &= ~TASSEL1;                           // ACLK为时钟源,32768
  TB0CTL |= TASSEL0; 
}

//红外初始化
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
  TB0CTL |= TBCLR;                              //计数器清零 
  TB0CTL &= ~TASSEL1;                           // ACLK为时钟源,32768
  TB0CTL |= TASSEL0; 
  TB0CTL &= ~(ID0 + ID1);                       //时钟源不分频,8MHz
  TB0CTL |= TBCLR;                              //计数器清零    
  TB0CTL &= ~MC1;                             // 定时器工作模式为增模式
  TB0CTL |= MC0; 
  
  TB0CCTL0 |= CCIE;
  TB0CCR0 = 0;                                //定时器TB停止
}

//红外脉冲接收下跳沿触发
void IRRecPulseDown(void)
{
  if(!IR_Rec_First_Flag)//脉冲起始，检测到红外引导码下降沿
  {
    IR_Rec_First_Flag = 1;
    TB0CCR0 = 1718;  //定时器TB重新开启，定时接收红外数据的半个位的时间
  }
}

//红外解码错误
void IRDecodeError(void)
{ 
  IR_Rec_First_Flag = 0;
  IR_Rec_Start_Flag = 0;
  IR_Rec_PulseIndex = 0;
  IR_Rec_Code = 0;
}

//初始化定时器TB0专用做红外解析，红外接收波特率2400
void Init_TimerB0_IRRecive(void)
{
  P4SEL &= ~BIT0;  //引脚设为外部信号下降沿中断输入                            
  P4DIR &= ~BIT0; 
  
  TB0CTL &= ~TASSEL0;                           // MCLK为时钟源,8M
  TB0CTL |= TASSEL1; 
  TB0CCTL0 |= CCIE;  //开定时器TB0中断
  TB0CCTL0 &= ~CCIFG;  //清CCR0中断标志
  TB0CCR0 = 0;    //定时器TB0停止
}

//暂停定时器TB
void Stop_TimerB0(void)                         
{
  TB0CCR0 = 0;
  TB0CCR1 = 0;
}

//下面是红外发送用
unsigned char IR_Send_First_Flag = 0;  //红外引导码发送位标志
unsigned char IR_Send_Start_Flag = 0;  //红外数据位发送开始标志
unsigned char IR_Send_PulseIndex = 0;  //红外发送到第几个位标志

unsigned char IR_Send_Data_Count = 0;//红外发送到第几个字节标志
unsigned char IR_Send_Count_38KHz = 0;//38KHzPWM波计数
unsigned char IR_Send_OneByte_OK_Flag = 0;//一个字节发送完成标志
unsigned char IR_Sending_Byte = 0;//当前正在发送的一个字节红外码数据

unsigned int IR_SendOneByte_Delay10ms_Count = 0;//掌上机红外每发送完一个字节数据需要延时10ms,才能发送下一个字节数据

//初始化定时器TB0专用做红外发射，红外发送波特率2400
void Init_TimerB0_IRSend(void)
{   
    P4SEL |= BIT0;                              //从P4.0输出
    P4DIR |= BIT0;  

    TB0CTL &= ~(CNTL0 + CNTL1);                   //16TBR 
    TB0CTL |= TBCLR;                              //计数器清零 
    TB0CTL |= TASSEL1;                           // SCLK为时钟源8MHz
    TB0CTL &= ~TASSEL0; 
    TB0CTL &= ~(ID0 + ID1);                       //时钟源不分频8MHz
    TB0CTL |= MC0;                             // 定时器增模式
    TB0CTL &= ~MC1;

    TB0CCR0 = 238;                                //定时器TB停止 ，38KHz载波   
    TB0CCTL0 |= CCIE;  
    TB0CCTL0 &= ~CCIFG;                         //清CCR0中断标志
      
    
    TB0CCR1 = 119;  
    TB0CCTL1 &= ~CAP;                            //比较
}


//开始红外发送模式
void Start_IRSend_PWM(void)
{
  _NOP();
  _NOP();
  
  TB0CCR0 = 0;//暂停定时器TB0
  TB0CCR1 = 0;
  Init_TimerB0_IRSend();//红外发送初始化
  g_irReceiveOrSendFlag = 1;//红外转发送模式
  
  IR_Send_OneByte_OK_Flag = 1;
  IR_Send_Data_Count = 0;
  IR_Sending_Byte = g_irSendBuffer[IR_Send_Data_Count];//IR发送的第一个字节
}


//结束红外发送模式，转为红外待接收模式
void Stop_IRSend_PWM(void)
{
  TB0CCR0 = 0;//暂停定时器TB0
  TB0CCR1 = 0;
  
  //IR_Send_OneByte_OK_Flag = 0;
  //IR_Send_Data_Count = 0;
  g_irSendByteNumberCount = 0;
  memset(g_irSendBuffer,0,IR_MAX_SEND_BUFFER);//红外发送缓冲区清空
  Init_TimerB0_IRRecive();//红外接收初始化
  g_irReceiveOrSendFlag = 0;//红外转接收模式
  
  _NOP();
  _NOP();
}

//打开红外发送PWM功能
void Open_IRSend_PWM(void)
{
  TB0CCTL1 |= OUTMOD0;   //比较重新开启
  TB0CCTL1 |= OUTMOD1;
  TB0CCTL1 |= OUTMOD2;
}

//关闭红外发送PWM功能
void Close_IRSend_PWM(void)
{
  TB0CCTL1 &= ~OUTMOD0;   //比较关闭
  TB0CCTL1 &= ~OUTMOD1;
  TB0CCTL1 &= ~OUTMOD2;
}

//红外发送错误
void IRSendError(void)
{
  IR_Send_First_Flag = 0;
  IR_Send_Start_Flag = 0;
  IR_Send_PulseIndex = 0;
}

//红外参数复位
void Reset_IR_Param(void)
{
  Init_TimerB0_IRRecive();
  g_irReceiveOrSendFlag = 0;//红外转接收模式
  g_irReceiveByteOkFlag = 1;
  
  IR_Rec_First_Flag = 0;
  IR_Rec_Start_Flag = 0;
  IR_Rec_PulseIndex = 0;
  IR_Rec_Code = 0;
  memset(g_irReceiveBuffer,0,IR_MAX_RECEIVE_BUFFER);//红外接收缓冲区清空
  g_irReceiveByteNumberCount = 0;
  
  IR_Send_First_Flag = 0;
  IR_Send_Start_Flag = 0;
  IR_Send_PulseIndex = 0;
  IR_Send_Data_Count = 0;//红外发送到第几个字节标志
  IR_Send_Count_38KHz = 0;//38KHzPWM波计数
  IR_Send_OneByte_OK_Flag = 1;//一个字节发送完成标志
  IR_Sending_Byte = 0;//当前正在发送的一个字节红外码数据
  IR_SendOneByte_Delay10ms_Count = 0;//掌上机红外每发送完一个字节数据需要延时10ms,才能发送下一个字节数据
  memset(g_irSendBuffer,0,IR_MAX_SEND_BUFFER);//红外发送缓冲区清空
  g_irSendByteNumberCount = 0;  
  TB0CCR1 = 0;
}

//红外发送一个字节
unsigned char IR_Send_Byte(void)
{
  if(!IR_Send_First_Flag)//发送字节起始位，低电平
  {
    if(!IR_Send_Count_38KHz)//开始发送起始位，开PWM
    {
      Open_IRSend_PWM();//开红外发送PWM
    }
    IR_Send_Count_38KHz++;
    if(IR_Send_Count_38KHz >= 14)//14个38KHz脉冲，刚好是红外发射一位的时间
    {
      IR_Send_Count_38KHz = 0;
      IR_Send_First_Flag = 1;
    }
  }
  else//发送数据字节8位
  {
    if(!IR_Send_Start_Flag) //判断一位是否发送完成
    {
      unsigned char temp = IR_Sending_Byte & 0x01;
      if(temp)//该位为1
      {
        Close_IRSend_PWM();//关红外发送PWM
      }
      else//该位为0
      {
        Open_IRSend_PWM();//开红外发送PWM
      }
      IR_Sending_Byte >>= 1; //发送的字节右移1位     
      IR_Send_Start_Flag = 1;
    }
    IR_Send_Count_38KHz++;
    if(IR_Send_Count_38KHz >= 14)//一位发送完成
    {
      IR_Send_Count_38KHz = 0;
      IR_Send_Start_Flag = 0;//一位发送完成标志
      IR_Send_PulseIndex ++;
      if(IR_Send_PulseIndex >= 8)//停止位发送完成
      {
        //状态回位
        IRSendError();
        Close_IRSend_PWM();//关红外发送PWM
        return 1;
      }
    }  
  }
  return 0;
}


//Timer0_B0 中断服务程序
#pragma vector=TIMERB0_VECTOR
__interrupt void Timer0_B0 (void) //TB0CCR0
{
  if(!g_irReceiveOrSendFlag)//IR_Rec
  {
    if(IR_Rec_First_Flag)
    {
      if(!IR_Rec_Start_Flag) //判断解析是否已经开始
      {
        if((IR_IN_PORT & IR_PIN_NUM) != IR_PIN_NUM) //IR_IN==0,红外起始位
        {
          TB0CCR0 = 3436;  //定时器TB重新开启，定时接收红外数据的一个位的时间
          IR_Rec_Start_Flag = 1;
        }
        else//干扰信号
        {
          g_irReceiveByteOkFlag = 1;
          IRDecodeError();
          Stop_TimerB0();//停计时器
          return; 
        }
      }
      else
      {
        TB0CCR0 = 3436;  //定时器TB重新开启，定时接收红外数据的一个位的时间
        IR_Rec_PulseIndex++;      
        if(IR_Rec_PulseIndex < 9)
        {
          //结果上移一位
          IR_Rec_Code <<= 1;
          if((IR_IN_PORT & IR_PIN_NUM) == IR_PIN_NUM) //IR_IN==1
          {
            IR_Rec_Code |= 1;
          }
        }                    
        else if(IR_Rec_PulseIndex > 10)//一个字节解码结束，结束码为高电平
        {
          if((IR_IN_PORT & IR_PIN_NUM) == IR_PIN_NUM) //IR_IN==1
          {
            g_irReceiveCode = reverseHighLowBit(IR_Rec_Code); //高地位互换       
            //调用相关事件
            g_externalTriggerEvent |= TRI_IR;  
          } 
          g_irReceiveByteOkFlag = 1;
          Stop_TimerB0();//停计时器
          //状态回位
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
    if(IR_Send_OneByte_OK_Flag)//一个字节发送完成
    {
      IR_SendOneByte_Delay10ms_Count++;
      if(IR_SendOneByte_Delay10ms_Count >= 336)//红外间隔10ms发送一个字节
      {
        IR_SendOneByte_Delay10ms_Count = 0;
        IR_Send_OneByte_OK_Flag = 0;
        return;
      }
    }
    else//一个字节没有发送完成
    {
      if(IR_Send_Byte())//红外发送完成
      {
        IR_Send_OneByte_OK_Flag = 1; 
        IR_Send_Data_Count++;
        if(IR_Send_Data_Count >= g_irSendByteNumberCount) //一帧数据发送完成
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
