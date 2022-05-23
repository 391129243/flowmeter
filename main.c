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


//����ѭ��
void MeterLoop()
{
   readIDbytesTDCGP21();// = testcomunication();
   
   calibrateResonator();
   
   ultrasonicWaterMeterParamInit();//��ʼ��������ˮ���������
   
   for(unsigned  long i= 600;i>0;i--);       // �ӳ�   
     
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




//main�������
int main( void )
{
  WDTCTL = WDTPW + WDTHOLD;  // �رտ��Ź� 
  initUCS();  //��ʼ��UCS
  initRTC();
  InitLCD();
  initIR();
  Init_Flash();
  Init_Keybord();
  initTimerA1();
  initRS485();
  initMBUS();
  initSPI();
  initConfigureRegisterTDCGP21(); // ��ʼ��TDC-GP22  
  
  _EINT();                                   //�����ж� 
  
  readSystemParam();
  
  LCD_ReFlash();

  
  MeterLoop();
}








