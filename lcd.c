#include "msp430x54x.h"
#include "functions.h"
#include "params.h"
#include "lcd.h"

const unsigned char digit[12] = {
0xD7,                    // displays "0"
0x06,                    // displays "1"
0xE3,                    // displays "2"
0xA7,                    // displays "3"
0x36,                    // displays "4"
0xB5,                    // displays "5"
0xF5,                    // displays "6"
0x07,                    // displays "7"
0xF7,                    // displays "8"
0xB7,                    // displays "9"
0x00,                    // displays " "
0x20                     // displays "-"
};

unsigned char seg[24] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

/*******************************************
*void ht1621_write_com(unsigned char com)
*
*写命令到 ht1621
*******************************************/
void ht1621_write_com(unsigned char com)
{
  unsigned char i;
  HT1621_CS0;
  HT1621_WR0;
  HT1621_DATA1;//写1
  delay_us(1);
  HT1621_WR1;
  delay_us(1);
  HT1621_WR0;
  HT1621_DATA0;//写0
  delay_us(1);
  HT1621_WR1;
  delay_us(1);
  HT1621_WR0;
  HT1621_DATA0;//写0
  delay_us(1);
  HT1621_WR1;
  delay_us(1);
  
  for(i=0;i<8;i++)
  {
      HT1621_WR0;
      if(com & 0x80)
        HT1621_DATA1;//写1
      else
        HT1621_DATA0;//写0
      com <<= 1;
      delay_us(1);
      HT1621_WR1;
      delay_us(1);
  }

  HT1621_WR0;
  HT1621_DATA1;//写1
  delay_us(1);
  HT1621_WR1;
  delay_us(1); 
  HT1621_CS1;
}

/*******************************************
*void ht1621_write_data(unsigned char addr,unsigned char data)
*
*写数据到 ht1621
*******************************************/
void ht1621_write_data(unsigned char addr,unsigned char DATA)
{
  unsigned char i;
  HT1621_CS0;
  HT1621_WR0;
  HT1621_DATA1;//写1
  delay_us(1);
  HT1621_WR1;
  delay_us(1);
  
  HT1621_WR0;
  HT1621_DATA0;//写0
  delay_us(1);
  HT1621_WR1;
  delay_us(1);
  
  HT1621_WR0;
  HT1621_DATA1;//写1
  delay_us(1);
  HT1621_WR1;
  delay_us(1);
  
  addr <<= 2;
  for(i=0;i<6;i++) /*Write RAM address: xxA5~A0*/
  {
      HT1621_WR0;
      if(addr & 0x80)
        HT1621_DATA1;//写1
      else
        HT1621_DATA0;//写0
      addr <<= 1;
      delay_us(1);
      HT1621_WR1;
      delay_us(1);
  }
  
  for(i=0;i<4;i++) /*Write Displayplay data :xxxxC0~C3*/
  {
      HT1621_WR0;
      if(DATA & 0x01)
        HT1621_DATA1;//写1
      else
        HT1621_DATA0;//写0
      DATA >>= 1;
      delay_us(1);
      HT1621_WR1;
      delay_us(1);
  } 
  HT1621_CS1;
}

/*******************************************
*void ht1621_clearLCD(void)
*
*清屏
*******************************************/
void ClearLCD(void)
{
  unsigned char i;
  for(i=0;i<32;i++)
  {
    ht1621_write_data(i,0x00);
    seg[i] = 0;
  }
}

void AllLCDDisplay(void)
{
  unsigned char i;
  for(i=0;i<32;i++)
    ht1621_write_data(i,0x0f);
}

//LCD初始化
void InitLCD(void)
{
  P4DIR |= BIT4;                //设为输出
  P4DIR |= BIT3;                //设为输出
  P4DIR |= BIT5;                //设为输出

  HT1621_CS1;
  HT1621_WR1; 
  HT1621_DATA1;//CS高电平脉冲初始化HT1621的串行接口
  ht1621_write_com(HT1621_RC_256K);//系统时钟为片内RC振荡器
  ht1621_write_com(HT1621_BIAS_3_4);////LCD1/3偏压，4个公共口
  ht1621_write_com(HT1621_SYS_ENABLE);//打开系统振荡器
  ht1621_write_com(HT1621_LCD_ON);//打开LCD偏压发生器
  ClearLCD();
  for(unsigned int i = 0; i < 4000; i++);
  
  AllLCDDisplay();
  dely_s(2);
}


void Icon_jiantou1_On_ht1621b(void)
{
  seg[0] = seg[0]|0x08;
  ht1621_write_data(0x17,seg[0]);
}
void Icon_jiantou1_Off_ht1621b(void)
{
  seg[0] = seg[0]&(~0x08);
  ht1621_write_data(0x17,seg[0]);
}
void Icon_jiantou2_On_ht1621b(void)
{
  seg[2] = seg[2]|0x08;
  ht1621_write_data(0x15,seg[2]);
}
void Icon_jiantou2_Off_ht1621b(void)
{
  seg[2] = seg[2]&(~0x08);
  ht1621_write_data(0x15,seg[2]);
}
void Icon_jiantou3_On_ht1621b(void)
{
  seg[4] = seg[4]|0x08;
  ht1621_write_data(0x13,seg[4]);
}
void Icon_jiantou3_Off_ht1621b(void)
{
  seg[4] = seg[4]&(~0x08);
  ht1621_write_data(0x13,seg[4]);
}

void Icon_xiaoshudian1_On_ht1621b(void)
{
  seg[6] = seg[6]|0x08;
  ht1621_write_data(0x11,seg[6]);
}
void Icon_xiaoshudian1_Off_ht1621b(void)
{
  seg[6] = seg[6]&(~0x08);
  ht1621_write_data(0x11,seg[6]);
}
void Icon_xiaoshudian2_On_ht1621b(void)
{
  seg[8] = seg[8]|0x08;
  ht1621_write_data(0x0f,seg[8]);
}
void Icon_xiaoshudian2_Off_ht1621b(void)
{
  seg[8] = seg[8]&(~0x08);
  ht1621_write_data(0x0f,seg[8]);
}
void Icon_xiaoshudian3_On_ht1621b(void)
{
  seg[10] = seg[10]|0x08;
  ht1621_write_data(0x0d,seg[10]);
}
void Icon_xiaoshudian3_Off_ht1621b(void)
{
  seg[10] = seg[10]&(~0x08);
  ht1621_write_data(0x0d,seg[10]);
}
void Icon_xiaoshudian4_On_ht1621b(void)
{
  seg[12] = seg[12]|0x08;
  ht1621_write_data(0x0b,seg[12]);
}
void Icon_xiaoshudian4_Off_ht1621b(void)
{
  seg[12] = seg[12]&(~0x08);
  ht1621_write_data(0x0b,seg[12]);
}
void Icon_xiaoshudian5_On_ht1621b(void)
{
  seg[16] = seg[16]|0x08;
  ht1621_write_data(0x07,seg[16]);
}
void Icon_xiaoshudian5_Off_ht1621b(void)
{
  seg[16] = seg[16]&(~0x08);
  ht1621_write_data(0x07,seg[16]);
}
void Icon_xiaoshudian6_On_ht1621b(void)
{
  seg[18] = seg[18]|0x08;
  ht1621_write_data(0x05,seg[18]);
}
void Icon_xiaoshudian6_Off_ht1621b(void)
{
  seg[18] = seg[18]&(~0x08);
  ht1621_write_data(0x05,seg[18]);
}
void Icon_xiaoshudian7_On_ht1621b(void)
{
  seg[20] = seg[20]|0x08;
  ht1621_write_data(0x03,seg[20]);
}
void Icon_xiaoshudian7_Off_ht1621b(void)
{
  seg[20] = seg[20]&(~0x08);
  ht1621_write_data(0x03,seg[20]);
}

void Icon_danwei_On_ht1621b(void)
{
  seg[14] = seg[14]|0x08;
  ht1621_write_data(0x09,seg[14]);
}
void Icon_danwei_Off_ht1621b(void)
{
  seg[14] = seg[14]&(~0x08);
  ht1621_write_data(0x09,seg[14]);
}


//空管报警图标显示（第一个向下箭头，表示空管报警）
void Display_Alarm_Icon(unsigned char flag)
{
  if(flag)
    Icon_jiantou1_On;
  else
    Icon_jiantou1_Off;
}

//跑水图标显示 (第三个向下箭头，表示水表正在走水）
void Display_WaterRunning_Icon(unsigned char flag)
{
  if(flag)
    Icon_jiantou3_On;
  else
    Icon_jiantou3_Off;
}


//累积量和瞬时流速单位图标显示 
void Display_unit_Icon(unsigned char flag)
{
  if(flag)
    Icon_danwei_On;
  else
    Icon_danwei_Off;
}


//总累积量和瞬时流速显示
void Display_Cum_Icon(unsigned long cum, unsigned long speed)
{
  ClearLCD();
  
  Display_unit_Icon(1);
  Icon_xiaoshudian1_On; //累积量小数点，单位0.1L
  Icon_xiaoshudian5_On; //瞬时流速小数点，单位1/L
  speed = speed * 1000 / 60;
  unsigned char ge1,shi1,bai1,qian1;
  speed %= 9999; 
  qian1 = speed/1000;
  bai1 = (speed-1000*qian1)/100;
  shi1 = (speed-1000*qian1-100*bai1)/10;
  ge1 = speed%10;
  
  unsigned char ge2,shi2,bai2,qian2,wan2,shiwan2,baiwan2,qianwan2;
  qianwan2 = cum/10000000;
  baiwan2 = (cum-10000000*qianwan2)/1000000;
  shiwan2 = (cum-10000000*qianwan2-1000000*baiwan2)/100000;
  wan2 = (cum-10000000*qianwan2-1000000*baiwan2-100000*shiwan2)/10000;
  qian2 = (cum-10000000*qianwan2-1000000*baiwan2-100000*shiwan2-10000*wan2)/1000;
  bai2 = (cum-10000000*qianwan2-1000000*baiwan2-100000*shiwan2-10000*wan2-1000*qian2)/100;
  shi2 = (cum-10000000*qianwan2-1000000*baiwan2-100000*shiwan2-10000*wan2-1000*qian2-100*bai2)/10;
  ge2 = cum%10;
  
  //瞬时流量显示
  seg[16] = (seg[16] & 0x08) | (digit[qian1] & 0x0f);
  ht1621_write_data(0x07,seg[16]);
  seg[17] = (digit[qian1]>>4) & 0x0f;
  ht1621_write_data(0x06,seg[17]);
  seg[18] = (seg[18] & 0x08) | (digit[bai1] & 0x0f);
  ht1621_write_data(0x05,seg[18]);
  seg[19] = (digit[bai1]>>4) & 0x0f;
  ht1621_write_data(0x04,seg[19]);  
  seg[20] = (seg[20] & 0x08) | (digit[shi1] & 0x0f);
  ht1621_write_data(0x03,seg[20]);
  seg[21] = (digit[shi1]>>4) & 0x0f;
  ht1621_write_data(0x02,seg[21]);
  seg[22] = (seg[22] & 0x08) | (digit[ge1] & 0x0f);
  ht1621_write_data(0x01,seg[22]);
  seg[23] = (digit[ge1]>>4) & 0x0f;
  ht1621_write_data(0x00,seg[23]);  
  
  
  //累积量显示
  if(qianwan2)
  {
    seg[0] = (seg[0] & 0x08) | (digit[qianwan2] & 0x0f);
    ht1621_write_data(0x17,seg[0]);
    seg[1] = (digit[qianwan2]>>4) & 0x0f;
    ht1621_write_data(0x16,seg[1]);      
    seg[2] = (seg[2] & 0x08) | (digit[baiwan2] & 0x0f);
    ht1621_write_data(0x15,seg[2]);
    seg[3] = (digit[baiwan2]>>4) & 0x0f;
    ht1621_write_data(0x14,seg[3]);     
    seg[4] = (seg[4] & 0x08) | (digit[shiwan2] & 0x0f);
    ht1621_write_data(0x13,seg[4]);
    seg[5] = (digit[shiwan2]>>4) & 0x0f;
    ht1621_write_data(0x12,seg[5]);      
  }
  else if(baiwan2)
  {
    seg[2] = (seg[2] & 0x08) | (digit[baiwan2] & 0x0f);
    ht1621_write_data(0x15,seg[2]);
    seg[3] = (digit[baiwan2]>>4) & 0x0f;
    ht1621_write_data(0x14,seg[3]);     
    seg[4] = (seg[4] & 0x08) | (digit[shiwan2] & 0x0f);
    ht1621_write_data(0x13,seg[4]);
    seg[5] = (digit[shiwan2]>>4) & 0x0f;
    ht1621_write_data(0x12,seg[5]);     
  }
  else if(shiwan2)
  {
    seg[4] = (seg[4] & 0x08) | (digit[shiwan2] & 0x0f);
    ht1621_write_data(0x13,seg[4]);
    seg[5] = (digit[shiwan2]>>4) & 0x0f;
    ht1621_write_data(0x12,seg[5]);     
  }
  
  seg[6] = (seg[6] & 0x08) | (digit[wan2] & 0x0f);
  ht1621_write_data(0x11,seg[6]);
  seg[7] = (digit[wan2]>>4) & 0x0f;
  ht1621_write_data(0x10,seg[7]);    
  seg[8] = (seg[8] & 0x08) | (digit[qian2] & 0x0f);
  ht1621_write_data(0x0f,seg[8]);
  seg[9] = (digit[qian2]>>4) & 0x0f;
  ht1621_write_data(0x0e,seg[9]);   
  seg[10] = (seg[10] & 0x08) | (digit[bai2] & 0x0f);
  ht1621_write_data(0x0d,seg[10]);
  seg[11] = (digit[bai2]>>4) & 0x0f;
  ht1621_write_data(0x0c,seg[11]);   
  seg[12] = (seg[12] & 0x08) | (digit[shi2] & 0x0f);
  ht1621_write_data(0x0b,seg[12]);
  seg[13] = (digit[shi2]>>4) & 0x0f;
  ht1621_write_data(0x0a,seg[13]);  
  seg[14] = (seg[14] & 0x08) | (digit[ge2] & 0x0f);
  ht1621_write_data(0x09,seg[14]);
  seg[15] = (digit[ge2]>>4) & 0x0f;
  ht1621_write_data(0x08,seg[15]);  
}


//日期显示
void Display_Date_Icon(unsigned char year, unsigned char month, unsigned char day, unsigned char hour, unsigned char minute)
{
  ClearLCD();
  //LCDM11 = ((LCDM11 & 0x08) | digit[11]);
  //LCDM14 = ((LCDM14 & 0x08) | digit[11]);
  seg[4] = (seg[4] & 0x08) | (digit[11] & 0x0f);
  ht1621_write_data(0x13,seg[4]);
  seg[5] = (digit[11]>>4) & 0x0f;
  ht1621_write_data(0x12,seg[5]);
  seg[10] = (seg[10] & 0x08) | (digit[11] & 0x0f);
  ht1621_write_data(0x0d,seg[10]);
  seg[11] = (digit[11]>>4) & 0x0f;
  ht1621_write_data(0x0c,seg[11]);     
  Icon_xiaoshudian6_On;
  
  unsigned char ge,shi;
  year = BCDtoDec(year);
  ge = year%10;
  shi = year/10;
  //LCDM9 = ((LCDM9 & 0x08) | digit[shi]);
  //LCDM10 = ((LCDM10 & 0x08) | digit[ge]); 
  seg[0] = (seg[0] & 0x08) | (digit[shi] & 0x0f);
  ht1621_write_data(0x17,seg[0]);
  seg[1] = (digit[shi]>>4) & 0x0f;
  ht1621_write_data(0x16,seg[1]);      
  seg[2] = (seg[2] & 0x08) | (digit[ge] & 0x0f);
  ht1621_write_data(0x15,seg[2]);
  seg[3] = (digit[ge]>>4) & 0x0f;
  ht1621_write_data(0x14,seg[3]);   
 
  month = BCDtoDec(month);
  ge = month%10;
  shi = month/10;
  //LCDM12 = ((LCDM12 & 0x08) | digit[shi]);
  //LCDM13 = ((LCDM13 & 0x08) | digit[ge]);
  seg[6] = (seg[6] & 0x08) | (digit[shi] & 0x0f);
  ht1621_write_data(0x11,seg[6]);
  seg[7] = (digit[shi]>>4) & 0x0f;
  ht1621_write_data(0x10,seg[7]);    
  seg[8] = (seg[8] & 0x08) | (digit[ge] & 0x0f);
  ht1621_write_data(0x0f,seg[8]);
  seg[9] = (digit[ge]>>4) & 0x0f;
  ht1621_write_data(0x0e,seg[9]);     

  day = BCDtoDec(day);
  ge = day%10;
  shi = day/10;
  //LCDM15 = ((LCDM15 & 0x08) | digit[shi]);
  //LCDM16 = ((LCDM16 & 0x08) | digit[ge]); 
  seg[12] = (seg[12] & 0x08) | (digit[shi] & 0x0f);
  ht1621_write_data(0x0b,seg[12]);
  seg[13] = (digit[shi]>>4) & 0x0f;
  ht1621_write_data(0x0a,seg[13]);  
  seg[14] = (seg[14] & 0x08) | (digit[ge] & 0x0f);
  ht1621_write_data(0x09,seg[14]);
  seg[15] = (digit[ge]>>4) & 0x0f;
  ht1621_write_data(0x08,seg[15]);   
 
  hour = BCDtoDec(hour);
  ge = hour%10;
  shi = hour/10;
  //LCDM17 = ((LCDM17 & 0x08) | digit[shi]);
  //LCDM18 = ((LCDM18 & 0x08) | digit[ge]);   
  seg[16] = (seg[16] & 0x08) | (digit[shi] & 0x0f);
  ht1621_write_data(0x07,seg[16]);
  seg[17] = (digit[shi]>>4) & 0x0f;
  ht1621_write_data(0x06,seg[17]);
  seg[18] = (seg[18] & 0x08) | (digit[ge] & 0x0f);
  ht1621_write_data(0x05,seg[18]);
  seg[19] = (digit[ge]>>4) & 0x0f;
  ht1621_write_data(0x04,seg[19]);     
 
  minute = BCDtoDec(minute);
  ge = minute%10;
  shi = minute/10;
  //LCDM19 = ((LCDM19 & 0x08) | digit[shi]);
  //LCDM20 = ((LCDM20 & 0x08) | digit[ge]); 
  seg[20] = (seg[20] & 0x08) | (digit[shi] & 0x0f);
  ht1621_write_data(0x03,seg[20]);
  seg[21] = (digit[shi]>>4) & 0x0f;
  ht1621_write_data(0x02,seg[21]);
  seg[22] = (seg[22] & 0x08) | (digit[ge] & 0x0f);
  ht1621_write_data(0x01,seg[22]);
  seg[23] = (digit[ge]>>4) & 0x0f;
  ht1621_write_data(0x00,seg[23]);   
}

//温度显示
void Display_Temp_Icon( unsigned long t1 )
{
  ClearLCD();
  seg[22] = (seg[22] & 0x08) | (digit[1] & 0x0f);
  ht1621_write_data(0x01,seg[22]);
  seg[23] = (digit[1]>>4) & 0x0f;
  ht1621_write_data(0x00,seg[23]);   
  Icon_xiaoshudian4_On; //温度小数点，单位0.1度
  
  unsigned char ge,shi,bai;
  t1 %= 999;
  bai = t1/100;
  shi = (t1-100*bai)/10;
  ge = t1%10;
  
  //温度显示
  if(bai)  
  {
    seg[10] = (seg[10] & 0x08) | (digit[bai] & 0x0f);
    ht1621_write_data(0x0d,seg[10]);
    seg[11] = (digit[bai]>>4) & 0x0f;
    ht1621_write_data(0x0c,seg[11]);     
  }
  
  seg[12] = (seg[12] & 0x08) | (digit[shi] & 0x0f);
  ht1621_write_data(0x0b,seg[12]);
  seg[13] = (digit[shi]>>4) & 0x0f;
  ht1621_write_data(0x0a,seg[13]);  
  seg[14] = (seg[14] & 0x08) | (digit[ge] & 0x0f);
  ht1621_write_data(0x09,seg[14]);
  seg[15] = (digit[ge]>>4) & 0x0f;
  ht1621_write_data(0x08,seg[15]);    
  
}


//飞行时间显示
void Display_TimeOfFlight_Icon( unsigned char flag, unsigned long time )
{
  ClearLCD();
  if(flag==1)//飞行时间差
  {
    seg[22] = (seg[22] & 0x08) | (digit[2] & 0x0f);
    ht1621_write_data(0x01,seg[22]);
    seg[23] = (digit[2]>>4) & 0x0f;
    ht1621_write_data(0x00,seg[23]);     
  }
  else if(flag==2)//上游飞行时间
  {
    seg[22] = (seg[22] & 0x08) | (digit[3] & 0x0f);
    ht1621_write_data(0x01,seg[22]);
    seg[23] = (digit[3]>>4) & 0x0f;
    ht1621_write_data(0x00,seg[23]);     
  }  
  else if(flag==3)//下游飞行时间 
  {
    seg[22] = (seg[22] & 0x08) | (digit[4] & 0x0f);
    ht1621_write_data(0x01,seg[22]);
    seg[23] = (digit[4]>>4) & 0x0f;
    ht1621_write_data(0x00,seg[23]);     
  } 
  
  unsigned char ge2,shi2,bai2,qian2,wan2,shiwan2,baiwan2,qianwan2;
  qianwan2 = time/10000000;
  baiwan2 = time%10000000/1000000;
  shiwan2 = time%1000000/100000;
  wan2 = time%100000/10000;
  qian2 = time%10000/1000;
  bai2 = time%1000/100;
  shi2 = time%100/10;;
  ge2 = time%10;
  
  //飞行时间显示，单位ps
  if(qianwan2)
  {
    seg[0] = (seg[0] & 0x08) | (digit[qianwan2] & 0x0f);
    ht1621_write_data(0x17,seg[0]);
    seg[1] = (digit[qianwan2]>>4) & 0x0f;
    ht1621_write_data(0x16,seg[1]);      
    seg[2] = (seg[2] & 0x08) | (digit[baiwan2] & 0x0f);
    ht1621_write_data(0x15,seg[2]);
    seg[3] = (digit[baiwan2]>>4) & 0x0f;
    ht1621_write_data(0x14,seg[3]);  
    seg[4] = (seg[4] & 0x08) | (digit[shiwan2] & 0x0f);
    ht1621_write_data(0x13,seg[4]);
    seg[5] = (digit[shiwan2]>>4) & 0x0f;
    ht1621_write_data(0x12,seg[5]); 
    seg[6] = (seg[6] & 0x08) | (digit[wan2] & 0x0f);
    ht1621_write_data(0x11,seg[6]);
    seg[7] = (digit[wan2]>>4) & 0x0f;
    ht1621_write_data(0x10,seg[7]);    
    seg[8] = (seg[8] & 0x08) | (digit[qian2] & 0x0f);
    ht1621_write_data(0x0f,seg[8]);
    seg[9] = (digit[qian2]>>4) & 0x0f;
    ht1621_write_data(0x0e,seg[9]);   
    seg[10] = (seg[10] & 0x08) | (digit[bai2] & 0x0f);
    ht1621_write_data(0x0d,seg[10]);
    seg[11] = (digit[bai2]>>4) & 0x0f;
    ht1621_write_data(0x0c,seg[11]);    
    seg[12] = (seg[12] & 0x08) | (digit[shi2] & 0x0f);
    ht1621_write_data(0x0b,seg[12]);
    seg[13] = (digit[shi2]>>4) & 0x0f;
    ht1621_write_data(0x0a,seg[13]);     
  }
  else if(baiwan2)
  { 
    seg[2] = (seg[2] & 0x08) | (digit[baiwan2] & 0x0f);
    ht1621_write_data(0x15,seg[2]);
    seg[3] = (digit[baiwan2]>>4) & 0x0f;
    ht1621_write_data(0x14,seg[3]);  
    seg[4] = (seg[4] & 0x08) | (digit[shiwan2] & 0x0f);
    ht1621_write_data(0x13,seg[4]);
    seg[5] = (digit[shiwan2]>>4) & 0x0f;
    ht1621_write_data(0x12,seg[5]); 
    seg[6] = (seg[6] & 0x08) | (digit[wan2] & 0x0f);
    ht1621_write_data(0x11,seg[6]);
    seg[7] = (digit[wan2]>>4) & 0x0f;
    ht1621_write_data(0x10,seg[7]);    
    seg[8] = (seg[8] & 0x08) | (digit[qian2] & 0x0f);
    ht1621_write_data(0x0f,seg[8]);
    seg[9] = (digit[qian2]>>4) & 0x0f;
    ht1621_write_data(0x0e,seg[9]);   
    seg[10] = (seg[10] & 0x08) | (digit[bai2] & 0x0f);
    ht1621_write_data(0x0d,seg[10]);
    seg[11] = (digit[bai2]>>4) & 0x0f;
    ht1621_write_data(0x0c,seg[11]);    
    seg[12] = (seg[12] & 0x08) | (digit[shi2] & 0x0f);
    ht1621_write_data(0x0b,seg[12]);
    seg[13] = (digit[shi2]>>4) & 0x0f;
    ht1621_write_data(0x0a,seg[13]);     
  }
  else if(shiwan2)
  {
    seg[4] = (seg[4] & 0x08) | (digit[shiwan2] & 0x0f);
    ht1621_write_data(0x13,seg[4]);
    seg[5] = (digit[shiwan2]>>4) & 0x0f;
    ht1621_write_data(0x12,seg[5]); 
    seg[6] = (seg[6] & 0x08) | (digit[wan2] & 0x0f);
    ht1621_write_data(0x11,seg[6]);
    seg[7] = (digit[wan2]>>4) & 0x0f;
    ht1621_write_data(0x10,seg[7]);    
    seg[8] = (seg[8] & 0x08) | (digit[qian2] & 0x0f);
    ht1621_write_data(0x0f,seg[8]);
    seg[9] = (digit[qian2]>>4) & 0x0f;
    ht1621_write_data(0x0e,seg[9]);   
    seg[10] = (seg[10] & 0x08) | (digit[bai2] & 0x0f);
    ht1621_write_data(0x0d,seg[10]);
    seg[11] = (digit[bai2]>>4) & 0x0f;
    ht1621_write_data(0x0c,seg[11]);    
    seg[12] = (seg[12] & 0x08) | (digit[shi2] & 0x0f);
    ht1621_write_data(0x0b,seg[12]);
    seg[13] = (digit[shi2]>>4) & 0x0f;
    ht1621_write_data(0x0a,seg[13]);     
  }
  else if(wan2)
  {
    seg[6] = (seg[6] & 0x08) | (digit[wan2] & 0x0f);
    ht1621_write_data(0x11,seg[6]);
    seg[7] = (digit[wan2]>>4) & 0x0f;
    ht1621_write_data(0x10,seg[7]);    
    seg[8] = (seg[8] & 0x08) | (digit[qian2] & 0x0f);
    ht1621_write_data(0x0f,seg[8]);
    seg[9] = (digit[qian2]>>4) & 0x0f;
    ht1621_write_data(0x0e,seg[9]);   
    seg[10] = (seg[10] & 0x08) | (digit[bai2] & 0x0f);
    ht1621_write_data(0x0d,seg[10]);
    seg[11] = (digit[bai2]>>4) & 0x0f;
    ht1621_write_data(0x0c,seg[11]);    
    seg[12] = (seg[12] & 0x08) | (digit[shi2] & 0x0f);
    ht1621_write_data(0x0b,seg[12]);
    seg[13] = (digit[shi2]>>4) & 0x0f;
    ht1621_write_data(0x0a,seg[13]);    
  }  
  else if(qian2)
  {
    seg[8] = (seg[8] & 0x08) | (digit[qian2] & 0x0f);
    ht1621_write_data(0x0f,seg[8]);
    seg[9] = (digit[qian2]>>4) & 0x0f;
    ht1621_write_data(0x0e,seg[9]);   
    seg[10] = (seg[10] & 0x08) | (digit[bai2] & 0x0f);
    ht1621_write_data(0x0d,seg[10]);
    seg[11] = (digit[bai2]>>4) & 0x0f;
    ht1621_write_data(0x0c,seg[11]);    
    seg[12] = (seg[12] & 0x08) | (digit[shi2] & 0x0f);
    ht1621_write_data(0x0b,seg[12]);
    seg[13] = (digit[shi2]>>4) & 0x0f;
    ht1621_write_data(0x0a,seg[13]);     
  } 
  else if(bai2)
  {
    seg[10] = (seg[10] & 0x08) | (digit[bai2] & 0x0f);
    ht1621_write_data(0x0d,seg[10]);
    seg[11] = (digit[bai2]>>4) & 0x0f;
    ht1621_write_data(0x0c,seg[11]);    
    seg[12] = (seg[12] & 0x08) | (digit[shi2] & 0x0f);
    ht1621_write_data(0x0b,seg[12]);
    seg[13] = (digit[shi2]>>4) & 0x0f;
    ht1621_write_data(0x0a,seg[13]);     
  } 
  else if(shi2)
  {
    seg[12] = (seg[12] & 0x08) | (digit[shi2] & 0x0f);
    ht1621_write_data(0x0b,seg[12]);
    seg[13] = (digit[shi2]>>4) & 0x0f;
    ht1621_write_data(0x0a,seg[13]);   
  } 
 
  seg[14] = (seg[14] & 0x08) | (digit[ge2] & 0x0f);
  ht1621_write_data(0x09,seg[14]);
  seg[15] = (digit[ge2]>>4) & 0x0f;
  ht1621_write_data(0x08,seg[15]);  
}

//红外设定状态显示
void Display_IR_Icon(void)
{
  ClearLCD();
  //红外设定状态
  seg[22] = (seg[22] & 0x08) | (digit[5] & 0x0f);
  ht1621_write_data(0x01,seg[22]);
  seg[23] = (digit[5]>>4) & 0x0f;
  ht1621_write_data(0x00,seg[23]);   
  
  
  seg[0] = (seg[0] & 0x08) | (digit[11] & 0x0f);
  ht1621_write_data(0x17,seg[0]);
  seg[1] = (digit[11]>>4) & 0x0f;
  ht1621_write_data(0x16,seg[1]);      
  seg[2] = (seg[2] & 0x08) | (digit[11] & 0x0f);
  ht1621_write_data(0x15,seg[2]);
  seg[3] = (digit[11]>>4) & 0x0f;
  ht1621_write_data(0x14,seg[3]);  
  seg[4] = (seg[4] & 0x08) | (digit[11] & 0x0f);
  ht1621_write_data(0x13,seg[4]);
  seg[5] = (digit[11]>>4) & 0x0f;
  ht1621_write_data(0x12,seg[5]); 
  seg[6] = (seg[6] & 0x08) | (digit[11] & 0x0f);
  ht1621_write_data(0x11,seg[6]);
  seg[7] = (digit[11]>>4) & 0x0f;
  ht1621_write_data(0x10,seg[7]);    
  seg[8] = (seg[8] & 0x08) | (digit[11] & 0x0f);
  ht1621_write_data(0x0f,seg[8]);
  seg[9] = (digit[11]>>4) & 0x0f;
  ht1621_write_data(0x0e,seg[9]);   
  seg[10] = (seg[10] & 0x08) | (digit[11] & 0x0f);
  ht1621_write_data(0x0d,seg[10]);
  seg[11] = (digit[11]>>4) & 0x0f;
  ht1621_write_data(0x0c,seg[11]);    
  seg[12] = (seg[12] & 0x08) | (digit[11] & 0x0f);
  ht1621_write_data(0x0b,seg[12]);
  seg[13] = (digit[11]>>4) & 0x0f;
  ht1621_write_data(0x0a,seg[13]);  
  seg[14] = (seg[14] & 0x08) | (digit[11] & 0x0f);
  ht1621_write_data(0x09,seg[14]);
  seg[15] = (digit[11]>>4) & 0x0f;
  ht1621_write_data(0x08,seg[15]);     
}


//其它信息显示
void Display_Else_Icon(unsigned char flag, unsigned long dat1, unsigned long dat2)
{
  if(flag == DISP_STAT_MAIN)
  {
    Display_Cum_Icon(dat1,dat2);
  }
  else if(flag == DISP_STAT_TIME)
  {
    Display_Date_Icon(g_tagRTC.g_Year,g_tagRTC.g_Month,g_tagRTC.g_Day,g_tagRTC.g_Hour,g_tagRTC.g_Minite);
  }
  else if(flag == DISP_STAT_TEMP)
  {
    Display_Temp_Icon(dat1);
  }
  else if(flag == DISP_STAT_TIMEF)
  {
    Display_TimeOfFlight_Icon(1,dat1);
  }
  else if(flag == DISP_STAT_TIMEU)
  {
    Display_TimeOfFlight_Icon(2,dat1);
  }
  else if(flag == DISP_STAT_TIMED)
  {
    Display_TimeOfFlight_Icon(3,dat1);
  } 
  else if(flag == DISP_STAT_IR)
  {
    Display_IR_Icon();
  }   
}

//LCD刷新
void LCD_ReFlash(void)
{
  g_displayState = DISP_STAT_MAIN;
  Display_Cum_Icon(WfmInfo.Normal.TotalVolumeDecimal,(unsigned long)g_waterSurfaceSpeedDisp);
}


