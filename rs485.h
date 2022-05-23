#ifndef RS485_H
#define RS485_H


//cj188 read data command
#define cj188ReadDataCommand                       0x01
#define cj188ReadMeterAddressCommand               0x03
#define cj188ReadDataCommandAck                    0x81
#define cj188ReadMeterAddressCommandAck            0x83

//cj188 read command ID
#define cj188ReadMeterBaseAccumulationLowByte      0x1F  /// Read water meter base Accumulation.
#define cj188ReadMeterBaseAccumulationHighByte     0x90

#define cj188ReadMeterAddressLowByte               0x0A  /// Read water meter address.
#define cj188ReadMeterAddressHighByte              0x81

extern void initRS485(void);
extern void doCj188Decode(void);

extern void Print_Byte(unsigned char ch);
extern void Print_String(char *str);
extern void Print_Text1(float timeValue11);
extern void Print_TimeOut(void);
extern void Print_Information(void);

#endif