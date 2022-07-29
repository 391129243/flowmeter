#ifndef __LCD_H
#define __LCD_H


#define HT1621_CS1         P4OUT |= BIT3                   //LCD_CS置高电平 
#define HT1621_CS0         P4OUT &= ~BIT3                  //LCD_CS置低电平
#define HT1621_WR1         P4OUT |= BIT4                
#define HT1621_WR0         P4OUT &= ~BIT4                  
#define HT1621_DATA1       P4OUT |= BIT5                   
#define HT1621_DATA0       P4OUT &= ~BIT5    

//ht1621 command
#define HT1621_SYS_DISABLE 0x00 //关闭系统振荡器和LCD偏压发生器
#define HT1621_SYS_ENABLE 0x01  //打开系统振荡器
#define HT1621_LCD_OFF 0x02    //关闭LCD偏压发生器
#define HT1621_LCD_ON 0x03    //打开LCD偏压发生器
#define HT1621_TIMER_DISABLE 0x04  //时基输出失效
#define HT1621_WDT_DISABLE 0x05  //WDT 溢出标志输出失效
#define HT1621_TIMER_ENABLE 0x06  //时基输出使能
#define HT1621_WDT_ENABLE 0x07  //WDT 溢出标志输出有效
#define HT1621_TONE_OFF 0x08  //关闭声音输出
#define HT1621_TONE_ON 0x09  //打开声音输出
#define HT1621_CLR_TIMER 0x0c  //时基发生器清零
#define HT1621_CLR_WDT 0x0e  //清除WDT状态
#define HT1621_XTAL_32K 0x14  //系统时钟为晶振
#define HT1621_RC_256K 0x18  //系统时钟为片内RC振荡器
#define HT1621_EXT_256K 0x1c  //系统时钟为外部时钟源
#define HT1621_BIAS_2_2 0x20  //LCD1/2偏压，2个公共口
#define HT1621_BIAS_2_3 0x24  //LCD1/2偏压，3个公共口
#define HT1621_BIAS_2_4 0x28  //LCD1/2偏压，4个公共口
#define HT1621_BIAS_3_2 0x21  //LCD1/3偏压，2个公共口
#define HT1621_BIAS_3_3 0x25  //LCD1/3偏压，3个公共口
#define HT1621_BIAS_3_4 0x29  //LCD1/3偏压，4个公共口
#define HT1621_TONE_4K 0x40  //声音频率4KHz
#define HT1621_TONE_2K 0x60  //声音频率2KHz
#define HT1621_IRQ_DISABLE 0x80  //使/IRQ 输出失效
#define HT1621_IRQ_ENABLE 0x88  //使/IRQ 输出有效
#define HT1621_F1 0xa0  //时基/WDT 时钟输出1Hz
#define HT1621_F2 0xa1  //时基/WDT 时钟输出2Hz
#define HT1621_F4 0xa2  //时基/WDT 时钟输出4Hz
#define HT1621_F8 0xa3  //时基/WDT 时钟输出8Hz
#define HT1621_F16 0xa4  //时基/WDT 时钟输出16Hz
#define HT1621_F32 0xa5  //时基/WDT 时钟输出32Hz
#define HT1621_F64 0xa6  //时基/WDT 时钟输出64Hz
#define HT1621_F128 0xa7  //时基/WDT 时钟输出128Hz



#define Icon_jiantou1_On   Icon_jiantou1_On_ht1621b()   //向下箭头1显示   
#define Icon_jiantou1_Off  Icon_jiantou1_Off_ht1621b()   //向下箭头1不显示 
#define Icon_jiantou2_On   Icon_jiantou2_On_ht1621b()   //向下箭头2显示 
#define Icon_jiantou2_Off  Icon_jiantou2_Off_ht1621b()   //向下箭头2不显示
#define Icon_jiantou3_On   Icon_jiantou3_On_ht1621b()   //向下箭头3显示 
#define Icon_jiantou3_Off  Icon_jiantou3_Off_ht1621b()   //向下箭头3不显示

#define Icon_xiaoshudian1_On   Icon_xiaoshudian1_On_ht1621b()   //小数点1显示 
#define Icon_xiaoshudian1_Off  Icon_xiaoshudian1_Off_ht1621b()   //小数点1不显示
#define Icon_xiaoshudian2_On   Icon_xiaoshudian2_On_ht1621b()   //小数点2显示 
#define Icon_xiaoshudian2_Off  Icon_xiaoshudian2_Off_ht1621b()    //小数点2不显示
#define Icon_xiaoshudian3_On   Icon_xiaoshudian3_On_ht1621b()   //小数点3显示 
#define Icon_xiaoshudian3_Off  Icon_xiaoshudian3_Off_ht1621b()    //小数点3不显示
#define Icon_xiaoshudian4_On   Icon_xiaoshudian4_On_ht1621b()   //小数点4显示 
#define Icon_xiaoshudian4_Off  Icon_xiaoshudian4_Off_ht1621b()    //小数点4不显示
#define Icon_xiaoshudian5_On   Icon_xiaoshudian5_On_ht1621b()   //小数点5显示 
#define Icon_xiaoshudian5_Off  Icon_xiaoshudian5_Off_ht1621b()    //小数点5不显示
#define Icon_xiaoshudian6_On   Icon_xiaoshudian6_On_ht1621b()   //小数点6显示 
#define Icon_xiaoshudian6_Off  Icon_xiaoshudian6_Off_ht1621b()    //小数点6不显示
#define Icon_xiaoshudian7_On   Icon_xiaoshudian7_On_ht1621b()   //小数点7显示 
#define Icon_xiaoshudian7_Off  Icon_xiaoshudian7_Off_ht1621b()    //小数点7不显示

#define Icon_danwei_On   Icon_danwei_On_ht1621b()   //单位显示 
#define Icon_danwei_Off  Icon_danwei_Off_ht1621b()   //单位不显示


void InitLCD(void);
void ClearLCD(void);
void AllLCDDisplay(void);

void Icon_jiantou1_On_ht1621b(void);
void Icon_jiantou1_Off_ht1621b(void);
void Icon_jiantou2_On_ht1621b(void);
void Icon_jiantou2_Off_ht1621b(void);
void Icon_jiantou3_On_ht1621b(void);
void Icon_jiantou3_Off_ht1621b(void);

void Icon_xiaoshudian1_On_ht1621b(void);
void Icon_xiaoshudian1_Off_ht1621b(void);
void Icon_xiaoshudian2_On_ht1621b(void);
void Icon_xiaoshudian2_Off_ht1621b(void);
void Icon_xiaoshudian3_On_ht1621b(void);
void Icon_xiaoshudian3_Off_ht1621b(void);
void Icon_xiaoshudian4_On_ht1621b(void);
void Icon_xiaoshudian4_Off_ht1621b(void);
void Icon_xiaoshudian5_On_ht1621b(void);
void Icon_xiaoshudian5_Off_ht1621b(void);
void Icon_xiaoshudian6_On_ht1621b(void);
void Icon_xiaoshudian6_Off_ht1621b(void);
void Icon_xiaoshudian7_On_ht1621b(void);
void Icon_xiaoshudian7_Off_ht1621b(void);

void Icon_danwei_On_ht1621b(void);
void Icon_danwei_Off_ht1621b(void);

void Display_Date_Icon(unsigned char year, unsigned char month, unsigned char day, unsigned char hour, unsigned char minute);
void Display_Alarm_Icon(unsigned char flag);
void Display_WaterRunning_Icon(unsigned char flag);

void Display_Cum_Icon(unsigned long cum, unsigned long speed);
void Display_Temp_Icon( unsigned long t1 );

void Display_Else_Icon(unsigned char flag, unsigned long dat1, unsigned long dat2);
void LCD_ReFlash(void);

extern int positiveFlag;


#endif 