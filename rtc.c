#include "msp430x54x.h"
#include "functions.h"
#include "params.h"
#include "string.h"
#include "rtc.h"
#include "lcd.h"
#include "dataSave.h"

//初始化RTC
void initRTC(void) 
{
   RTCCTL01 = RTCBCD +  RTCHOLD + RTCMODE + RTCTEV_0;  //BCD码,日历格式  
   RTCPS0CTL = RT0PSHOLD;                                                           
   RTCPS1CTL = RT1PSHOLD;                 
   RTCSEC = 0x22;                                     //初始化秒          
   RTCMIN = 0x58;                                     //初始化分钟
   RTCHOUR = 0x23;                                    //初始化小时
   RTCDOW = 0x00;                     

   RTCDAY = 0x26;                                    //日期初始化                   
   RTCMON = 0x07;                                    //初始化月份 
   RTCYEAR = 0x2017;                                 //初始化年份                 
   RTCAMINHR = 0x0080;                               //闹钟小时和分钟设置:00:00晚上12点
   RTCADOWDAY = 0x0000;                              //闹钟星期和日期设置
   RTCCTL01 &= ~RTCHOLD;                             //打开RTC模块              
   RTCPS0CTL &= ~RT0PSHOLD;                          //打开RTCPS0CTL
   RTCPS1CTL &= ~RT1PSHOLD;                          //打开RTCPS1CTL
   RTCCTL01 &= ~(RTCAIFG + RTCTEVIFG);
   //RTCCTL01 |= (RTCAIE + RTCTEVIE);                     //打开安全访问使能 ，闹钟使能
   //RTCCTL01 &= ~RTCAIFG;
   //RTCCTL01 |= RTCAIE;                     //闹钟使能
   
   enableMinuteInterrupt();
} 

//打开RTC中断
void enableRtcAlarmInterrupt(void)
{
  RTCCTL01 &= ~RTCAIFG;
  RTCCTL01 |= RTCAIE;
}

//关闭RTC中断
void disableRtcAlarmInterrupt(void)
{
  RTCCTL01 &= ~RTCAIFG;
  RTCCTL01 &= ~RTCAIE;
}

//打开1分钟中断
void enableMinuteInterrupt(void)
{
  RTCCTL01 &= ~RTCTEVIFG;
  RTCCTL01 |= RTCTEVIE;
}

//关闭1分钟中断
void disableMinuteInterrupt(void)
{
  RTCCTL01 &= ~RTCTEVIFG;
  RTCCTL01 &= ~RTCTEVIE;
}

//设置时间
void rtcSetDateTimeBCD(uchar *data)
{
	// Disable alarm interrupt.
        RTCCTL01 &= ~RTCAIFG;
	RTCCTL01 &= ~RTCAIE;                    

	while (1)
	{
            if((RTCCTL01 & RTCRDY) == RTCRDY)
            {
                RTCCTL01 |= RTCHOLD;                             //暂停RTC模块
                RTCSEC  = *data++;
                RTCMIN  = *data++;
                RTCHOUR = *data++;
                RTCDOW = *data++;
                RTCDAY = *data++;
                RTCMON = *data++;
                RTCYEARL = *data;
                RTCYEARH = 0x20;
                RTCCTL01 &= ~RTCHOLD;                           //打开RTC模块
                break;
            }
	}

	RTCCTL01 &= ~RTCAIFG ;                    //清除标志位
	RTCCTL01 |= RTCAIE;                         //打开安全访问使能 ，闹钟使能
}

//读取时间
void rtcGetDateTimeBCD(uchar *data)
{
    while (1)
    {
        if((RTCCTL01 & RTCRDY) == RTCRDY)
        {
            RTCCTL01 |= RTCHOLD;                             //暂停RTC模块
            *data++ = RTCSEC;
            *data++ = RTCMIN;
            *data++ = RTCHOUR;
            *data++ = RTCDOW;
            *data++ = RTCDAY;
            *data++ = RTCMON;
            *data = RTCYEARL;
            RTCCTL01 &= ~RTCHOLD;                           //打开RTC模块 
            break;
        }
    }
}

//设置闹钟
void rtcSetAlarmTimeBCD(uchar amin, uchar ahour)
{
	// Disable alarm interrupt.
        RTCCTL01 &= ~RTCAIFG;
	RTCCTL01 &= ~RTCAIE;                    

	while (1)
	{
            if((RTCCTL01 & RTCRDY) == RTCRDY)
            {
                RTCCTL01 |= RTCHOLD;                             //暂停RTC模块
                RTCAMIN = amin;  //分钟设置
                RTCAMIN |= 0x80;
                RTCAHOUR = ahour; //小时设置
                RTCAHOUR |= 0x80;
                
                RTCADOWDAY = 0x0000;                              //闹钟星期和日期设置
                RTCCTL01 &= ~RTCHOLD;                           //打开RTC模块
                break;
            }
	}

	RTCCTL01 &= ~RTCAIFG ;                    //清除标志位
	RTCCTL01 |= RTCAIE;                         //打开安全访问使能 ，闹钟使能
}

//RTC处理事件
void doRtcEvent(void)
{
  disableRtcAlarmInterrupt(); //关闭RTC闹钟中断，防止重复触发
  
}


// RTC中断
#pragma vector=RTC_VECTOR
__interrupt void rtc(void)
{    
  switch(RTCIV)
  {
       case 2 : break;             //RTCRDYIFG
       case 4 : 
         {
           rtcGetDateTimeBCD((uchar *)&g_tagRTC); //获取水表当前时间
           //日期刷新显示
           if( g_displayState == DISP_STAT_TIME )
           {
             Display_Date_Icon( g_tagRTC.g_Year, g_tagRTC.g_Month, g_tagRTC.g_Day, g_tagRTC.g_Hour, g_tagRTC.g_Minite);
           }
           if((g_tagRTC.g_Hour==0x00)&&(g_tagRTC.g_Minite==0x02)) //一天到了
           {
             // 日冻结
             saveDayCum();
             if(g_tagRTC.g_Day==0x01) //一个月到了
             {
               // 月冻结
               saveMonthCum();
             }
           }    
         }
       break;    //RTCTEVIFG     
       case 6 :  
         { 
           delay_ms(2);             
           g_externalTriggerEvent |= TRI_RTC;
           LPM3_EXIT;    
         } 
       break; //RTCAIFG        
       case 8 : break;
       case 10 : break; 
       default: break;
  }
}