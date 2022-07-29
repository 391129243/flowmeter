#include "msp430x54x.h"
#include "tdc-gp22.h"
#include "params.h"
#include "timerA0.h"


//定时器TA0, 定时1s中用来计算超声波水表的累积量
void InitTimerA0(unsigned int interval)
{
  TA0CTL |= TACLR;      //计数器清零 
  
  TA0CTL &= ~TASSEL1;    // ACLK   32768
  TA0CTL |= TASSEL0; 
  
  TA0CTL |= ID__8;   // 32768/8=4096
  
  TA0CTL |= MC__UP;   //  UP mode
  
  TA0CTL &= ~TAIFG;  // Clear interrupt flag
    
  TA0CCTL0 = CCIE;  // Enable interrupt
  
  TA0CCR0 = interval*4096; //4Hz采样频率
  
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
