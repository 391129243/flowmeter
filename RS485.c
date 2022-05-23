#include "msp430x54x.h"
#include "params.h"
#include "rs485.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "functions.h"


//RS485初始化
void initRS485(void)
{
  //RS485串口9600,N,8,1
  P10SEL |= BIT4 + BIT5;                                        // 选择串口功能
  P10DIR |= BIT4;
  P10DIR &= ~BIT5;
  
  UCA3CTL1 |= UCSWRST;                                          // 复位串口功能
  
  //UCA3CTL0 |= UCPEN;                                         // 使能奇偶效验  
  //UCA3CTL0 |= UCPAR;                                         // 偶效验 
  UCA3CTL0 &= ~UCMSB;                                         // LSB first,字节的低位先传
  UCA3CTL0 &= ~UC7BIT;                                         // 8位数据模式
  UCA3CTL0 &= ~UCSPB;                                         // 1位停止位
  UCA3CTL0 &= ~UCMODE1;                                         // 选择串口功能,点对点异步通讯 
  UCA3CTL0 &= ~UCMODE0;                                         
  UCA3CTL0 &= ~UCSYNC; 
  
  UCA3CTL1 &= ~UCSSEL1;                                        // 时钟源选择ACLK
  UCA3CTL1 |= UCSSEL0;
  UCA3CTL1 &= ~UCRXEIE;                                        // 接收的字节error，接收缓冲器不接收，不产生接收中断
  UCA3CTL1 &= ~UCBRKIE;                                        // 接收的字节break，接收缓冲器不接收，不产生接收中断
  UCA3CTL1 &= ~UCDORM;                                        // not dormant
  UCA3CTL1 &= ~UCTXADDR;                                        // next frame transmitted is Data not an Address
  UCA3CTL1 &= ~UCTXBRK;                                        // next frame transmitted is not a break
  
  UCA3BR0 = 0x03;                                                 // 32768/9600=3.413  
  UCA3BR1 = 0x00;
  UCA3MCTL = 0x06;                                               // 波特率调整因子
  
  UCA3CTL1 &= ~UCSWRST;                                        // 使能串口功能
  UCA3IFG &= ~UCRXIFG;                             // 清除接收和发送中断标志
  UCA3IE |= UCRXIE;                                     // 使能接收/发送中断
  
  P2DIR |= BIT0;  /// RS485 Control Set 1, Tx modle
  P2OUT |= BIT0;
} 


void Print_Byte(unsigned char ch)
{
   UCA3TXBUF = ch;
   while (!(UCA3IFG & UCTXIFG)); //等待发送完成
   UCA3IFG &= ~UCTXIFG; //清发送完成标志位
}
void Print_String(char *str)
{
   while(*str)
   {
       Print_Byte(*str++);
   }
}

/*void Print_Text(float timeValue)
{
   char tmpbuf1[10];
   sprintf(tmpbuf1,"%.1f",timeValue);
   Print_String(tmpbuf1);
}*/

void Print_Text1(float timeValue11)
{
   char tmpbuf2[10];
   sprintf(tmpbuf2,"%.0f",timeValue11);
   Print_String(tmpbuf2);
}

void Print_TimeOut(void)
{
   Print_String("TimeOut! ");
   Print_Byte('\r');
   Print_Byte('\n'); 
}

void Print_Information(void)
{
  Print_String("Accumulate:");//输出累积流量
  Print_Text1((float)(WfmInfo.Normal.TotalVolumeDecimal*100));
  Print_String("L");//单位
  Print_Byte('\r');
  Print_Byte('\n');
  
  Print_String("Current Speed:");//输出瞬时流量
  Print_Text1((float)(g_waterSurfaceSpeedDisp*3.6));
  Print_String("L/h");//单位
  Print_Byte('\r');
  Print_Byte('\n'); 

  Print_String("Time Tof:");//输出飞行时间差
  Print_Text1((float)g_timeOfFlight_ave);
  Print_String("ps");//单位
  Print_Byte('\r');
  Print_Byte('\n');

  Print_String("Time Up:");//输出上游飞行时间
  Print_Text1((float)(g_averageTimeResultUp*1000000));
  Print_String("ps");//单位
  Print_Byte('\r');
  Print_Byte('\n'); 
  
  Print_String("Time Up:");//输出下游飞行时间
  Print_Text1((float)(g_averageTimeResultDown*1000000));
  Print_String("ps");//单位
  Print_Byte('\r');
  Print_Byte('\n'); 

  Print_Byte('\r');
  Print_Byte('\n'); 
  Print_Byte('\r');
  Print_Byte('\n');  

  
}




/***************************************************************************
 * DecodeIrMessage function.
 *
 * @brief	Decode the IR cj188 messages.
 **************************************************************************/
void DecodeUartMessage(void)
{
	switch( g_uartReceiveBuffer[9] )
	{
		// Read command.
		case cj188ReadDataCommandAck:
		{

			break;
		}
		case cj188ReadMeterAddressCommandAck:
		{
			break;
		}
		default: break;
	}
}


/***************************************************************************
 * judgeUartMessageIsCorrect function.
 *
 * @brief	Judge the uart message is correct or not.
 **************************************************************************/
unsigned char judgeUartMessageIsCorrect(void)
{
  unsigned char dataSum;

  if((g_uartReceiveBuffer[0] == 0x68) && (g_uartReceiveBuffer[1] == 0x10) && (g_uartReceiveBuffer[g_uartReceiveByteNumberCount-1] == 0x16))  /// Judge frame head, meter type and the end byte.
  {
    if((g_uartReceiveBuffer[2] == g_systemPara.szTermID[0]) && (g_uartReceiveBuffer[3] == g_systemPara.szTermID[1]) && (g_uartReceiveBuffer[4] == g_systemPara.szTermID[2]) && (g_uartReceiveBuffer[5] == g_systemPara.szTermID[3]) && (g_uartReceiveBuffer[6] == g_systemPara.szTermID[4]) && (g_uartReceiveBuffer[7] == g_systemPara.szTermID[5]) && (g_uartReceiveBuffer[8] == g_systemPara.szTermID[6]))  /// Judge the water meter address.
    {
      dataSum = sumCheck(g_uartReceiveBuffer, (g_uartReceiveByteNumberCount - 2));
      if(g_uartReceiveBuffer[g_uartReceiveByteNumberCount-2] == dataSum)
      {
        return 1;
      }
      else
        return 0;
    }
    else
      return 0;
  }
  else
    return 0;
}


void doCj188Decode(void)
{

}


//RS485
#pragma vector=USCI_A3_VECTOR
__interrupt void USCI_A3_ISR(void)
{
  if(UCA3IFG & UCRXIFG) 
  {
      g_uartReceiveCode = UCA3RXBUF;		/// Receive byte.
      doCj188Decode();
  }
}