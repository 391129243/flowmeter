#ifndef TIMERA1_H
#define TIMERA1_H

#include "msp430x54x.h"

#define uchar unsigned char
#define uint  unsigned int


extern void initTimerA1(void);
extern void startTimerA1(uint interval);
extern void stopTimerA1(void);
extern void enableTimerA1Interrupt(void);
extern void disableTimerA1Interrupt(void);
//extern void doCollectDataEvent(void);

#endif