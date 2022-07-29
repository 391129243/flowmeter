#ifndef __LCD_H
#define __LCD_H


#define HT1621_CS1         P4OUT |= BIT3                   //LCD_CS�øߵ�ƽ 
#define HT1621_CS0         P4OUT &= ~BIT3                  //LCD_CS�õ͵�ƽ
#define HT1621_WR1         P4OUT |= BIT4                
#define HT1621_WR0         P4OUT &= ~BIT4                  
#define HT1621_DATA1       P4OUT |= BIT5                   
#define HT1621_DATA0       P4OUT &= ~BIT5    

//ht1621 command
#define HT1621_SYS_DISABLE 0x00 //�ر�ϵͳ������LCDƫѹ������
#define HT1621_SYS_ENABLE 0x01  //��ϵͳ����
#define HT1621_LCD_OFF 0x02    //�ر�LCDƫѹ������
#define HT1621_LCD_ON 0x03    //��LCDƫѹ������
#define HT1621_TIMER_DISABLE 0x04  //ʱ�����ʧЧ
#define HT1621_WDT_DISABLE 0x05  //WDT �����־���ʧЧ
#define HT1621_TIMER_ENABLE 0x06  //ʱ�����ʹ��
#define HT1621_WDT_ENABLE 0x07  //WDT �����־�����Ч
#define HT1621_TONE_OFF 0x08  //�ر��������
#define HT1621_TONE_ON 0x09  //���������
#define HT1621_CLR_TIMER 0x0c  //ʱ������������
#define HT1621_CLR_WDT 0x0e  //���WDT״̬
#define HT1621_XTAL_32K 0x14  //ϵͳʱ��Ϊ����
#define HT1621_RC_256K 0x18  //ϵͳʱ��ΪƬ��RC����
#define HT1621_EXT_256K 0x1c  //ϵͳʱ��Ϊ�ⲿʱ��Դ
#define HT1621_BIAS_2_2 0x20  //LCD1/2ƫѹ��2��������
#define HT1621_BIAS_2_3 0x24  //LCD1/2ƫѹ��3��������
#define HT1621_BIAS_2_4 0x28  //LCD1/2ƫѹ��4��������
#define HT1621_BIAS_3_2 0x21  //LCD1/3ƫѹ��2��������
#define HT1621_BIAS_3_3 0x25  //LCD1/3ƫѹ��3��������
#define HT1621_BIAS_3_4 0x29  //LCD1/3ƫѹ��4��������
#define HT1621_TONE_4K 0x40  //����Ƶ��4KHz
#define HT1621_TONE_2K 0x60  //����Ƶ��2KHz
#define HT1621_IRQ_DISABLE 0x80  //ʹ/IRQ ���ʧЧ
#define HT1621_IRQ_ENABLE 0x88  //ʹ/IRQ �����Ч
#define HT1621_F1 0xa0  //ʱ��/WDT ʱ�����1Hz
#define HT1621_F2 0xa1  //ʱ��/WDT ʱ�����2Hz
#define HT1621_F4 0xa2  //ʱ��/WDT ʱ�����4Hz
#define HT1621_F8 0xa3  //ʱ��/WDT ʱ�����8Hz
#define HT1621_F16 0xa4  //ʱ��/WDT ʱ�����16Hz
#define HT1621_F32 0xa5  //ʱ��/WDT ʱ�����32Hz
#define HT1621_F64 0xa6  //ʱ��/WDT ʱ�����64Hz
#define HT1621_F128 0xa7  //ʱ��/WDT ʱ�����128Hz



#define Icon_jiantou1_On   Icon_jiantou1_On_ht1621b()   //���¼�ͷ1��ʾ   
#define Icon_jiantou1_Off  Icon_jiantou1_Off_ht1621b()   //���¼�ͷ1����ʾ 
#define Icon_jiantou2_On   Icon_jiantou2_On_ht1621b()   //���¼�ͷ2��ʾ 
#define Icon_jiantou2_Off  Icon_jiantou2_Off_ht1621b()   //���¼�ͷ2����ʾ
#define Icon_jiantou3_On   Icon_jiantou3_On_ht1621b()   //���¼�ͷ3��ʾ 
#define Icon_jiantou3_Off  Icon_jiantou3_Off_ht1621b()   //���¼�ͷ3����ʾ

#define Icon_xiaoshudian1_On   Icon_xiaoshudian1_On_ht1621b()   //С����1��ʾ 
#define Icon_xiaoshudian1_Off  Icon_xiaoshudian1_Off_ht1621b()   //С����1����ʾ
#define Icon_xiaoshudian2_On   Icon_xiaoshudian2_On_ht1621b()   //С����2��ʾ 
#define Icon_xiaoshudian2_Off  Icon_xiaoshudian2_Off_ht1621b()    //С����2����ʾ
#define Icon_xiaoshudian3_On   Icon_xiaoshudian3_On_ht1621b()   //С����3��ʾ 
#define Icon_xiaoshudian3_Off  Icon_xiaoshudian3_Off_ht1621b()    //С����3����ʾ
#define Icon_xiaoshudian4_On   Icon_xiaoshudian4_On_ht1621b()   //С����4��ʾ 
#define Icon_xiaoshudian4_Off  Icon_xiaoshudian4_Off_ht1621b()    //С����4����ʾ
#define Icon_xiaoshudian5_On   Icon_xiaoshudian5_On_ht1621b()   //С����5��ʾ 
#define Icon_xiaoshudian5_Off  Icon_xiaoshudian5_Off_ht1621b()    //С����5����ʾ
#define Icon_xiaoshudian6_On   Icon_xiaoshudian6_On_ht1621b()   //С����6��ʾ 
#define Icon_xiaoshudian6_Off  Icon_xiaoshudian6_Off_ht1621b()    //С����6����ʾ
#define Icon_xiaoshudian7_On   Icon_xiaoshudian7_On_ht1621b()   //С����7��ʾ 
#define Icon_xiaoshudian7_Off  Icon_xiaoshudian7_Off_ht1621b()    //С����7����ʾ

#define Icon_danwei_On   Icon_danwei_On_ht1621b()   //��λ��ʾ 
#define Icon_danwei_Off  Icon_danwei_Off_ht1621b()   //��λ����ʾ


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