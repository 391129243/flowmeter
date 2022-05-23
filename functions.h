#ifndef FUNCTIONS_H__
#define FUNCTIONS_H__

#include "msp430x54x.h"

#define uchar unsigned char
#define uint  unsigned int

#define CPU           (8000000)         //主时钟为8M时钟
#define delay_us(x)   (__delay_cycles((double)x*CPU/1000000.0))
#define delay_ms(x)   (__delay_cycles((double)x*CPU/1000.0))
#define dely_s(x)     (__delay_cycles((double)x*CPU/1.0))


extern char startWith(unsigned char * p1,unsigned char * p2);
extern unsigned char dectoBCD(unsigned char dec);
extern unsigned char BCDtoDec(unsigned char bcd);
extern double dotHextoDotDec(unsigned long dotHex);
extern unsigned char sumCheck(unsigned char *p1, unsigned char len);
extern uchar reverseHighLowBit(uchar  dat);
extern unsigned char CharChange(unsigned char  dat);
unsigned int ChangeLadderWaterData(unsigned char data1, unsigned char data2, unsigned char data3);
unsigned char Reverse_ChangeLadderWaterData(unsigned int data1, unsigned char num);

#endif	// FUNCTIONS_H__
