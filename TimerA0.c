#include "msp430x54x.h"
#include "tdc-gp22.h"
#include "params.h"
#include "timerA0.h"


//��ʱ��TA0, ��ʱ1s���������㳬����ˮ����ۻ���
void InitTimerA0(unsigned int interval)
{
  TA0CTL |= TACLR;      //���������� 
  
  TA0CTL &= ~TASSEL1;    // ACLK   32768
  TA0CTL |= TASSEL0; 
  
  TA0CTL |= ID__8;   // 32768/8=4096
  
  TA0CTL |= MC__UP;   //  UP mode
  
  TA0CTL &= ~TAIFG;  // Clear interrupt flag
    
  TA0CCTL0 = CCIE;  // Enable interrupt
  
  TA0CCR0 = interval*4096; //4Hz����Ƶ��
  
}



void StopTimerA0(void)
{
  TA0CCTL0 = 0;  // Disenble interrupt
  TA0CCR0 = 0;
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void TimerA0ISR (void) //CCR0
{

  //AmrTimerIntProcess();
}
