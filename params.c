#include "params.h"


/*****************************************************
*The related parameters of step tariffing water meter.
*****************************************************/
struct tagSysPara    g_systemPara;

 WFMCOUNTINFO WFMCountInfo;
 WFM_INFO	WfmInfo;

/******************************************************
*The related parameters of water meter accumulation.
******************************************************/
struct tagAccumulationPara  g_accumulationPara;
unsigned long g_Turn_Number_Zheng = 0u, g_Turn_Number_Fu = 0u, g_Total_Number = 0u, g_Jishu_Number = 0u;

/******************************************************
*The related parameters of external trigger event.
******************************************************/
unsigned int g_externalTriggerEvent             = 0;


/******************************************************
*RTC��ر���
******************************************************/
struct tagRTC  g_tagRTC;
unsigned char g_rtcTempBuffer[7];
unsigned char g_rtcAlarmMinTime                         = 0;
unsigned char g_rtcAlarmHourTime                        = 0;


/******************************************************
*����ͨ��������ʾ��ر���
******************************************************/
unsigned char g_irReceiveBuffer[IR_MAX_RECEIVE_BUFFER];
unsigned char g_irSendBuffer[IR_MAX_SEND_BUFFER];
unsigned char g_irSendByteNumberCount            = 0;
unsigned char g_irReceiveByteNumberCount         = 0;
unsigned char g_irReceiveCode           = 0;
unsigned char g_irReceiveByteOkFlag     = 1;
unsigned char g_irReceiveOrSendFlag     = 0;


/******************************************************
*���ݴ洢��ر���
******************************************************/
unsigned char g_DataSave_Buffer[700];



/******************************************************
*The related parameters of CJ188 communication.
******************************************************/
unsigned char g_uartReceiveBuffer[UART_MAX_RECEIVE_BUFFER];
unsigned char g_uartSendBuffer[UART_MAX_SEND_BUFFER];
unsigned char g_uartSendByteNumberCount             = 0;
unsigned char g_uartReceiveByteNumberCount          = 0;
unsigned char g_uartReceiveCode                     = 0;  /// The UART receive byte
unsigned char g_uartCommunicationSuccessFlag        = 0;  /// communication with meter success or not flag
unsigned char g_uartCommunicationTimeoutFlag        = 0;  /// communication with meter timeout flag


/******************************************************
*The related parameters of SVD.
******************************************************/
unsigned char g_svdStartflag                     = 0;
unsigned char g_svdLowVoltageDetectNumber        = 0;
unsigned char g_batteryLevel                     = 5;


/******************************************************
*The related parameters of LCD.
******************************************************/
unsigned char g_displayState             = DISP_STAT_MAIN;
unsigned char g_hourDisplay                      = 0;
unsigned char g_dayDisplay                       = 0;
unsigned char g_monthDisplay                     = 0;
unsigned char g_yearDisplay                      = 0;
unsigned long g_accumulationTotalDisplay         = 0;


unsigned char g_KeyInteruptDown_Flag = 0;

/******************************************************
*The related parameters of TDC.
******************************************************/
volatile unsigned char g_tdcInterruptFlag                 = 0;
unsigned int  g_tdcStatusRegister                = 0;
unsigned long g_tdcTimeOutCount                  = 0;

double         g_calibrateResult                  = 0;
double         g_calibrateCorrectionFactor        = 0;

unsigned char g_numberOfTemperatureMeasure       = 1;
double         g_temperature1Result0              = 0;
double         g_temperature1Result1              = 0;
double         g_temperature1Result2              = 0;
double         g_temperature1Result3              = 0;
double         g_temperature2Result0              = 0;
double         g_temperature2Result1              = 0;
double         g_temperature2Result2              = 0;
double         g_temperature2Result3              = 0;
double         g_RhotByRref                       = 0;
double         g_RcoldByRref                      = 0;

double         g_timeResultup0                      = 0;
double         g_timeResultup1                      = 0;
double         g_timeResultup2                      = 0;
double         g_timeResultup3                      = 0;

double         g_timeResultdown0                    = 0;
double         g_timeResultdown1                    = 0;
double         g_timeResultdown2                    = 0;
double         g_timeResultdown3                    = 0;


double         g_timeResult0                      = 0;
double         g_timeResult1                      = 0;
double         g_timeResult2                      = 0;
double         g_timeResult3                      = 0;
double         g_averageTimeResultUp              = 0;
double         g_averageTimeResultDown            = 0;
double         g_PW1STValue                       = 0;
long         g_timeOfFlight                     = 0;
long         g_timeOfFlight_ave                 = 0;
long         g_timeOfFlightBuffer[100]           = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                                                           ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
 
long         tempValueBuffer[100]           = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                                                           ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

 long         g_tofDisplay                       = 0;
double         g_waterLineSpeed                   = 0;
double         g_waterSurfaceSpeed                = 0;
double         g_waterSurfaceSpeedDisp            = 0;
double         g_accumulationAmount               = 0;
double         g_tempAccumulationAmount           = 0;
unsigned char g_hitCount                         = 1;
unsigned char g_streamCount                      = 1;
unsigned char g_upStreamFlag                     = 0;
unsigned char g_upTimeOutFlag                    = 0;
unsigned char g_downTimeOutFlag                  = 0;
unsigned char g_oneSecondAccumulationFlag        = 0;
unsigned char g_startAccumulationFlag            = 0;
unsigned char g_startAccumulationCount           = 0;
unsigned char g_endAccumulationCount             = 0;

unsigned char g_valueOpenFlag                    = 0;//���ſ��ر�־
unsigned char g_valueOpenCount                   = 0;//���ſ��ر�־

unsigned int g_tempValue                         = 0;
double         g_tempHot                          = 0;
double         g_tempClod                         = 0;
unsigned int  g_tempTableP                       = 0;
