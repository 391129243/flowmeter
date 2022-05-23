#ifndef DATASAVE_H
#define DATASAVE_H

#include "msp430x54x.h"

#define uchar unsigned char
#define uint  unsigned int

// System_Params_ADDR
#define MaterAddressSaveAddress                     0
#define MaterAddressSize                            7

// WaterCum_Data_ADDR
#define TotalVolumeDecimalSaveAddress               0
#define TotalVolumeDecimalSize                      4
#define TotalVolumeSaveAddress                      TotalVolumeDecimalSaveAddress+TotalVolumeDecimalSize
#define TotalVolumeSize                             4

// MonthDayCumData__ADDR
#define MonthDataSaveAddress                        0
#define MonthDataSize                               8
#define DayDataSaveAddress                          MonthDataSaveAddress+96
#define DayDataSize                                 8

void Save_WaterMeterAddress(void);
void Read_WaterMeterAddress(void);

void Save_WaterCumData(void);
void Read_WaterCumData(void);

void saveMonthCum(void);
uchar readMonthCum(uchar addr);

void saveDayCum(void);
uchar readDayCum(uchar addr);

void readSystemParam( void );


#endif