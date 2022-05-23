#include "msp430x54x.h"
#include "params.h"
#include "rs485.h"
#include "mbus.h"


//MBUS初始化
void initMBUS(void)
{
  //MBUS串口9600,N,8,1
  UCA0CTL1 |= UCSWRST;                                          // 复位串口功能
  
  UCA0CTL0 |= UCPEN;                                         // 使能奇偶效验  
  UCA0CTL0 |= UCPAR;                                         // 偶效验 
  UCA0CTL0 &= ~UCMSB;                                         // LSB first,字节的低位先传
  UCA0CTL0 &= ~UC7BIT;                                         // 8位数据模式
  UCA0CTL0 &= ~UCSPB;                                         // 1位停止位
  UCA0CTL0 &= ~UCMODE1;                                         // 选择串口功能,点对点异步通讯 
  UCA0CTL0 &= ~UCMODE0;                                         
  UCA0CTL0 &= ~UCSYNC; 
  
  UCA0CTL1 &= ~UCSSEL1;                                        // 时钟源选择ACLK
  UCA0CTL1 |= UCSSEL0;
  UCA0CTL1 &= ~UCRXEIE;                                        // 接收的字节error，接收缓冲器不接收，不产生接收中断
  UCA0CTL1 &= ~UCBRKIE;                                        // 接收的字节break，接收缓冲器不接收，不产生接收中断
  UCA0CTL1 &= ~UCDORM;                                        // not dormant
  UCA0CTL1 &= ~UCTXADDR;                                        // next frame transmitted is Data not an Address
  UCA0CTL1 &= ~UCTXBRK;                                        // next frame transmitted is not a break
  
  UCA0BR0 = 0x03;                                                 // 32768/9600=3.413
  UCA0BR1 = 0x00;
  UCA0MCTL = 0x06;                                               // 波特率调整因子
  
  UCA0CTL1 &= ~UCSWRST;                                        // 使能串口功能
  UCA0IFG &= ~UCRXIFG;                             // 清除接收和发送中断标志
  //UCA0IE |= UCRXIE+UCTXIE;                                     // 使能接收/发送中断
} 



//MBUS
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
{
  if(UCA0IFG & UCRXIFG) 
  {
      g_uartReceiveCode = UCA0RXBUF;		/// Receive byte.
      doCj188Decode();
  }
}