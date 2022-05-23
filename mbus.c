#include "msp430x54x.h"
#include "params.h"
#include "rs485.h"
#include "mbus.h"


//MBUS��ʼ��
void initMBUS(void)
{
  //MBUS����9600,N,8,1
  UCA0CTL1 |= UCSWRST;                                          // ��λ���ڹ���
  
  UCA0CTL0 |= UCPEN;                                         // ʹ����żЧ��  
  UCA0CTL0 |= UCPAR;                                         // żЧ�� 
  UCA0CTL0 &= ~UCMSB;                                         // LSB first,�ֽڵĵ�λ�ȴ�
  UCA0CTL0 &= ~UC7BIT;                                         // 8λ����ģʽ
  UCA0CTL0 &= ~UCSPB;                                         // 1λֹͣλ
  UCA0CTL0 &= ~UCMODE1;                                         // ѡ�񴮿ڹ���,��Ե��첽ͨѶ 
  UCA0CTL0 &= ~UCMODE0;                                         
  UCA0CTL0 &= ~UCSYNC; 
  
  UCA0CTL1 &= ~UCSSEL1;                                        // ʱ��Դѡ��ACLK
  UCA0CTL1 |= UCSSEL0;
  UCA0CTL1 &= ~UCRXEIE;                                        // ���յ��ֽ�error�����ջ����������գ������������ж�
  UCA0CTL1 &= ~UCBRKIE;                                        // ���յ��ֽ�break�����ջ����������գ������������ж�
  UCA0CTL1 &= ~UCDORM;                                        // not dormant
  UCA0CTL1 &= ~UCTXADDR;                                        // next frame transmitted is Data not an Address
  UCA0CTL1 &= ~UCTXBRK;                                        // next frame transmitted is not a break
  
  UCA0BR0 = 0x03;                                                 // 32768/9600=3.413
  UCA0BR1 = 0x00;
  UCA0MCTL = 0x06;                                               // �����ʵ�������
  
  UCA0CTL1 &= ~UCSWRST;                                        // ʹ�ܴ��ڹ���
  UCA0IFG &= ~UCRXIFG;                             // ������պͷ����жϱ�־
  //UCA0IE |= UCRXIE+UCTXIE;                                     // ʹ�ܽ���/�����ж�
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