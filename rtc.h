#ifndef RTC_H
#define RTC_H

#include "msp430x54x.h"

#define uchar unsigned char
#define uint  unsigned int

extern void initRTC(void) ;
extern void enableRtcAlarmInterrupt(void);
extern void disableRtcAlarmInterrupt(void);
extern void enableMinuteInterrupt(void);
extern void disableMinuteInterrupt(void);
extern void rtcSetDateTimeBCD(uchar *data);
extern void rtcGetDateTimeBCD(uchar *data);
extern void rtcSetAlarmTimeBCD(uchar amin, uchar ahour);
extern void doRtcEvent(void);

#endif