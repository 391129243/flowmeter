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

//红外发送缓冲区初始化
void Init_IR_Send_Buf(void)
{
  g_irSendBuffer[0] = 0x68;//帧起始符
  g_irSendBuffer[1] = 0x10;//仪表类型（水表）
  g_irSendBuffer[2] = g_systemPara.szTermID[0];//表ID号（七个字节）
  g_irSendBuffer[3] = g_systemPara.szTermID[1];
  g_irSendBuffer[4] = g_systemPara.szTermID[2];
  g_irSendBuffer[5] = g_systemPara.szTermID[3];
  g_irSendBuffer[6] = g_systemPara.szTermID[4];
  g_irSendBuffer[7] = g_systemPara.szTermID[5];
  g_irSendBuffer[8] = g_systemPara.szTermID[6];
}

void Init_IR_Send_Buf_ReadMeterAddress(void)
{
  g_irSendBuffer[0] = 0x68;//帧起始符
  g_irSendBuffer[1] = 0x10;//仪表类型（水表）
  g_irSendBuffer[2] = g_systemPara.szTermID[0];//表地址（七个字节）
  g_irSendBuffer[3] = g_systemPara.szTermID[1];
  g_irSendBuffer[4] = g_systemPara.szTermID[2];
  g_irSendBuffer[5] = g_systemPara.szTermID[3];
  g_irSendBuffer[6] = g_systemPara.szTermID[4];
  g_irSendBuffer[7] = g_systemPara.szTermID[5];
  g_irSendBuffer[8] = g_systemPara.szTermID[6];
}


//解析接收的irCJ188数据帧
void decodeIrReceiveCJ188Frame(void)
{
  switch (g_irReceiveBuffer[9])
  {
    //write
    case WriteData_CMD:
    {
      Init_IR_Send_Buf();//设定发送帧的固定字节
      g_irSendBuffer[12] = 0xA0;
      g_irSendBuffer[13] = 0x01;
      g_irSendBuffer[14] = 0x00;
         
      if((WriteData_SysTime_DefineL == g_irReceiveBuffer[11]) && (WriteData_SysTime_DefineH == g_irReceiveBuffer[12]))//设系统时间
      {
        if((g_irReceiveBuffer[14] >= 0x60) || (g_irReceiveBuffer[15] >= 0x60) || (g_irReceiveBuffer[16] >= 0x25) || (g_irReceiveBuffer[17] >= 0x32) || (g_irReceiveBuffer[17] <= 0x00) || (g_irReceiveBuffer[18] >= 0x13) || (g_irReceiveBuffer[18] <= 0x00) || (g_irReceiveBuffer[19] <= 0x13))//系统时间无效判断
        {
          g_irSendBuffer[9] = 0xC4;//控制码，异常
        }
        else//有效
        {
          g_rtcTempBuffer[0] = g_irReceiveBuffer[14];   //second
          g_rtcTempBuffer[1] = g_irReceiveBuffer[15];   //minute
          g_rtcTempBuffer[2] = g_irReceiveBuffer[16];   //hour
          g_rtcTempBuffer[3] = 0;                       //weekday
          g_rtcTempBuffer[4] = g_irReceiveBuffer[17];   //day
          g_rtcTempBuffer[5] = g_irReceiveBuffer[18];   //month
          g_rtcTempBuffer[6] = g_irReceiveBuffer[19];   //year     
          
          rtcSetDateTimeBCD(g_rtcTempBuffer);
            
          //回系统时间设定成功命令
          g_irSendBuffer[9] = 0x84;//控制码        
        }
        g_irSendBuffer[10] = 0x03;
        g_irSendBuffer[11] = 0x15;
        g_irSendBuffer[15] = 0x16;
        g_irSendByteNumberCount = 16;
        g_irSendBuffer[14] = sumCheck(g_irSendBuffer,(g_irSendByteNumberCount -2));//和效验位
      }
      else//error
      {
        return;
      }   
      Start_IRSend_PWM();//开始一帧的红外发送   
      break;
    }

   //read
  case ReadData_CMD: 
    {
      Init_IR_Send_Buf();//设定发送帧的固定字节
      g_irSendBuffer[9] = 0x81;
      g_irSendBuffer[13] = 0x01;      
   
      if((ReadData_Month_DefineL == g_irReceiveBuffer[11]) && (ReadData_Month_DefineH == g_irReceiveBuffer[12]))//读月冻结数据
      {
        uchar flag;
        flag = readMonthCum(g_irReceiveBuffer[14]);
        if(flag==0)//异常
        {
          g_irSendBuffer[9] = 0xC1;
          g_irSendBuffer[10] = 0x03;
          g_irSendBuffer[11] = 0x79;
          g_irSendBuffer[12] = 0x81; 
          g_irSendBuffer[13] = 0x01;
          g_irSendBuffer[14] = 0x00;
          g_irSendBuffer[15] = 0x16;
          g_irSendByteNumberCount = 16;
          g_irSendBuffer[14] = sumCheck(g_irSendBuffer,(g_irSendByteNumberCount -2));//和效验位            
        }
        else//正常响应
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
          g_irSendBuffer[18] = 0x2A;//两位小数        
          g_irSendBuffer[19] = 0x00;
          g_irSendBuffer[20] = 0x16;
          g_irSendByteNumberCount = 21;
          g_irSendBuffer[19] = sumCheck(g_irSendBuffer,(g_irSendByteNumberCount -2));//和效验位             
        }
      }  
      else if((ReadData_Day_DefineL == g_irReceiveBuffer[11]) && (ReadData_Day_DefineH == g_irReceiveBuffer[12]))//读日冻结数据
      {
        uchar flag;
        flag = readDayCum(g_irReceiveBuffer[14]);
        if(flag==0)//异常
        {
          g_irSendBuffer[9] = 0xC1;
          g_irSendBuffer[10] = 0x03;
          g_irSendBuffer[11] = 0x7B;
          g_irSendBuffer[12] = 0x81; 
          g_irSendBuffer[13] = 0x01;
          g_irSendBuffer[14] = 0x00;
          g_irSendBuffer[15] = 0x16;
          g_irSendByteNumberCount = 16;
          g_irSendBuffer[14] = sumCheck(g_irSendBuffer,(g_irSendByteNumberCount -2));//和效验位            
        }
        else//正常响应
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
          g_irSendBuffer[18] = 0x2A;//两位小数                    
          g_irSendBuffer[19] = 0x00;
          g_irSendBuffer[20] = 0x16;
          g_irSendByteNumberCount = 21;
          g_irSendBuffer[19] = sumCheck(g_irSendBuffer,(g_irSendByteNumberCount -2));//和效验位             
        }        
      }  
      else if((ReadData_SysTime_DefineL == g_irReceiveBuffer[11]) && (ReadData_SysTime_DefineH == g_irReceiveBuffer[12]))//读系统时间
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
        g_irSendBuffer[20] = sumCheck(g_irSendBuffer,(g_irSendByteNumberCount -2));//和效验位         
      }
      else if((ReadData_Cum_DefineL == g_irReceiveBuffer[11]) && (ReadData_Cum_DefineH == g_irReceiveBuffer[12]))//读计量数据
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
        g_irSendBuffer[18] = 0x2A;//两位小数
        
        temp = (uchar)(WfmInfo.Normal.TotalVolumeDecimal%100);
        g_irSendBuffer[19] = dectoBCD(temp);
        temp = (uchar)((WfmInfo.Normal.TotalVolumeDecimal%10000)/100);
        g_irSendBuffer[20] = dectoBCD(temp);
        temp = (uchar)((WfmInfo.Normal.TotalVolumeDecimal%1000000)/10000);
        g_irSendBuffer[21] = dectoBCD(temp);
        temp = (uchar)(WfmInfo.Normal.TotalVolumeDecimal/1000000);
        g_irSendBuffer[22] = dectoBCD(temp);
        g_irSendBuffer[23] = 0x2A;//两位小数
        
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
        g_irSendBuffer[33] = sumCheck(g_irSendBuffer,(g_irSendByteNumberCount -2));//和效验位
      }     
      Start_IRSend_PWM();//开始一帧的红外发送
      break;
    }
  case ReadAdress_CMD: 
    {
      if((ReadAdress_Adress_DefineL == g_irReceiveBuffer[11]) && (ReadAdress_Adress_DefineH == g_irReceiveBuffer[12]))//读表地址
      {
        Init_IR_Send_Buf_ReadMeterAddress();//设定发送帧的固定字节
        g_irSendBuffer[9] = 0x83;
        g_irSendBuffer[10] = 0x03;
        g_irSendBuffer[11] = 0x0A;
        g_irSendBuffer[12] = 0x81;
        g_irSendBuffer[13] = 0x02;
        g_irSendBuffer[14] = 0x00;
        g_irSendBuffer[15] = 0x16;
        g_irSendByteNumberCount = 16;
        g_irSendBuffer[14] = sumCheck(g_irSendBuffer,(g_irSendByteNumberCount -2));//和效验位
        
      }
      else//error
      {
        return;
      } 
      Start_IRSend_PWM();//开始一帧的红外发送
      break;
    }
  }
}

//irCJ188数据帧表地址、和效验判断
uchar judgeIrCJ188FrameIsCorrect(void)
{
  uchar dataSum;
  if((g_irReceiveBuffer[2] == g_systemPara.szTermID[0]) && (g_irReceiveBuffer[3] == g_systemPara.szTermID[1]) && (g_irReceiveBuffer[4] == g_systemPara.szTermID[2]) && (g_irReceiveBuffer[5] == g_systemPara.szTermID[3]) && (g_irReceiveBuffer[6] == g_systemPara.szTermID[4]) && (g_irReceiveBuffer[7] == g_systemPara.szTermID[5]) && (g_irReceiveBuffer[8] == g_systemPara.szTermID[6]))//判断表地址
  {
    dataSum = sumCheck(g_irReceiveBuffer, (g_irReceiveByteNumberCount - 2));
    if(g_irReceiveBuffer[g_irReceiveByteNumberCount-2] == dataSum)
    {
      return 1;
    }
    else
      return 0;
  }
  else if((g_irReceiveBuffer[2] == 0xAA) && (g_irReceiveBuffer[3] == 0xAA) && (g_irReceiveBuffer[4] == 0xAA) && (g_irReceiveBuffer[5] == 0xAA) && (g_irReceiveBuffer[6] == 0xAA) && (g_irReceiveBuffer[7] == 0xAA) && (g_irReceiveBuffer[8] == 0xAA))//读写表地址命令
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

//红外接收CJ188数据帧
uchar judgeIrCmdStartFlag = 0;
void doDecodeIR(void)
{
  g_irReceiveByteOkFlag = 1;//红外一个字节接收完成
  
  g_irReceiveBuffer[g_irReceiveByteNumberCount++] = g_irReceiveCode;
  if(0 == judgeIrCmdStartFlag)//未找到帧头
  {
    if((g_irReceiveByteNumberCount >= 2) && (g_irReceiveBuffer[g_irReceiveByteNumberCount - 1] == 0x10) && (g_irReceiveBuffer[g_irReceiveByteNumberCount - 2] == 0x68))
    {
      judgeIrCmdStartFlag = 1;
      //memset(g_irReceiveBuffer,0,IR_MAX_RECEIVE_BUFFER);//红外接收缓冲区清空
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
  else//已找到帧头
  {
    if(g_irReceiveByteNumberCount >= 13)//2+7+2+0+2
    {
      if((g_irReceiveByteNumberCount == (13 + g_irReceiveBuffer[10])) && (g_irReceiveBuffer[g_irReceiveByteNumberCount - 1] == 0x16))//判断一帧是否结束
      {
        if( judgeIrCJ188FrameIsCorrect())
        {
          decodeIrReceiveCJ188Frame();
        }
        judgeIrCmdStartFlag = 0;
        memset(g_irReceiveBuffer,0,IR_MAX_RECEIVE_BUFFER);//红外接收缓冲区清空
        g_irReceiveByteNumberCount = 0;        
      }
      else if(g_irReceiveByteNumberCount > (13 + g_irReceiveBuffer[10]))//判断帧接收失败
      {
        judgeIrCmdStartFlag = 0;
        memset(g_irReceiveBuffer,0,IR_MAX_RECEIVE_BUFFER);//红外接收缓冲区清空
        g_irReceiveByteNumberCount = 0;
      }
    }
  }
}

 