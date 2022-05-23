#include "msp430x54x.h"
#include "functions.h"
#include "dataSave.h"
#include "timerB0.h"
#include "params.h"
#include "ir.h"
#include "rtc.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"

//���ⷢ�ͻ�������ʼ��
void Init_IR_Send_Buf(void)
{
  g_irSendBuffer[0] = 0x68;//֡��ʼ��
  g_irSendBuffer[1] = 0x10;//�Ǳ����ͣ�ˮ��
  g_irSendBuffer[2] = g_systemPara.szTermID[0];//��ID�ţ��߸��ֽڣ�
  g_irSendBuffer[3] = g_systemPara.szTermID[1];
  g_irSendBuffer[4] = g_systemPara.szTermID[2];
  g_irSendBuffer[5] = g_systemPara.szTermID[3];
  g_irSendBuffer[6] = g_systemPara.szTermID[4];
  g_irSendBuffer[7] = g_systemPara.szTermID[5];
  g_irSendBuffer[8] = g_systemPara.szTermID[6];
}

void Init_IR_Send_Buf_ReadMeterAddress(void)
{
  g_irSendBuffer[0] = 0x68;//֡��ʼ��
  g_irSendBuffer[1] = 0x10;//�Ǳ����ͣ�ˮ��
  g_irSendBuffer[2] = g_systemPara.szTermID[0];//���ַ���߸��ֽڣ�
  g_irSendBuffer[3] = g_systemPara.szTermID[1];
  g_irSendBuffer[4] = g_systemPara.szTermID[2];
  g_irSendBuffer[5] = g_systemPara.szTermID[3];
  g_irSendBuffer[6] = g_systemPara.szTermID[4];
  g_irSendBuffer[7] = g_systemPara.szTermID[5];
  g_irSendBuffer[8] = g_systemPara.szTermID[6];
}


//�������յ�irCJ188����֡
void decodeIrReceiveCJ188Frame(void)
{
  switch (g_irReceiveBuffer[9])
  {
    //write
    case WriteData_CMD:
    {
      Init_IR_Send_Buf();//�趨����֡�Ĺ̶��ֽ�
      g_irSendBuffer[12] = 0xA0;
      g_irSendBuffer[13] = 0x01;
      g_irSendBuffer[14] = 0x00;
         
      if((WriteData_SysTime_DefineL == g_irReceiveBuffer[11]) && (WriteData_SysTime_DefineH == g_irReceiveBuffer[12]))//��ϵͳʱ��
      {
        if((g_irReceiveBuffer[14] >= 0x60) || (g_irReceiveBuffer[15] >= 0x60) || (g_irReceiveBuffer[16] >= 0x25) || (g_irReceiveBuffer[17] >= 0x32) || (g_irReceiveBuffer[17] <= 0x00) || (g_irReceiveBuffer[18] >= 0x13) || (g_irReceiveBuffer[18] <= 0x00) || (g_irReceiveBuffer[19] <= 0x13))//ϵͳʱ����Ч�ж�
        {
          g_irSendBuffer[9] = 0xC4;//�����룬�쳣
        }
        else//��Ч
        {
          g_rtcTempBuffer[0] = g_irReceiveBuffer[14];   //second
          g_rtcTempBuffer[1] = g_irReceiveBuffer[15];   //minute
          g_rtcTempBuffer[2] = g_irReceiveBuffer[16];   //hour
          g_rtcTempBuffer[3] = 0;                       //weekday
          g_rtcTempBuffer[4] = g_irReceiveBuffer[17];   //day
          g_rtcTempBuffer[5] = g_irReceiveBuffer[18];   //month
          g_rtcTempBuffer[6] = g_irReceiveBuffer[19];   //year     
          
          rtcSetDateTimeBCD(g_rtcTempBuffer);
            
          //��ϵͳʱ���趨�ɹ�����
          g_irSendBuffer[9] = 0x84;//������        
        }
        g_irSendBuffer[10] = 0x03;
        g_irSendBuffer[11] = 0x15;
        g_irSendBuffer[15] = 0x16;
        g_irSendByteNumberCount = 16;
        g_irSendBuffer[14] = sumCheck(g_irSendBuffer,(g_irSendByteNumberCount -2));//��Ч��λ
      }
      else//error
      {
        return;
      }   
      Start_IRSend_PWM();//��ʼһ֡�ĺ��ⷢ��   
      break;
    }

   //read
  case ReadData_CMD: 
    {
      Init_IR_Send_Buf();//�趨����֡�Ĺ̶��ֽ�
      g_irSendBuffer[9] = 0x81;
      g_irSendBuffer[13] = 0x01;      
   
      if((ReadData_Month_DefineL == g_irReceiveBuffer[11]) && (ReadData_Month_DefineH == g_irReceiveBuffer[12]))//���¶�������
      {
        uchar flag;
        flag = readMonthCum(g_irReceiveBuffer[14]);
        if(flag==0)//�쳣
        {
          g_irSendBuffer[9] = 0xC1;
          g_irSendBuffer[10] = 0x03;
          g_irSendBuffer[11] = 0x79;
          g_irSendBuffer[12] = 0x81; 
          g_irSendBuffer[13] = 0x01;
          g_irSendBuffer[14] = 0x00;
          g_irSendBuffer[15] = 0x16;
          g_irSendByteNumberCount = 16;
          g_irSendBuffer[14] = sumCheck(g_irSendBuffer,(g_irSendByteNumberCount -2));//��Ч��λ            
        }
        else//������Ӧ
        {
          g_irSendBuffer[9] = 0x81;
          g_irSendBuffer[10] = 0x08;
          g_irSendBuffer[11] = 0x79;
          g_irSendBuffer[12] = 0x81; 
          g_irSendBuffer[13] = 0x01;
          uchar temp = (uchar)(WfmInfo.Normal.TotalVolumeSave%100);
          g_irSendBuffer[14] = dectoBCD(temp);
          temp = (uchar)((WfmInfo.Normal.TotalVolumeSave%10000)/100);
          g_irSendBuffer[15] = dectoBCD(temp);
          temp = (uchar)((WfmInfo.Normal.TotalVolumeSave%1000000)/10000);
          g_irSendBuffer[16] = dectoBCD(temp);
          temp = (uchar)(WfmInfo.Normal.TotalVolumeSave/1000000);
          g_irSendBuffer[17] = dectoBCD(temp);  
          g_irSendBuffer[18] = 0x2A;//��λС��        
          g_irSendBuffer[19] = 0x00;
          g_irSendBuffer[20] = 0x16;
          g_irSendByteNumberCount = 21;
          g_irSendBuffer[19] = sumCheck(g_irSendBuffer,(g_irSendByteNumberCount -2));//��Ч��λ             
        }
      }  
      else if((ReadData_Day_DefineL == g_irReceiveBuffer[11]) && (ReadData_Day_DefineH == g_irReceiveBuffer[12]))//���ն�������
      {
        uchar flag;
        flag = readDayCum(g_irReceiveBuffer[14]);
        if(flag==0)//�쳣
        {
          g_irSendBuffer[9] = 0xC1;
          g_irSendBuffer[10] = 0x03;
          g_irSendBuffer[11] = 0x7B;
          g_irSendBuffer[12] = 0x81; 
          g_irSendBuffer[13] = 0x01;
          g_irSendBuffer[14] = 0x00;
          g_irSendBuffer[15] = 0x16;
          g_irSendByteNumberCount = 16;
          g_irSendBuffer[14] = sumCheck(g_irSendBuffer,(g_irSendByteNumberCount -2));//��Ч��λ            
        }
        else//������Ӧ
        {
          g_irSendBuffer[9] = 0x81;
          g_irSendBuffer[10] = 0x08;
          g_irSendBuffer[11] = 0x7B;
          g_irSendBuffer[12] = 0x81; 
          g_irSendBuffer[13] = 0x01;
          uchar temp = (uchar)(WfmInfo.Normal.TotalVolumeSave%100);
          g_irSendBuffer[14] = dectoBCD(temp);
          temp = (uchar)((WfmInfo.Normal.TotalVolumeSave%10000)/100);
          g_irSendBuffer[15] = dectoBCD(temp);
          temp = (uchar)((WfmInfo.Normal.TotalVolumeSave%1000000)/10000);
          g_irSendBuffer[16] = dectoBCD(temp);
          temp = (uchar)(WfmInfo.Normal.TotalVolumeSave/1000000);
          g_irSendBuffer[17] = dectoBCD(temp);  
          g_irSendBuffer[18] = 0x2A;//��λС��                    
          g_irSendBuffer[19] = 0x00;
          g_irSendBuffer[20] = 0x16;
          g_irSendByteNumberCount = 21;
          g_irSendBuffer[19] = sumCheck(g_irSendBuffer,(g_irSendByteNumberCount -2));//��Ч��λ             
        }        
      }  
      else if((ReadData_SysTime_DefineL == g_irReceiveBuffer[11]) && (ReadData_SysTime_DefineH == g_irReceiveBuffer[12]))//��ϵͳʱ��
      {
        rtcGetDateTimeBCD((uchar *)&g_tagRTC);
        
        g_irSendBuffer[10] = 0x09;
        g_irSendBuffer[11] = 0x53;
        g_irSendBuffer[12] = 0x81;
        g_irSendBuffer[14] = g_tagRTC.g_Second;
        g_irSendBuffer[15] = g_tagRTC.g_Minite;
        g_irSendBuffer[16] = g_tagRTC.g_Hour;
        g_irSendBuffer[17] = g_tagRTC.g_Day;
        g_irSendBuffer[18] = g_tagRTC.g_Month;
        g_irSendBuffer[19] = g_tagRTC.g_Year;
        g_irSendBuffer[20] = 0x00;
        g_irSendBuffer[21] = 0x16;
        g_irSendByteNumberCount = 22;
        g_irSendBuffer[20] = sumCheck(g_irSendBuffer,(g_irSendByteNumberCount -2));//��Ч��λ         
      }
      else if((ReadData_Cum_DefineL == g_irReceiveBuffer[11]) && (ReadData_Cum_DefineH == g_irReceiveBuffer[12]))//����������
      {
        g_irSendBuffer[10] = 0x16;
        g_irSendBuffer[11] = 0x1F;
        g_irSendBuffer[12] = 0x90;
        g_irSendBuffer[13] = 0x04;
        
        
        uchar temp = (uchar)(WfmInfo.Normal.TotalVolume%100);
        g_irSendBuffer[14] = dectoBCD(temp);
        temp = (uchar)((WfmInfo.Normal.TotalVolume%10000)/100);
        g_irSendBuffer[15] = dectoBCD(temp);
        temp = (uchar)((WfmInfo.Normal.TotalVolume%1000000)/10000);
        g_irSendBuffer[16] = dectoBCD(temp);
        temp = (uchar)(WfmInfo.Normal.TotalVolume/1000000);
        g_irSendBuffer[17] = dectoBCD(temp);  
        g_irSendBuffer[18] = 0x2A;//��λС��
        
        temp = (uchar)(WfmInfo.Normal.TotalVolumeDecimal%100);
        g_irSendBuffer[19] = dectoBCD(temp);
        temp = (uchar)((WfmInfo.Normal.TotalVolumeDecimal%10000)/100);
        g_irSendBuffer[20] = dectoBCD(temp);
        temp = (uchar)((WfmInfo.Normal.TotalVolumeDecimal%1000000)/10000);
        g_irSendBuffer[21] = dectoBCD(temp);
        temp = (uchar)(WfmInfo.Normal.TotalVolumeDecimal/1000000);
        g_irSendBuffer[22] = dectoBCD(temp);
        g_irSendBuffer[23] = 0x2A;//��λС��
        
        rtcGetDateTimeBCD((uchar *)&g_tagRTC);
        
        g_irSendBuffer[24] = g_tagRTC.g_Second;
        g_irSendBuffer[25] = g_tagRTC.g_Minite;
        g_irSendBuffer[26] = g_tagRTC.g_Hour;
        g_irSendBuffer[27] = g_tagRTC.g_Day;
        g_irSendBuffer[28] = g_tagRTC.g_Month;
        g_irSendBuffer[29] = g_tagRTC.g_Year;
        g_irSendBuffer[30] = 0x00;
        g_irSendBuffer[31] = 0x00;
        g_irSendBuffer[32] = 0x00;
        g_irSendBuffer[33] = 0x00;
        g_irSendBuffer[34] = 0x16;
        g_irSendByteNumberCount = 35;
        g_irSendBuffer[33] = sumCheck(g_irSendBuffer,(g_irSendByteNumberCount -2));//��Ч��λ
      }     
      Start_IRSend_PWM();//��ʼһ֡�ĺ��ⷢ��
      break;
    }
  case ReadAdress_CMD: 
    {
      if((ReadAdress_Adress_DefineL == g_irReceiveBuffer[11]) && (ReadAdress_Adress_DefineH == g_irReceiveBuffer[12]))//�����ַ
      {
        Init_IR_Send_Buf_ReadMeterAddress();//�趨����֡�Ĺ̶��ֽ�
        g_irSendBuffer[9] = 0x83;
        g_irSendBuffer[10] = 0x03;
        g_irSendBuffer[11] = 0x0A;
        g_irSendBuffer[12] = 0x81;
        g_irSendBuffer[13] = 0x02;
        g_irSendBuffer[14] = 0x00;
        g_irSendBuffer[15] = 0x16;
        g_irSendByteNumberCount = 16;
        g_irSendBuffer[14] = sumCheck(g_irSendBuffer,(g_irSendByteNumberCount -2));//��Ч��λ
        
      }
      else//error
      {
        return;
      } 
      Start_IRSend_PWM();//��ʼһ֡�ĺ��ⷢ��
      break;
    }
  }
}

//irCJ188����֡���ַ����Ч���ж�
uchar judgeIrCJ188FrameIsCorrect(void)
{
  uchar dataSum;
  if((g_irReceiveBuffer[2] == g_systemPara.szTermID[0]) && (g_irReceiveBuffer[3] == g_systemPara.szTermID[1]) && (g_irReceiveBuffer[4] == g_systemPara.szTermID[2]) && (g_irReceiveBuffer[5] == g_systemPara.szTermID[3]) && (g_irReceiveBuffer[6] == g_systemPara.szTermID[4]) && (g_irReceiveBuffer[7] == g_systemPara.szTermID[5]) && (g_irReceiveBuffer[8] == g_systemPara.szTermID[6]))//�жϱ��ַ
  {
    dataSum = sumCheck(g_irReceiveBuffer, (g_irReceiveByteNumberCount - 2));
    if(g_irReceiveBuffer[g_irReceiveByteNumberCount-2] == dataSum)
    {
      return 1;
    }
    else
      return 0;
  }
  else if((g_irReceiveBuffer[2] == 0xAA) && (g_irReceiveBuffer[3] == 0xAA) && (g_irReceiveBuffer[4] == 0xAA) && (g_irReceiveBuffer[5] == 0xAA) && (g_irReceiveBuffer[6] == 0xAA) && (g_irReceiveBuffer[7] == 0xAA) && (g_irReceiveBuffer[8] == 0xAA))//��д���ַ����
  {
    dataSum = sumCheck(g_irReceiveBuffer, (g_irReceiveByteNumberCount - 2));
    if(g_irReceiveBuffer[g_irReceiveByteNumberCount-2] == dataSum)
    {
      return 1;
    }
    else
      return 0;
  }  
  else
    return 0;
}

//�������CJ188����֡
uchar judgeIrCmdStartFlag = 0;
void doDecodeIR(void)
{
  g_irReceiveByteOkFlag = 1;//����һ���ֽڽ������
  
  g_irReceiveBuffer[g_irReceiveByteNumberCount++] = g_irReceiveCode;
  if(0 == judgeIrCmdStartFlag)//δ�ҵ�֡ͷ
  {
    if((g_irReceiveByteNumberCount >= 2) && (g_irReceiveBuffer[g_irReceiveByteNumberCount - 1] == 0x10) && (g_irReceiveBuffer[g_irReceiveByteNumberCount - 2] == 0x68))
    {
      judgeIrCmdStartFlag = 1;
      //memset(g_irReceiveBuffer,0,IR_MAX_RECEIVE_BUFFER);//������ջ��������
      g_irReceiveBuffer[0] = 0x68;
      g_irReceiveBuffer[1] = 0x10;
      g_irReceiveByteNumberCount = 2;
    }
    else if((g_irReceiveByteNumberCount >= 2) && (g_irReceiveBuffer[g_irReceiveByteNumberCount - 1] == 0x68))
    {
      g_irReceiveBuffer[0] = 0x68;
      g_irReceiveByteNumberCount = 1;
    }
    else if(g_irReceiveByteNumberCount >= 2)
    {
      g_irReceiveByteNumberCount = 0;
    }     
  }
  else//���ҵ�֡ͷ
  {
    if(g_irReceiveByteNumberCount >= 13)//2+7+2+0+2
    {
      if((g_irReceiveByteNumberCount == (13 + g_irReceiveBuffer[10])) && (g_irReceiveBuffer[g_irReceiveByteNumberCount - 1] == 0x16))//�ж�һ֡�Ƿ����
      {
        if( judgeIrCJ188FrameIsCorrect())
        {
          decodeIrReceiveCJ188Frame();
        }
        judgeIrCmdStartFlag = 0;
        memset(g_irReceiveBuffer,0,IR_MAX_RECEIVE_BUFFER);//������ջ��������
        g_irReceiveByteNumberCount = 0;        
      }
      else if(g_irReceiveByteNumberCount > (13 + g_irReceiveBuffer[10]))//�ж�֡����ʧ��
      {
        judgeIrCmdStartFlag = 0;
        memset(g_irReceiveBuffer,0,IR_MAX_RECEIVE_BUFFER);//������ջ��������
        g_irReceiveByteNumberCount = 0;
      }
    }
  }
}

 