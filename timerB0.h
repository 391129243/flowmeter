#ifndef TIMERB0_H
#define TIMERB0_H

#include "msp430x54x.h"

#define uchar unsigned char
#define uint  unsigned int

#define IR_POWER_CTL_OUTPUT        P10DIR |= BIT6 
#define IR_POWER_ON                P10OUT &= ~BIT6
#define IR_POWER_OFF               P10OUT |= BIT6

#define IR_CTL_INPUT               P1DIR &= ~BIT0
#define IR_IN_PORT                 P1IN                   
#define IR_PIN_NUM                 BIT0
#define IR_INT_ENABLE              P1IE |= BIT0
#define IR_INT_DISABLE             P1IE &= ~BIT0
#define IR_INT_SIGN_DOWN           P1IES |= BIT0


extern void initIR(void);
extern void powerOnIR(void);
extern void powerCloseIR(void);
extern void IRRecPulseDown(void);
extern void Start_IRSend_PWM(void);
extern void Reset_IR_Param(void);

#endif