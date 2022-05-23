#include "msp430x54x.h"
#include "functions.h"
#include "params.h"
#include "string.h"
#include "rtc.h"
#include "lcd.h"
#include "dataSave.h"

//��ʼ��RTC
void initRTC(void) 
{
   RTCCTL01 = RTCBCD +  RTCHOLD + RTCMODE + RTCTEV_0;  //BCD��,������ʽ  
   RTCPS0CTL = RT0PSHOLD;                                                           
   RTCPS1CTL = RT1PSHOLD;                 
   RTCSEC = 0x22;                                     //��ʼ����          
   RTCMIN = 0x58;                                     //��ʼ������
   RTCHOUR = 0x23;                                    //��ʼ��Сʱ
   RTCDOW = 0x00;                     

   RTCDAY = 0x26;                                    //���ڳ�ʼ��                   
   RTCMON = 0x07;                                    //��ʼ���·� 
   RTCYEAR = 0x2017;                                 //��ʼ�����                 
   RTCAMINHR = 0x0080;                               //����Сʱ�ͷ�������:00:00����12��
   RTCADOWDAY = 0x0000;                              //�������ں���������
   RTCCTL01 &= ~RTCHOLD;                             //��RTCģ��              
   RTCPS0CTL &= ~RT0PSHOLD;                          //��RTCPS0CTL
   RTCPS1CTL &= ~RT1PSHOLD;                          //��RTCPS1CTL
   RTCCTL01 &= ~(RTCAIFG + RTCTEVIFG);
   //RTCCTL01 |= (RTCAIE + RTCTEVIE);                     //�򿪰�ȫ����ʹ�� ������ʹ��
   //RTCCTL01 &= ~RTCAIFG;
   //RTCCTL01 |= RTCAIE;                     //����ʹ��
   
   enableMinuteInterrupt();
} 

//��RTC�ж�
void enableRtcAlarmInterrupt(void)
{
  RTCCTL01 &= ~RTCAIFG;
  RTCCTL01 |= RTCAIE;
}

//�ر�RTC�ж�
void disableRtcAlarmInterrupt(void)
{
  RTCCTL01 &= ~RTCAIFG;
  RTCCTL01 &= ~RTCAIE;
}

//��1�����ж�
void enableMinuteInterrupt(void)
{
  RTCCTL01 &= ~RTCTEVIFG;
  RTCCTL01 |= RTCTEVIE;
}

//�ر�1�����ж�
void disableMinuteInterrupt(void)
{
  RTCCTL01 &= ~RTCTEVIFG;
  RTCCTL01 &= ~RTCTEVIE;
}

//����ʱ��
void rtcSetDateTimeBCD(uchar *data)
{
	// Disable alarm interrupt.
        RTCCTL01 &= ~RTCAIFG;
	RTCCTL01 &= ~RTCAIE;                    

	while (1)
	{
            if((RTCCTL01 & RTCRDY) == RTCRDY)
            {
                RTCCTL01 |= RTCHOLD;                             //��ͣRTCģ��
                RTCSEC  = *data++;
                RTCMIN  = *data++;
                RTCHOUR = *data++;
                RTCDOW = *data++;
                RTCDAY = *data++;
                RTCMON = *data++;
                RTCYEARL = *data;
                RTCYEARH = 0x20;
                RTCCTL01 &= ~RTCHOLD;                           //��RTCģ��
                break;
            }
	}

	RTCCTL01 &= ~RTCAIFG ;                    //�����־λ
	RTCCTL01 |= RTCAIE;                         //�򿪰�ȫ����ʹ�� ������ʹ��
}

//��ȡʱ��
void rtcGetDateTimeBCD(uchar *data)
{
    while (1)
    {
        if((RTCCTL01 & RTCRDY) == RTCRDY)
        {
            RTCCTL01 |= RTCHOLD;                             //��ͣRTCģ��
            *data++ = RTCSEC;
            *data++ = RTCMIN;
            *data++ = RTCHOUR;
            *data++ = RTCDOW;
            *data++ = RTCDAY;
            *data++ = RTCMON;
            *data = RTCYEARL;
            RTCCTL01 &= ~RTCHOLD;                           //��RTCģ�� 
            break;
        }
    }
}

//��������
void rtcSetAlarmTimeBCD(uchar amin, uchar ahour)
{
	// Disable alarm interrupt.
        RTCCTL01 &= ~RTCAIFG;
	RTCCTL01 &= ~RTCAIE;                    

	while (1)
	{
            if((RTCCTL01 & RTCRDY) == RTCRDY)
            {
                RTCCTL01 |= RTCHOLD;                             //��ͣRTCģ��
                RTCAMIN = amin;  //��������
                RTCAMIN |= 0x80;
                RTCAHOUR = ahour; //Сʱ����
                RTCAHOUR |= 0x80;
                
                RTCADOWDAY = 0x0000;                              //�������ں���������
                RTCCTL01 &= ~RTCHOLD;                           //��RTCģ��
                break;
            }
	}

	RTCCTL01 &= ~RTCAIFG ;                    //�����־λ
	RTCCTL01 |= RTCAIE;                         //�򿪰�ȫ����ʹ�� ������ʹ��
}

//RTC�����¼�
void doRtcEvent(void)
{
  disableRtcAlarmInterrupt(); //�ر�RTC�����жϣ���ֹ�ظ�����
  
}


// RTC�ж�
#pragma vector=RTC_VECTOR
__interrupt void rtc(void)
{    
  switch(RTCIV)
  {
       case 2 : break;             //RTCRDYIFG
       case 4 : 
         {
           rtcGetDateTimeBCD((uchar *)&g_tagRTC); //��ȡˮ��ǰʱ��
           //����ˢ����ʾ
           if( g_displayState == DISP_STAT_TIME )
           {
             Display_Date_Icon( g_tagRTC.g_Year, g_tagRTC.g_Month, g_tagRTC.g_Day, g_tagRTC.g_Hour, g_tagRTC.g_Minite);
           }
           if((g_tagRTC.g_Hour==0x00)&&(g_tagRTC.g_Minite==0x02)) //һ�쵽��
           {
             // �ն���
             saveDayCum();
             if(g_tagRTC.g_Day==0x01) //һ���µ���
             {
               // �¶���
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