#ifndef IR_H
#define IR_H

#include "msp430x54x.h"

#define uchar unsigned char
#define uint  unsigned int

//CJ188控制码
//写命令
#define WriteData_CMD         0x04
#define WriteAdress_CMD       0x15
#define WriteCum_CMD          0x16

//读命令
#define ReadData_CMD          0x01
#define ReadAdress_CMD        0x03
#define ReadKeyVer_CMD        0x09

//写命令标识
#define WriteData_SysTime_DefineL                   0x15//写系统时间
#define WriteData_SysTime_DefineH                   0xA0



//读命令标识
#define ReadData_Month_DefineL                      0x79//读月冻结数据
#define ReadData_Month_DefineH                      0x81

#define ReadData_Day_DefineL                        0x7B//读日冻结数据
#define ReadData_Day_DefineH                        0x81

#define ReadData_SysTime_DefineL                    0x53//读系统时间
#define ReadData_SysTime_DefineH                    0x81

#define ReadData_Cum_DefineL                        0x1F//读计量数据
#define ReadData_Cum_DefineH                        0x90

#define ReadAdress_Adress_DefineL                   0x0A//读地址
#define ReadAdress_Adress_DefineH                   0x81
 
extern void doDecodeIR(void);

#endif