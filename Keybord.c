#include "msp430x54x.h"
#include "dataSave.h"
#include "timerA0.h"
#include "timerB0.h"
#include "keybord.h"
#include "params.h"
#include "flash.h"
#include "lcd.h"
#include "timerA1.h"
#include "tdc-gp22.h"

void Init_Keybord(void)
{
  P1IES |= ( BIT2);  //�½����жϣ�BIT0Ϊ������գ�BIT2Ϊ����
  P1IE |= ( BIT2);
  P1DIR &= ~( BIT2);
}

void Enable_Keybord(void)
{
  P2IE |= BIT2;
}

void Disable_Keybord(void)
{
  P2IE &= ~BIT2;
}


//�̰����жϴ������
void doKeyPressL(void)//��������
{  
  unsigned long temp,temp1;         
  
	if( g_displayState == DISP_STAT_CLOSE )               
	{
          g_displayState = DISP_STAT_MAIN;
          
          temp = WfmInfo.Normal.TotalVolume;
          g_waterSurfaceSpeedDisp = g_waterSurfaceSpeed*3.6;//��λת��Ϊm3/h
          if( g_waterSurfaceSpeedDisp < 0 )
            g_waterSurfaceSpeedDisp *= -1;
          temp1 = (unsigned long)(g_waterSurfaceSpeedDisp);          
          
          Display_Else_Icon(DISP_STAT_MAIN,temp,temp1);
        
          if(WfmInfo.State == WFM_STATE_RUN)  
            Display_WaterRunning_Icon(1);		
	}
	else if( g_displayState == DISP_STAT_MAIN )                
	{
          g_displayState = DISP_STAT_TIME;

          Display_Else_Icon(DISP_STAT_TIME,0,0);
	}        
	else if( g_displayState == DISP_STAT_TIME )                
	{
          g_displayState = DISP_STAT_TEMP;
          
          temp = (unsigned long)g_tempValue;
          Display_Else_Icon(DISP_STAT_TEMP,temp,0);
	}             
	else if( g_displayState == DISP_STAT_TEMP )          
	{
          g_displayState = DISP_STAT_TIMEF;
          
          if( g_timeOfFlight_ave < 0 )
            g_timeOfFlight_ave *= -1;
          temp = (unsigned long)g_timeOfFlight_ave;          
          Display_Else_Icon(DISP_STAT_TIMEF,temp,0);
	}
	else if( g_displayState == DISP_STAT_TIMEF )          
	{
          g_displayState = DISP_STAT_TIMEU;
          
          if( g_averageTimeResultUp < 0 )
            g_averageTimeResultUp *= -1;
          temp = (unsigned long)(g_averageTimeResultUp*1000000);            
          if( 0 == g_averageTimeResultDown )
           Display_Else_Icon(DISP_STAT_TIMEU,0,0);  
          else
           Display_Else_Icon(DISP_STAT_TIMEU,temp,0);
	}
	else if( g_displayState == DISP_STAT_TIMEU )          
	{
          g_displayState = DISP_STAT_TIMED;
          
          if( g_averageTimeResultDown < 0 )
            g_averageTimeResultDown *= -1;
          temp = (unsigned long)(g_averageTimeResultDown*1000000);            
          if( 0 == g_averageTimeResultUp )
           Display_Else_Icon(DISP_STAT_TIMED,0,0);  
          else
           Display_Else_Icon(DISP_STAT_TIMED,temp,0);
	}   
	else if( g_displayState == DISP_STAT_TIMED )          
	{
          g_displayState = DISP_STAT_IR;
          
          Display_Else_Icon(DISP_STAT_IR,0,0);
	}   
	else if( g_displayState == DISP_STAT_IR )          
	{
          g_displayState = DISP_STAT_MAIN;
          
          temp = WfmInfo.Normal.TotalVolume;
          g_waterSurfaceSpeedDisp = g_waterSurfaceSpeed*3.6;//��λת��Ϊm3/h
          if( g_waterSurfaceSpeedDisp < 0 )
            g_waterSurfaceSpeedDisp *= -1;
          temp1 = (unsigned long)(g_waterSurfaceSpeedDisp);          
          
          Display_Else_Icon(DISP_STAT_MAIN,temp,temp1);
        
          if(WfmInfo.State == WFM_STATE_RUN)  
            Display_WaterRunning_Icon(1);
	}           
}



#pragma vector=PORT1_VECTOR
__interrupt void PORT1 (void)
{
  if(P1IFG & BIT0)//�����ж�
  {
    P1IFG &= ~BIT0;  //�ֶ����жϱ�־λ 
    _NOP();//ȥխ������Ŵ���
    _NOP();
    _NOP();
    _NOP();
    _NOP();
    if((P1IN & BIT0) != BIT0) //P1.0=0,���Ǹ����źţ��������ĺ�����ʼλ�ź�
    {
      if((g_irReceiveByteOkFlag == 1) && (g_irReceiveOrSendFlag == 0)) //����һ���ֽڽ������ �����Һ��ⷢ��һ֡���
      {
        IRRecPulseDown();
        g_irReceiveByteOkFlag = 0;
        return;
      }
    }
    return;
  }
  else if(P1IFG & BIT2)//�����ж�
  {
    P1IFG &= ~BIT2;  //�ֶ����жϱ�־λ 
    Disable_Keybord(); 
    g_KeyInteruptDown_Flag = 1;//����ʽ�����½����жϱ�־��λ
    startTimerA1(1); //����100ms��ʱ���
    LPM3_EXIT;
    return;
  }
}