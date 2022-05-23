#include "params.h"
#include "functions.h"

/***************************************************************************
 * startWith function.
 *
 * @brief	Judge the first char.
 **************************************************************************/
char startWith(unsigned char * p1,unsigned char * p2)
{
  while(*p2)
  {
    if(!*p1)
      return 0;
    if(*p1++ != *p2++)
      return 0;
  }
  return 1;
}


/***************************************************************************
 * dectoBCD function.
 *
 * @brief	Convert DEC to BCD of the char.
 **************************************************************************/
unsigned char dectoBCD(unsigned char dec)
{
   unsigned char bcd = 0;
   bcd = ((dec/10)<<4) | (dec%10);
   return bcd;
}


/***************************************************************************
 * BCDtoDec function.
 *
 * @brief	Convert BCD to DEC of the char.
 **************************************************************************/
unsigned char BCDtoDec(unsigned char bcd)
{
   unsigned char dec = 0;
   dec = ((bcd >> 4) & 0x0F)*10 + (bcd & 0x0F);
   return dec;
}


/***************************************************************************
 * DotHextoDotDec function.
 *
 * @brief	Convert Dot HEX to Dot DEC of the char.
 **************************************************************************/
double dotHextoDotDec(unsigned long dotHex)
{
   double dotDec = 0;
   dotDec = (double)(((dotHex >> 28) & 0x0000000F)*4096 + ((dotHex >> 24) & 0x0000000F)*256 + ((dotHex >> 20) & 0x0000000F)*16 + ((dotHex >> 16) & 0x0000000F)*1);
   dotDec = dotDec + ((double)((dotHex >> 12) & 0x0000000F))/16 + ((double)((dotHex >> 8) & 0x0000000F))/256 + ((double)((dotHex >> 4) & 0x0000000F))/4096 + ((double)((dotHex >> 0) & 0x0000000F))/65536;
   return dotDec;
}


/***************************************************************************
 * sumCheck function.
 *
 * @brief	Used to IR communication of sum check.
 **************************************************************************/
unsigned char sumCheck(unsigned char *p1, unsigned char len)
{
  unsigned char i, temp = 0;
  for(i = 0; i < len; i++)
  {
    temp += p1[i];
  }
  return temp;
}

//�ֽڸߵ�λ��������
uchar reverseHighLowBit(uchar  dat)
{    
  uchar  val;
  
  val = ((dat&0x01)<<7)|((dat&0x02)<<5)|((dat&0x04)<<3)|((dat&0x08)<<1)|((dat&0x10)>>1)|((dat&0x20)>>3)|((dat&0x40)>>5)|((dat&0x80)>>7);    
  return  val;
}


//�ֽڸߵ�λ��������
unsigned char CharChange(unsigned char  dat)//�ֽڸߵ�λ����
{    
  unsigned char  val;
  val = ((dat&0x01)<<7)|((dat&0x02)<<5)|((dat&0x04)<<3)|((dat&0x08)<<1)|((dat&0x10)>>1)|((dat&0x20)>>3)|((dat&0x40)>>5)|((dat&0x80)>>7);    
  return  val;
}



//ת��������յ��Ľ���ˮ��/ˮ������
unsigned int ChangeLadderWaterData(unsigned char data1, unsigned char data2, unsigned char data3)
{    
  unsigned int temp = 0;
  data1 = BCDtoDec(data1);
  data2 = BCDtoDec(data2);
  data3 = BCDtoDec(data3);
  temp = (unsigned int)(data3*10000 + data2*100 + data1);
  return  temp;
}

//ת������ˮ��/ˮ������Ϊ������ո�ʽ
unsigned char Reverse_ChangeLadderWaterData(unsigned int data1, unsigned char num)
{    
  unsigned char temp = 0;
  if(num == 1)
  {
    temp = data1 % 100;
  }
  else if(num == 2)
  {
    temp = data1 % 10000;
    temp = data1 / 100;
  }
  else if(num == 3)
  {
    temp = data1 / 10000;
  }
    
  temp = dectoBCD(temp);
  return  temp;
}
