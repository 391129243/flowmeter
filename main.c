#include "msp430x54x.h"
#include "tdc-gp22.h"
#include "params.h"
#include "functions.h"
#include "stdio.h"
#include "timerA0.h"
#include "ucs.h" 
#include "lcd.h"
#include "dataSave.h"
#include "flash.h"
#include "keybord.h"
#include "timerB0.h"
#include "ir.h"
#include "rs485.h"
#include "mbus.h"
#include "rtc.h"
#include "timerA1.h"


//计量循环
void MeterLoop()
{
   readIDbytesTDCGP21();// = testcomunication();
   
   calibrateResonator();
   
   ultrasonicWaterMeterParamInit();//初始化超声波水表计量参数
   
   for(unsigned  long i= 600;i>0;i--);       // 延迟   
     
   while(1)
   {  
        if(g_externalTriggerEvent & TRI_KEYPRESS_L)
        {
          g_externalTriggerEvent &= ~TRI_KEYPRESS_L;
          doKeyPressL();
        }
        if(g_externalTriggerEvent & TRI_IR)
        {
          g_externalTriggerEvent &= ~TRI_IR;
          doDecodeIR();
        }
        if(g_externalTriggerEvent & TRI_RTC)
        {
          g_externalTriggerEvent &= ~TRI_RTC;
          doRtcEvent();
        }  
        ultrasonicTimeOfFlightMeasure();
   }
}




//main函数入口
int main( void )
{
  WDTCTL = WDTPW + WDTHOLD;  // 关闭看门狗 
  initUCS();  //初始化UCS
  initRTC();
  InitLCD();
  initIR();
  Init_Flash();
  Init_Keybord();
  initTimerA1();
  initRS485();
  initMBUS();
  initSPI();
  initConfigureRegisterTDCGP21(); // 初始化TDC-GP22  
  
  _EINT();                                   //开总中断 
  
  readSystemParam();
  
  LCD_ReFlash();

  
  MeterLoop();
}








