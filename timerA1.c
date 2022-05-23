#include "msp430x54x.h"
#include "params.h"
#include "timerA1.h"
#include "timerB0.h"
#include "rtc.h"
#include "timerA0.h"
#include "dataSave.h"
#include "keybord.h"
#include "lcd.h"

//初始化定时器TA1
void initTimerA1(void)
{
  TA1CTL |= TACLR;      //计数器清零  
  TA1CTL &= ~TASSEL1;    // ACLK   32768
  TA1CTL |= TASSEL0; 
  TA1CTL |= ID__8;   // 32768/8=4096
  TA1CTL |= MC__UP;   //  UP mode
  TA1CTL &= ~TAIFG;  // Clear interrupt flag
  TA1CCTL0 = 0;  // Disenble interrupt
  TA1CCR0 = 0; //stop
}

//启动定时器A1
void startTimerA1(uint interval) // 100ms
{
  TA1CCTL0 = CCIE;  // Enable interrupt
  TA1CCR0 = interval*410; //start
}

//关闭定时器A1
void stopTimerA1(void)
{
  TA1CCTL0 = 0;  // Disenble interrupt
  TA1CCR0 = 0;
}

//开定时器A1中断
void enableTimerA1Interrupt(void)
{
  TA1CCTL0 = CCIE;  // Enable interrupt
}

//关闭定时器A0中断
void disableTimerA1Interrupt(void)
{
  TA1CCTL0 = 0;  // Disenble interrupt
}

unsigned int  LCDReturnMainMenuDelay                     = 0;
void clearLcdReturnMainMenuDelayCount(void)
{
	LCDReturnMainMenuDelay = 0;
}


unsigned char touchKeyDelayCount                         = 0;

#pragma vector=TIMER1_A0_VECTOR
__interrupt void TimerA1ISR (void) //CCR0
{
  if( 1 == g_KeyInteruptDown_Flag )
  {
          touchKeyDelayCount++;
          if( 0x04 == (P1IN & 0x04) )   /// Touch over.
          {
                  g_KeyInteruptDown_Flag = 0;

                  if( touchKeyDelayCount >= 40 )  /// Long touch.
                  {
                          g_externalTriggerEvent |= TRI_KEYPRESS_L;
                  }
                  else if( touchKeyDelayCount >= 10 )  /// Middle touch.
                  {
                          g_externalTriggerEvent |= TRI_KEYPRESS_L;
                  }
                  else if( touchKeyDelayCount >= 2 )  /// Short touch.
                  {
                          g_externalTriggerEvent |= TRI_KEYPRESS_L;
                  }

                  touchKeyDelayCount     = 0;
                  Enable_Keybord();
                  clearLcdReturnMainMenuDelayCount();
          }
  }
  // LCD return back to the main menu.
  else
  {
          LCDReturnMainMenuDelay++;
          if( LCDReturnMainMenuDelay >= 600 )
          {
                  if( g_displayState == DISP_STAT_IR )
                  {
                          if( LCDReturnMainMenuDelay <= 700 )
                          {
                                  return;
                          }
                  }

                  // Display.

                  //g_displayState = DISP_STAT_MAIN;
                  //Display_Cum_Icon(WfmInfo.Normal.TotalVolumeDecimal,(unsigned long)g_waterSurfaceSpeedDisp);
                  clearLcdReturnMainMenuDelayCount();
                  Reset_IR_Param();                    /// Reset IR params.
                  stopTimerA1();                      /// Close timer 100ms.
          }
  }
    
  return;
}
