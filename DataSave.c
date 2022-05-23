#include "msp430x54x.h"
#include "functions.h"
#include "dataSave.h"
#include "params.h"
#include "flash.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"



//存储表地址
void Save_WaterMeterAddress(void)
{
  SPI_Flash_PowerOn();
  _DINT();//写flash前，先关闭中断
  SPI_Flash_Read_Buffer(System_Params_ADDR,(unsigned char *)g_DataSave_Buffer,Flash_Save_Buffer_Lenth);
  for( uchar i = 0; i < 7; i++ )
  {
    g_DataSave_Buffer[MaterAddressSaveAddress+i] = g_systemPara.szTermID[i];
  }  
  SPI_Flash_Erase_Sector(System_Params_ADDR);
  SPI_Flash_Write_Buffer(System_Params_ADDR,(unsigned char *)g_DataSave_Buffer,Flash_Save_Buffer_Lenth);
  _EINT();//写flash完成，重新开启中断
  SPI_Flash_PowerOff();
}

//读表地址
void Read_WaterMeterAddress(void)
{
  SPI_Flash_PowerOn();
  _DINT();//写flash前，先关闭中断
  SPI_Flash_Read_Buffer(System_Params_ADDR,(unsigned char *)g_DataSave_Buffer,Flash_Save_Buffer_Lenth);
  for( uchar i = 0; i < 7; i++ )
  {
    g_systemPara.szTermID[i] = g_DataSave_Buffer[MaterAddressSaveAddress+i];
  }
  _EINT();//写flash完成，重新开启中断
  SPI_Flash_PowerOff();
}

//存储累计量、字轮基数、正转量、反转量
void Save_WaterCumData(void)
{
  SPI_Flash_PowerOn();
  _DINT();//写flash前，先关闭中断
  SPI_Flash_Read_Buffer(WaterCum_Data_ADDR,(unsigned char *)g_DataSave_Buffer,Flash_Save_Buffer_Lenth);
  
  g_DataSave_Buffer[TotalVolumeDecimalSaveAddress+0] = (uchar)(WfmInfo.Normal.TotalVolumeDecimal); 
  g_DataSave_Buffer[TotalVolumeDecimalSaveAddress+1] = (uchar)(WfmInfo.Normal.TotalVolumeDecimal>>8);
  g_DataSave_Buffer[TotalVolumeDecimalSaveAddress+2] = (uchar)(WfmInfo.Normal.TotalVolumeDecimal>>16);
  g_DataSave_Buffer[TotalVolumeDecimalSaveAddress+3] = (uchar)(WfmInfo.Normal.TotalVolumeDecimal>>24);
  
  g_DataSave_Buffer[TotalVolumeSaveAddress+0] = (uchar)(WfmInfo.Normal.TotalVolume); 
  g_DataSave_Buffer[TotalVolumeSaveAddress+1] = (uchar)(WfmInfo.Normal.TotalVolume>>8);
  g_DataSave_Buffer[TotalVolumeSaveAddress+2] = (uchar)(WfmInfo.Normal.TotalVolume>>16);
  g_DataSave_Buffer[TotalVolumeSaveAddress+3] = (uchar)(WfmInfo.Normal.TotalVolume>>24);
  
  SPI_Flash_Erase_Sector(WaterCum_Data_ADDR);
  SPI_Flash_Write_Buffer(WaterCum_Data_ADDR,(unsigned char *)g_DataSave_Buffer,Flash_Save_Buffer_Lenth);
  _EINT();//写flash完成，重新开启中断
  SPI_Flash_PowerOff();
}

//读储累计量、字轮基数、正转量、反转量
void Read_WaterCumData(void)
{
  SPI_Flash_PowerOn();
  _DINT();//写flash前，先关闭中断
  SPI_Flash_Read_Buffer(WaterCum_Data_ADDR,(unsigned char *)g_DataSave_Buffer,Flash_Save_Buffer_Lenth);
  
  WfmInfo.Normal.TotalVolumeDecimal <<= 8;
  WfmInfo.Normal.TotalVolumeDecimal |= g_DataSave_Buffer[TotalVolumeDecimalSaveAddress+3];
  WfmInfo.Normal.TotalVolumeDecimal <<= 8;
  WfmInfo.Normal.TotalVolumeDecimal |= g_DataSave_Buffer[TotalVolumeDecimalSaveAddress+2];
  WfmInfo.Normal.TotalVolumeDecimal <<= 8;
  WfmInfo.Normal.TotalVolumeDecimal |= g_DataSave_Buffer[TotalVolumeDecimalSaveAddress+1];
  WfmInfo.Normal.TotalVolumeDecimal <<= 8;
  WfmInfo.Normal.TotalVolumeDecimal |= g_DataSave_Buffer[TotalVolumeDecimalSaveAddress+0];
  
  WfmInfo.Normal.TotalVolume <<= 8;
  WfmInfo.Normal.TotalVolume |= g_DataSave_Buffer[TotalVolumeSaveAddress+3];
  WfmInfo.Normal.TotalVolume <<= 8;
  WfmInfo.Normal.TotalVolume |= g_DataSave_Buffer[TotalVolumeSaveAddress+2];
  WfmInfo.Normal.TotalVolume <<= 8;
  WfmInfo.Normal.TotalVolume |= g_DataSave_Buffer[TotalVolumeSaveAddress+1];
  WfmInfo.Normal.TotalVolume <<= 8;
  WfmInfo.Normal.TotalVolume |= g_DataSave_Buffer[TotalVolumeSaveAddress+0];  
  
  _EINT();//写flash完成，重新开启中断
  SPI_Flash_PowerOff();
}


//save Month Cum
void saveMonthCum(void)
{
  uint addr1;
  
  SPI_Flash_PowerOn();
  _DINT();//写flash前，先关闭中断
  SPI_Flash_Read_Buffer(MonthDayCumData__ADDR,(unsigned char *)g_DataSave_Buffer,Flash_Save_Buffer_Lenth);
  
  switch(g_tagRTC.g_Month)
  {
  case 0x01:
    addr1 = MonthDataSaveAddress+MonthDataSize*11; //存12月份的冻结 
    break;
  case 0x02:
    addr1 = MonthDataSaveAddress+MonthDataSize*0;//存1月份的冻结
    break;   
  case 0x03:
    addr1 = MonthDataSaveAddress+MonthDataSize*1;//存2月份的冻结
    break;
  case 0x04:
    addr1 = MonthDataSaveAddress+MonthDataSize*2;//存3月份的冻结
    break;
  case 0x05:
    addr1 = MonthDataSaveAddress+MonthDataSize*3;//存4月份的冻结
    break;
  case 0x06:
    addr1 = MonthDataSaveAddress+MonthDataSize*4;//存5月份的冻结
    break; 
  case 0x07:
    addr1 = MonthDataSaveAddress+MonthDataSize*5;//存6月份的冻结
    break;
  case 0x08:
    addr1 = MonthDataSaveAddress+MonthDataSize*6;//存7月份的冻结
    break; 
  case 0x09:
    addr1 = MonthDataSaveAddress+MonthDataSize*7;//存8月份的冻结
    break;
  case 0x10:
    addr1 = MonthDataSaveAddress+MonthDataSize*8;//存9月份的冻结
    break; 
  case 0x11:
    addr1 = MonthDataSaveAddress+MonthDataSize*9;//存10月份的冻结
    break;
  case 0x12:
    addr1 = MonthDataSaveAddress+MonthDataSize*10;//存11月份的冻结
    break; 
  default:
    break;    
  }
  
  g_DataSave_Buffer[addr1+0] = (uchar)(WfmInfo.Normal.TotalVolumeDecimal); 
  g_DataSave_Buffer[addr1+1] = (uchar)(WfmInfo.Normal.TotalVolumeDecimal>>8);
  g_DataSave_Buffer[addr1+2] = (uchar)(WfmInfo.Normal.TotalVolumeDecimal>>16);
  g_DataSave_Buffer[addr1+3] = (uchar)(WfmInfo.Normal.TotalVolumeDecimal>>24);
  
  g_DataSave_Buffer[addr1+4] = (uchar)(WfmInfo.Normal.TotalVolume); 
  g_DataSave_Buffer[addr1+5] = (uchar)(WfmInfo.Normal.TotalVolume>>8);
  g_DataSave_Buffer[addr1+6] = (uchar)(WfmInfo.Normal.TotalVolume>>16);
  g_DataSave_Buffer[addr1+7] = (uchar)(WfmInfo.Normal.TotalVolume>>24);  
    
  SPI_Flash_Erase_Sector(MonthDayCumData__ADDR);
  SPI_Flash_Write_Buffer(MonthDayCumData__ADDR,(unsigned char *)g_DataSave_Buffer,Flash_Save_Buffer_Lenth);
  _EINT();//写flash完成，重新开启中断
  SPI_Flash_PowerOff();  
}

//read Month Cum
uchar readMonthCum(uchar addr)
{
  uchar ret = 1;
  uint addr1;
 
  SPI_Flash_PowerOn();
  _DINT();//写flash前，先关闭中断
  SPI_Flash_Read_Buffer(MonthDayCumData__ADDR,(unsigned char *)g_DataSave_Buffer,Flash_Save_Buffer_Lenth);
  
  switch(addr)
  {
  case 0x01:
    addr1 = MonthDataSaveAddress+MonthDataSize*0;//读1月份的冻结
    break;
  case 0x02:
    addr1 = MonthDataSaveAddress+MonthDataSize*1;//读2月份的冻结
    break;   
  case 0x03:
    addr1 = MonthDataSaveAddress+MonthDataSize*2;//读3月份的冻结
    break;
  case 0x04:
    addr1 = MonthDataSaveAddress+MonthDataSize*3;//读4月份的冻结
    break;
  case 0x05:
    addr1 = MonthDataSaveAddress+MonthDataSize*4;//读5月份的冻结
    break;
  case 0x06:
    addr1 = MonthDataSaveAddress+MonthDataSize*5;//读6月份的冻结
    break; 
  case 0x07:
    addr1 = MonthDataSaveAddress+MonthDataSize*6;//读7月份的冻结
    break;
  case 0x08:
    addr1 = MonthDataSaveAddress+MonthDataSize*7;//读8月份的冻结
    break; 
  case 0x09:
    addr1 = MonthDataSaveAddress+MonthDataSize*8;//读9月份的冻结
    break;
  case 0x10:
    addr1 = MonthDataSaveAddress+MonthDataSize*9;//读10月份的冻结
    break; 
  case 0x11:
    addr1 = MonthDataSaveAddress+MonthDataSize*10;//读11月份的冻结
    break;
  case 0x12:
    addr1 = MonthDataSaveAddress+MonthDataSize*11;//读12月份的冻结
    break; 
  default:
    ret = 0;
    break;    
  }  

  WfmInfo.Normal.TotalVolumeDecimalSave <<= 8;
  WfmInfo.Normal.TotalVolumeDecimalSave |= g_DataSave_Buffer[addr1+3];
  WfmInfo.Normal.TotalVolumeDecimalSave <<= 8;
  WfmInfo.Normal.TotalVolumeDecimalSave |= g_DataSave_Buffer[addr1+2];
  WfmInfo.Normal.TotalVolumeDecimalSave <<= 8;
  WfmInfo.Normal.TotalVolumeDecimalSave |= g_DataSave_Buffer[addr1+1];
  WfmInfo.Normal.TotalVolumeDecimalSave <<= 8;
  WfmInfo.Normal.TotalVolumeDecimalSave |= g_DataSave_Buffer[addr1+0];
  
  WfmInfo.Normal.TotalVolumeSave <<= 8;
  WfmInfo.Normal.TotalVolumeSave |= g_DataSave_Buffer[addr1+7];
  WfmInfo.Normal.TotalVolumeSave <<= 8;
  WfmInfo.Normal.TotalVolumeSave |= g_DataSave_Buffer[addr1+6];
  WfmInfo.Normal.TotalVolumeSave <<= 8;
  WfmInfo.Normal.TotalVolumeSave |= g_DataSave_Buffer[addr1+5];
  WfmInfo.Normal.TotalVolumeSave <<= 8;
  WfmInfo.Normal.TotalVolumeSave |= g_DataSave_Buffer[addr1+4];   
  
  _EINT();//写flash完成，重新开启中断
  SPI_Flash_PowerOff();  
  
  return ret;
}

//save Day Cum
void saveDayCum(void)
{
  uint addr1;
  
  SPI_Flash_PowerOn();
  _DINT();//写flash前，先关闭中断
  SPI_Flash_Read_Buffer(MonthDayCumData__ADDR,(unsigned char *)g_DataSave_Buffer,Flash_Save_Buffer_Lenth);  
  
  switch(g_tagRTC.g_Day)
  {
  case 0x01:
    addr1 = DayDataSaveAddress+DayDataSize*0;//存1日的冻结
    break;
  case 0x02:
    addr1 = DayDataSaveAddress+DayDataSize*1;//存2日的冻结
    break;   
  case 0x03:
    addr1 = DayDataSaveAddress+DayDataSize*2;//存3日的冻结
    break;
  case 0x04:
    addr1 = DayDataSaveAddress+DayDataSize*3;//存4日的冻结
    break;
  case 0x05:
    addr1 = DayDataSaveAddress+DayDataSize*4;//存5日的冻结
    break;
  case 0x06:
    addr1 = DayDataSaveAddress+DayDataSize*5;//存6日的冻结
    break; 
  case 0x07:
    addr1 = DayDataSaveAddress+DayDataSize*6;//存7日的冻结
    break;
  case 0x08:
    addr1 = DayDataSaveAddress+DayDataSize*7;//存8日的冻结
    break; 
  case 0x09:
    addr1 = DayDataSaveAddress+DayDataSize*8;//存9日的冻结
    break;
  case 0x10:
    addr1 = DayDataSaveAddress+DayDataSize*9;//存10日的冻结
    break; 
  case 0x11:
    addr1 = DayDataSaveAddress+DayDataSize*10;//存11日的冻结
    break;
  case 0x12:
    addr1 = DayDataSaveAddress+DayDataSize*11;//存12日的冻结
    break; 
  case 0x13:
    addr1 = DayDataSaveAddress+DayDataSize*12;//存13日的冻结
    break; 
  case 0x14:
    addr1 = DayDataSaveAddress+DayDataSize*13;//存14日的冻结
    break; 
  case 0x15:
    addr1 = DayDataSaveAddress+DayDataSize*14;//存15日的冻结
    break;
  case 0x16:
    addr1 = DayDataSaveAddress+DayDataSize*15;//存16日的冻结
    break; 
  case 0x17:
    addr1 = DayDataSaveAddress+DayDataSize*16;//存17日的冻结
    break;
  case 0x18:
    addr1 = DayDataSaveAddress+DayDataSize*17;//存18日的冻结
    break; 
  case 0x19:
    addr1 = DayDataSaveAddress+DayDataSize*18;//存19日的冻结
    break;
  case 0x20:
    addr1 = DayDataSaveAddress+DayDataSize*19;//存20日的冻结
    break; 
  case 0x21:
    addr1 = DayDataSaveAddress+DayDataSize*20;//存21日的冻结
    break; 
   case 0x22:
    addr1 = DayDataSaveAddress+DayDataSize*21;//存22日的冻结
    break;
   case 0x23:
    addr1 = DayDataSaveAddress+DayDataSize*22;//存23日的冻结
    break; 
  case 0x24:
    addr1 = DayDataSaveAddress+DayDataSize*23;//存24日的冻结
    break; 
  case 0x25:
    addr1 = DayDataSaveAddress+DayDataSize*24;//存25日的冻结
    break; 
  case 0x26:
    addr1 = DayDataSaveAddress+DayDataSize*25;//存26日的冻结
    break;
  case 0x27:
    addr1 = DayDataSaveAddress+DayDataSize*26;//存27日的冻结
    break;      
  case 0x28:
    addr1 = DayDataSaveAddress+DayDataSize*27;//存28日的冻结
    break; 
  case 0x29:
    addr1 = DayDataSaveAddress+DayDataSize*28;//存29日的冻结
    break;  
  case 0x30:
    addr1 = DayDataSaveAddress+DayDataSize*29;//存30日的冻结
    break; 
  case 0x31:
    addr1 = DayDataSaveAddress+DayDataSize*30;//存31日的冻结
    break;      
  default:
    break;    
  }  
  
  g_DataSave_Buffer[addr1+0] = (uchar)(WfmInfo.Normal.TotalVolumeDecimal); 
  g_DataSave_Buffer[addr1+1] = (uchar)(WfmInfo.Normal.TotalVolumeDecimal>>8);
  g_DataSave_Buffer[addr1+2] = (uchar)(WfmInfo.Normal.TotalVolumeDecimal>>16);
  g_DataSave_Buffer[addr1+3] = (uchar)(WfmInfo.Normal.TotalVolumeDecimal>>24);
  
  g_DataSave_Buffer[addr1+4] = (uchar)(WfmInfo.Normal.TotalVolume); 
  g_DataSave_Buffer[addr1+5] = (uchar)(WfmInfo.Normal.TotalVolume>>8);
  g_DataSave_Buffer[addr1+6] = (uchar)(WfmInfo.Normal.TotalVolume>>16);
  g_DataSave_Buffer[addr1+7] = (uchar)(WfmInfo.Normal.TotalVolume>>24);    
  
  SPI_Flash_Erase_Sector(MonthDayCumData__ADDR);
  SPI_Flash_Write_Buffer(MonthDayCumData__ADDR,(unsigned char *)g_DataSave_Buffer,Flash_Save_Buffer_Lenth);
  _EINT();//写flash完成，重新开启中断
  SPI_Flash_PowerOff();    
}

//read Day Cum
uchar readDayCum(uchar addr)
{
  uchar ret=1;
  uint addr1;

  SPI_Flash_PowerOn();
  _DINT();//写flash前，先关闭中断
  SPI_Flash_Read_Buffer(MonthDayCumData__ADDR,(unsigned char *)g_DataSave_Buffer,Flash_Save_Buffer_Lenth);
  
  switch(addr)
  {
  case 0x01:
    addr1 = DayDataSaveAddress+DayDataSize*0;//存1日的冻结
    break;
  case 0x02:
    addr1 = DayDataSaveAddress+DayDataSize*1;//存2日的冻结
    break;   
  case 0x03:
    addr1 = DayDataSaveAddress+DayDataSize*2;//存3日的冻结
    break;
  case 0x04:
    addr1 = DayDataSaveAddress+DayDataSize*3;//存4日的冻结
    break;
  case 0x05:
    addr1 = DayDataSaveAddress+DayDataSize*4;//存5日的冻结
    break;
  case 0x06:
    addr1 = DayDataSaveAddress+DayDataSize*5;//存6日的冻结
    break; 
  case 0x07:
    addr1 = DayDataSaveAddress+DayDataSize*6;//存7日的冻结
    break;
  case 0x08:
    addr1 = DayDataSaveAddress+DayDataSize*7;//存8日的冻结
    break; 
  case 0x09:
    addr1 = DayDataSaveAddress+DayDataSize*8;//存9日的冻结
    break;
  case 0x10:
    addr1 = DayDataSaveAddress+DayDataSize*9;//存10日的冻结
    break; 
  case 0x11:
    addr1 = DayDataSaveAddress+DayDataSize*10;//存11日的冻结
    break;
  case 0x12:
    addr1 = DayDataSaveAddress+DayDataSize*11;//存12日的冻结
    break; 
  case 0x13:
    addr1 = DayDataSaveAddress+DayDataSize*12;//存13日的冻结
    break; 
  case 0x14:
    addr1 = DayDataSaveAddress+DayDataSize*13;//存14日的冻结
    break; 
  case 0x15:
    addr1 = DayDataSaveAddress+DayDataSize*14;//存15日的冻结
    break;
  case 0x16:
    addr1 = DayDataSaveAddress+DayDataSize*15;//存16日的冻结
    break; 
  case 0x17:
    addr1 = DayDataSaveAddress+DayDataSize*16;//存17日的冻结
    break;
  case 0x18:
    addr1 = DayDataSaveAddress+DayDataSize*17;//存18日的冻结
    break; 
  case 0x19:
    addr1 = DayDataSaveAddress+DayDataSize*18;//存19日的冻结
    break;
  case 0x20:
    addr1 = DayDataSaveAddress+DayDataSize*19;//存20日的冻结
    break; 
  case 0x21:
    addr1 = DayDataSaveAddress+DayDataSize*20;//存21日的冻结
    break; 
   case 0x22:
    addr1 = DayDataSaveAddress+DayDataSize*21;//存22日的冻结
    break;
   case 0x23:
    addr1 = DayDataSaveAddress+DayDataSize*22;//存23日的冻结
    break; 
  case 0x24:
    addr1 = DayDataSaveAddress+DayDataSize*23;//存24日的冻结
    break; 
  case 0x25:
    addr1 = DayDataSaveAddress+DayDataSize*24;//存25日的冻结
    break; 
  case 0x26:
    addr1 = DayDataSaveAddress+DayDataSize*25;//存26日的冻结
    break;
  case 0x27:
    addr1 = DayDataSaveAddress+DayDataSize*26;//存27日的冻结
    break;      
  case 0x28:
    addr1 = DayDataSaveAddress+DayDataSize*27;//存28日的冻结
    break; 
  case 0x29:
    addr1 = DayDataSaveAddress+DayDataSize*28;//存29日的冻结
    break;  
  case 0x30:
    addr1 = DayDataSaveAddress+DayDataSize*29;//存30日的冻结
    break; 
  case 0x31:
    addr1 = DayDataSaveAddress+DayDataSize*30;//存31日的冻结
    break;      
  default:
    ret = 0;
    break;    
  }

  WfmInfo.Normal.TotalVolumeDecimalSave <<= 8;
  WfmInfo.Normal.TotalVolumeDecimalSave |= g_DataSave_Buffer[addr1+3];
  WfmInfo.Normal.TotalVolumeDecimalSave <<= 8;
  WfmInfo.Normal.TotalVolumeDecimalSave |= g_DataSave_Buffer[addr1+2];
  WfmInfo.Normal.TotalVolumeDecimalSave <<= 8;
  WfmInfo.Normal.TotalVolumeDecimalSave |= g_DataSave_Buffer[addr1+1];
  WfmInfo.Normal.TotalVolumeDecimalSave <<= 8;
  WfmInfo.Normal.TotalVolumeDecimalSave |= g_DataSave_Buffer[addr1+0];
  
  WfmInfo.Normal.TotalVolumeSave <<= 8;
  WfmInfo.Normal.TotalVolumeSave |= g_DataSave_Buffer[addr1+7];
  WfmInfo.Normal.TotalVolumeSave <<= 8;
  WfmInfo.Normal.TotalVolumeSave |= g_DataSave_Buffer[addr1+6];
  WfmInfo.Normal.TotalVolumeSave <<= 8;
  WfmInfo.Normal.TotalVolumeSave |= g_DataSave_Buffer[addr1+5];
  WfmInfo.Normal.TotalVolumeSave <<= 8;
  WfmInfo.Normal.TotalVolumeSave |= g_DataSave_Buffer[addr1+4];    
  
  _EINT();//写flash完成，重新开启中断
  SPI_Flash_PowerOff();  
  
  return ret;
}



//读系统参数
void readSystemParam( void )
{
  /*
  g_systemPara.szTermID[0] = 1;
  g_systemPara.szTermID[1] = 0;
  g_systemPara.szTermID[2] = 0;
  g_systemPara.szTermID[3] = 0;
  g_systemPara.szTermID[4] = 0;
  g_systemPara.szTermID[5] = 0;
  g_systemPara.szTermID[6] = 0;  
  Save_WaterMeterAddress();
  
  WfmInfo.Normal.TotalVolumeDecimal = 0;
  WfmInfo.Normal.TotalVolume = 0; 
  Save_WaterCumData();
  */ 
  
  //Read_WaterMeterAddress();
  //Read_WaterCumData(); 
}
