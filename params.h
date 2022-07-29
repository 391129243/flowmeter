/***************************************************************************
 *
 * @file	params.h
 * @brief	This is system whole params header file(C17W15).
 *
 * @par		Copyright:
 *			Copyright (C) HS-WuTing 2015. All rights reserved.
 *
 * @warning -
 *
 * @par		History:
 *			- 15/07/2015    1.0
 *				- First revision.
 *
 **************************************************************************/

#ifndef PARAMS_H__
#define PARAMS_H__


struct tagSysPara
{
  unsigned char        szTermID[7];               /// Water meter address.
  //unsigned char        szUserID[32];              /// Water meter family code.
};
extern struct tagSysPara  g_systemPara;


#define	WATERFLOW_COEFF_MAX_COUNT 16

typedef struct {
	unsigned long 	TotalVolume; // 1L/10000
	unsigned long 	TotalVolumeDecimal;  //Decimal*10000
	unsigned long 	TotalVolumeSave; // 1L/10000
	unsigned long 	TotalVolumeDecimalSave;  //Decimal*10000        
	double	InstantFlowRate;	//当前流速
	double	InstantFrequence;	//当前频率
	unsigned short	InstantInterval;	//
} FLOW_ST;

typedef struct {
	unsigned char m_state;
	unsigned char m_direction;			//流向预设
	unsigned long m_upper_threshold;	//上限报警值
	unsigned long m_low_threshold;		//下限报警值
	unsigned char m_pulse_volume;		//脉冲输出设置 1： 0.01；2：0.001
	unsigned long m_freq[WATERFLOW_COEFF_MAX_COUNT];
	unsigned long m_velocity[WATERFLOW_COEFF_MAX_COUNT];
	long m_freq_buchang[WATERFLOW_COEFF_MAX_COUNT];
	unsigned short m_coeff_count;
} WFM_PARAMETER ;

typedef struct {
	unsigned char		State;
	WFM_PARAMETER		Config;
	FLOW_ST				Normal;
	FLOW_ST				Reverse;

	unsigned char 		WaterFlow_Direction;	//当前流向
	unsigned char 		WaterFlow_Warn;			//报警
	unsigned short		Interval;				//timer interval by msec
	double		TotalNormalCount;
	double		TotalReverseCount;
}WFM_INFO;

typedef struct {
	double Normal_SumCnt;
	double Reverse_SumCnt;
	unsigned short NormalDuration;
	unsigned short ReverseDuration;
	double NormalCntPre;
	double ReverseCntPre;
} WFMCOUNTINFO;

extern WFMCOUNTINFO WFMCountInfo;
extern WFM_INFO	WfmInfo;

/******************************************************
*The related parameters of water meter accumulation.
******************************************************/
struct tagAccumulationPara
{
  unsigned long accumulationBase;
  unsigned long accumulationPositive;
  unsigned long accumulationNegative;
  unsigned long accumulationTotal;
};
extern struct tagAccumulationPara  g_accumulationPara;

extern unsigned long g_Turn_Number_Zheng,g_Turn_Number_Fu,g_Total_Number,g_Jishu_Number;

/******************************************************
*The related parameters of external trigger event.
******************************************************/
/******************************************************
*外部触发事件相关变量
******************************************************/
#define TRI_IR            0x0001    //红外遥控按键按下
#define TRI_RTC           0x0002    //RTC中断
#define TRI_KEYPRESS_L    0x0004    //按键按下(短按)
#define TRI_KEYPRESS_LL   0x0008    //按键按下(中按)
#define TRI_KEYPRESS_LLL  0x0010    //按键按下(长按)


extern unsigned int g_externalTriggerEvent;


/******************************************************
*RTC相关变量
******************************************************/
struct tagRTC
{
  unsigned char g_Second;   
  unsigned char g_Minite;
  unsigned char g_Hour;   
  unsigned char g_Weekday; 
  unsigned char g_Day;   
  unsigned char g_Month;  
  unsigned char g_Year;
};
extern struct tagRTC  g_tagRTC;
extern unsigned char g_rtcTempBuffer[7];
extern unsigned char g_rtcAlarmMinTime;
extern unsigned char g_rtcAlarmHourTime;



/******************************************************
*红外通信命令显示相关变量
******************************************************/
#define  IR_MAX_RECEIVE_BUFFER    50  /// Define the size of IR receive buffer.
#define  IR_MAX_SEND_BUFFER       80  /// Define the size of IR send buffer.

extern unsigned char g_irReceiveBuffer[IR_MAX_RECEIVE_BUFFER];
extern unsigned char g_irSendBuffer[IR_MAX_SEND_BUFFER];
extern unsigned char g_irSendByteNumberCount;
extern unsigned char g_irReceiveByteNumberCount;
extern unsigned char g_irReceiveCode;
extern unsigned char g_irReceiveByteOkFlag;
extern unsigned char g_irReceiveOrSendFlag;


/******************************************************
*数据存储相关变量
******************************************************/
#define  Flash_Save_Buffer_Lenth       700  
extern unsigned char g_DataSave_Buffer[700];



/******************************************************
*The related parameters of CJ188 communication.
******************************************************/
#define  UART_MAX_RECEIVE_BUFFER    50  /// Define the size of UART receive buffer.
#define  UART_MAX_SEND_BUFFER       50  /// Define the size of UART send buffer.

extern unsigned char g_uartReceiveBuffer[UART_MAX_RECEIVE_BUFFER];
extern unsigned char g_uartSendBuffer[UART_MAX_SEND_BUFFER];
extern unsigned char g_uartSendByteNumberCount;
extern unsigned char g_uartReceiveByteNumberCount;
extern unsigned char g_uartReceiveCode;
extern unsigned char g_uartCommunicationSuccessFlag;
extern unsigned char g_uartCommunicationTimeoutFlag;


/******************************************************
*The related parameters of SVD.
******************************************************/
extern unsigned char g_svdStartflag;
extern unsigned char g_svdLowVoltageDetectNumber;
extern unsigned char g_batteryLevel;


/******************************************************
*The related parameters of LCD.
******************************************************/
#define DISP_STAT_CLOSE         0   /// Menu of close LCD
#define DISP_STAT_MAIN          1   /// Menu of main
#define DISP_STAT_TIME          2   /// Menu of time
#define DISP_STAT_TEMP          3   /// Menu of temp
#define DISP_STAT_TIMEF         4   /// Menu of time flight
#define DISP_STAT_TIMEU         5   /// Menu of time up
#define DISP_STAT_TIMED         6   /// Menu of time down
#define DISP_STAT_IR            7   /// Menu of ir

extern unsigned char g_displayState;

extern unsigned char g_hourDisplay;
extern unsigned char g_dayDisplay;
extern unsigned char g_monthDisplay;
extern unsigned char g_yearDisplay;
extern unsigned long g_accumulationTotalDisplay;

extern unsigned char g_KeyInteruptDown_Flag;


/******************************************************
*The related parameters of TDC.
******************************************************/
extern volatile unsigned char g_tdcInterruptFlag;
extern unsigned int  g_tdcStatusRegister;
extern unsigned long g_tdcTimeOutCount;

extern double         g_calibrateResult;
extern double         g_calibrateCorrectionFactor;

extern unsigned char g_numberOfTemperatureMeasure;
extern double         g_temperature1Result0;
extern double         g_temperature1Result1;
extern double         g_temperature1Result2;
extern double         g_temperature1Result3;
extern double         g_temperature2Result0;
extern double         g_temperature2Result1;
extern double         g_temperature2Result2;
extern double         g_temperature2Result3;
extern double         g_RhotByRref;
extern double         g_RcoldByRref;



extern double         g_timeResultup0;
extern double         g_timeResultup1;
extern double         g_timeResultup2;
extern double         g_timeResultup3;

extern double         g_timeResultdown0;
extern double         g_timeResultdown1;
extern double         g_timeResultdown2;
extern double         g_timeResultdown3;

extern double         g_timeResult0;
extern double         g_timeResult1;
extern double         g_timeResult2;
extern double         g_timeResult3;
extern double         g_averageTimeResultUp;
extern double         g_averageTimeResultDown;
extern double         g_PW1STValue;
extern long         g_timeOfFlight;
extern long         g_timeOfFlight_ave;

extern long         g_timeOfFlightBuffer[60];
extern long         tempValueBuffer[60];

extern double         g_timeOfFlightUpBuffer[60];
extern double         tempValueUpBuffer[60];

extern double         g_timeOfFlightDownBuffer[60];
extern double         tempValueDownBuffer[60];


extern long         g_tofDisplay;
extern double         g_waterLineSpeed;
extern double         g_waterSurfaceSpeed;
extern double         g_waterSurfaceSpeedDisp;
extern double         g_accumulationAmount;
extern double         g_tempAccumulationAmount;
extern unsigned char g_hitCount;
extern unsigned char g_streamCount;
extern unsigned char g_upStreamFlag;
extern unsigned char g_upTimeOutFlag;
extern unsigned char g_downTimeOutFlag;
extern unsigned char g_oneSecondAccumulationFlag;
extern unsigned char g_startAccumulationFlag;
extern unsigned char g_startAccumulationCount;
extern unsigned char g_endAccumulationCount;

extern unsigned char g_valueOpenFlag;//阀门开关标志
extern unsigned char g_valueOpenCount;//阀门开关标志

extern unsigned int  g_tempValue;
extern double         g_tempHot;
extern double         g_tempClod;
extern unsigned int  g_tempTableP;

#endif  // PARAMS_H__
