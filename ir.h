#ifndef IR_H
#define IR_H

#include "msp430x54x.h"

#define uchar unsigned char
#define uint  unsigned int

//CJ188������
//д����
#define WriteData_CMD         0x04
#define WriteAdress_CMD       0x15
#define WriteCum_CMD          0x16

//������
#define ReadData_CMD          0x01
#define ReadAdress_CMD        0x03
#define ReadKeyVer_CMD        0x09

//д�����ʶ
#define WriteData_SysTime_DefineL                   0x15//дϵͳʱ��
#define WriteData_SysTime_DefineH                   0xA0



//�������ʶ
#define ReadData_Month_DefineL                      0x79//���¶�������
#define ReadData_Month_DefineH                      0x81

#define ReadData_Day_DefineL                        0x7B//���ն�������
#define ReadData_Day_DefineH                        0x81

#define ReadData_SysTime_DefineL                    0x53//��ϵͳʱ��
#define ReadData_SysTime_DefineH                    0x81

#define ReadData_Cum_DefineL                        0x1F//����������
#define ReadData_Cum_DefineH                        0x90

#define ReadAdress_Adress_DefineL                   0x0A//����ַ
#define ReadAdress_Adress_DefineH                   0x81
 
extern void doDecodeIR(void);

#endif