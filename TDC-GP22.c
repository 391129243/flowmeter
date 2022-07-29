#include "msp430x54x.h"
#include "tdc-gp22.h"
#include "params.h"
#include "functions.h"
#include "stdio.h"
#include "timerA0.h"
#include "string.h"
#include "lcd.h"
#include "dataSave.h"
#include "rs485.h"

#include "cluster.h"

long kalman_filter( long z);

double kalman_filterup( double zup);


double kalman_filterdown( double zdown);

//PT1000�ֶȱ�
const unsigned int tempTable[1000]=
{
0,0,1,1,2,2,2,3,3,4, //0
4,4,5,5,5,6,6,7,7,7, //1
8,8,9,9,9,10,10,11,11,11, //2
12,12,13,13,13,14,14,14,15,15, //3
16,16,16,17,17,18,18,18,19,19, //4
20,20,20,21,21,21,22,22,23,23, //5
23,24,24,25,25,25,26,26,27,27, //6
27,28,28,29,29,29,30,30,30,31, //7
31,32,32,32,33,33,34,34,34,35, //8
35,36,36,36,37,37,37,38,38,39, //9
39,39,40,40,41,41,41,42,42,43, //10
43,43,44,44,44,45,45,46,46,46, //11
47,47,48,48,48,49,49,50,50,50, //12
51,51,51,52,52,53,53,53,54,54, //13
55,55,55,56,56,57,57,57,58,58, //14
58,59,59,60,60,60,61,61,62,62, //15
62,63,63,64,64,64,65,65,65,66, //16
66,67,67,67,68,68,69,69,69,70, //17
70,71,71,71,72,72,72,73,73,74, //18
74,74,75,75,76,76,76,77,77,78, //19
78,78,79,79,79,80,80,81,81,81, //20
82,82,83,83,83,84,84,85,85,85, //21
86,86,86,87,87,88,88,88,89,89, //22
90,90,90,91,91,92,92,92,93,93, //23
93,94,94,95,95,95,96,96,97,97, //24
97,98,98,99,99,99,100,100,100,101, //25
101,102,102,102,103,103,104,104,104,105, //26
105,105,106,106,107,107,107,108,108,109, //27
109,109,110,110,111,111,111,112,112,112, //28
113,113,114,114,114,115,115,116,116,116, //29
117,117,118,118,118,119,119,120,120,120, //30
121,121,121,122,122,123,123,123,124,124, //31
124,125,125,126,126,126,127,127,128,128, //32
128,129,129,130,130,130,131,131,131,132, //33
132,133,133,133,134,134,135,135,135,136, //34
136,136,137,137,138,138,138,139,139,140, //35
140,140,141,141,141,142,142,143,143,143, //36
144,144,145,145,145,146,146,147,147,147, //37
148,148,148,149,149,150,150,150,151,151, //38
152,152,152,153,153,153,154,154,155,155, //39
155,156,156,157,157,157,158,158,158,159, //40
159,160,160,160,161,161,162,162,162,163, //41
163,164,164,164,165,165,165,166,166,167, //42
167,167,168,168,169,169,169,170,170,170, //43
171,171,172,172,172,173,173,174,174,174, //44
175,175,175,176,176,177,177,177,178,178, //45
179,179,179,180,180,180,181,181,182,182, //46
182,183,183,184,184,184,185,185,186,186, //47
186,187,187,187,188,188,189,189,189,190, //48
190,191,191,191,192,192,192,193,193,194, //49
194,194,195,195,196,196,196,197,197,197, //50
198,198,199,199,199,200,200,201,201,201, //51
202,202,202,203,203,204,204,204,205,205, //52
206,206,206,207,207,207,208,208,209,209, //53
209,210,210,211,211,211,212,212,212,213, //54
213,214,214,214,215,215,216,216,216,217, //55
217,217,218,218,219,219,219,220,220,221, //56
221,221,222,222,222,223,223,224,224,224, //57
225,225,226,226,226,227,227,227,228,228, //58
229,229,229,230,230,230,231,231,232,232, //59
232,233,233,234,234,234,235,235,235,236, //60
236,237,237,237,238,238,239,239,239,240, //61
240,240,241,241,242,242,242,243,243,244, //62
244,244,245,245,245,246,246,247,247,247, //63
248,248,249,249,249,250,250,250,251,251, //64
252,252,252,253,253,254,254,254,255,255, //65
255,256,256,257,257,257,258,258,258,259, //66
259,260,260,260,261,261,262,262,262,263, //67
263,263,264,264,265,265,265,266,266,267, //68
267,267,268,268,268,269,269,270,270,270, //69
271,271,272,272,272,273,273,273,274,274, //70
275,275,275,276,276,277,277,277,278,278, //71
278,279,279,280,280,280,281,281,281,282, //72
282,283,283,283,284,284,285,285,285,286, //73
286,286,287,287,288,288,288,289,289,289, //74
290,290,291,291,291,292,292,293,293,293, //75
294,294,294,295,295,296,296,296,297,297, //76
298,298,298,299,299,299,300,300,301,301, //77
301,302,302,302,303,303,304,304,304,305, //78
305,306,306,306,307,307,307,308,308,309, //79
309,309,310,310,310,311,311,312,312,312, //80
313,313,314,314,314,315,315,315,316,316, //81
317,317,317,318,318,319,319,319,320,320, //82
320,321,321,322,322,322,323,323,323,324, //83
324,325,325,325,326,326,327,327,327,328, //84
328,328,329,329,330,330,330,331,331,331, //85
332,332,333,333,333,334,334,335,335,335, //86
336,336,336,337,337,338,338,338,339,339, //87
339,340,340,341,341,341,342,342,342,343, //88
343,344,344,344,345,345,346,346,346,347, //89
347,347,348,348,349,349,349,350,350,350, //90
351,351,352,352,352,353,353,354,354,354, //91
355,355,355,356,356,357,357,357,358,358, //92
358,359,359,360,360,360,361,361,362,362, //93
362,363,363,363,364,364,365,365,365,366, //94
366,366,367,367,368,368,368,369,369,369, //95
370,370,371,371,371,372,372,373,373,373, //96
374,374,374,375,375,376,376,376,377,377, //97
377,378,378,379,379,379,380,380,381,381, //98
381,382,382,382,383,383,384,384,384,385, //99
  };

//SPI��ʼ��
void initSPI(void)
{
    //SPI-TD_GP21��SPIͨ������
  P9SEL |= BIT1 + BIT2 + BIT3;     //SPI��������,��ΪƬ������Ķ���ӿ�
  P9DIR |= BIT1 + BIT3;           //MO,CLK��Ϊ��� 
  P9DIR &= ~BIT2;                 //MIΪ����
  P9DIR |= BIT4;                  //��Ϊ�������TDC_SSN��
  
  P2IES |= BIT1;  
  P2IE |= BIT1;
  P2DIR &= ~BIT1;                //��Ϊ���룬��TDC_INT��
  
  P9DIR |= BIT0;                //��Ϊ�������TDC_RSTN��
  P9DIR |= BIT5;                //��Ϊ�������TDC_Fire_IN��
  P9DIR |= BIT6;                 //��Ϊ�������TDC_EN_Start��
  P9DIR |= BIT7;                 //��Ϊ�������TDC_Start��
  P8DIR |= BIT6;                 //��Ϊ�������TDC_EN_Stop1��
  P8DIR |= BIT7;                //��Ϊ�������TDC_EN_Stop2��
  
  P8DIR |= BIT4;                 //��Ϊ�������Channel_Switch1��
  P8DIR |= BIT5;                 //��Ϊ�������Channel_Switch2��
  
  
  //TDC-GP21��SPI�ӿ�
  UCB2CTL1 |= UCSWRST;             // ��λSPI����
  UCB2CTL0 &= ~UCCKPH;
  //UCB2CTL0 |= UCCKPH;              // ��TDC��������λ1����ʱ����λѡ��=0
  UCB2CTL0 &= ~UCCKPL;             // ʱ�ӿ���״̬��Ϊ�͵�ƽ 
  UCB2CTL0 |= UCMSB;               // MSB�ȴ�,��λ�ȷ���
  UCB2CTL0 &= ~UC7BIT;             // 8λ����ģʽ
  UCB2CTL0 |= UCMST;               // ��ģʽ
  UCB2CTL0 &= ~UCMODE0;           // ѡ��ģʽ0������SPI
  UCB2CTL0 &= ~UCMODE1;          
  UCB2CTL0 |= UCSYNC;              // ͬ��
  
  UCB2CTL1 &= ~UCSSEL_2;                                       
  UCB2CTL1 |= UCSSEL_1;            // ʱ��Դѡ��ACLK    
  
  UCB2BR0 = 0x03;                // 32768/9600=3.413
  UCB2BR1 = 0x00;
  UCA0MCTL = 0x06;               // �����ʵ�������
  
  UCB2CTL1 &= ~UCSWRST;          // ʹ��SPI����,�Ӹ�λģʽ������
  UCB2IFG &= ~UCRXIFG;          // ������պͷ����жϱ�־
                                //�����жϱ�־λĬ��Ϊ��
  //UCB2IFG &= ~UCTXIFG;
 // UCB2IE |= UCRXIE+UCTXIE;      // ʹ�ܽ���/�����ж� 
  
  TDC_CS_1;
  
  TDC_Channel_Switch2_0;
  TDC_Channel_Switch1_0;
  TDC_START_1;
  TDC_EN_STOP1_1;
  TDC_EN_STOP2_1;
  TDC_EN_START_1;
  TDC_RESET_1;
} 
  
void writeByteTDCGP21(unsigned char ch)
{
  UCB2TXBUF = ch;
  while (!(UCB2IFG & UCTXIFG)); //�ȴ���һ���ֽڷ������
  UCB2IFG &= ~UCTXIFG; //�巢����ɱ�־λ
}

unsigned char readByteTDCGP21(void)
{
  while(!(UCB2IFG & UCRXIFG));  //�ȴ���ǰ�ֽڽ������
  UCB2IFG &= ~UCRXIFG;    //�������ɱ�־λ
  return UCB2RXBUF;
}

unsigned char readAndWriteByteTDCGP21(unsigned char data)
{
  writeByteTDCGP21(data);
  return readByteTDCGP21();
}


void resetTDCGP21(void)
{
  TDC_RESET_1;
  _NOP();
  _NOP();
  _NOP();
  TDC_RESET_0;
  for(unsigned int i = 0; i < 65000; i++)
  {
    _NOP();
    _NOP();
    _NOP();
  }
  TDC_RESET_1; 
  _NOP();
  _NOP();
  _NOP();  
}

void powerOnResetTDCGP21(void)
{
  TDC_CS_0;
  _NOP();
  _NOP();
  _NOP();
  readAndWriteByteTDCGP21(POWER_ON_RESET);
  _NOP();
  _NOP();
  _NOP(); 
  TDC_CS_1;
}

void initMeasureTDCGP21(void)
{
  TDC_CS_0;
  _NOP();
  _NOP();
  _NOP();
  readAndWriteByteTDCGP21(INIT_MEASURE);
  _NOP();
  _NOP();
  _NOP(); 
  TDC_CS_1;
}

void calibrateResonatorTDCGP21(void)
{
  TDC_CS_0;
  _NOP();
  _NOP();
  _NOP();
  readAndWriteByteTDCGP21(START_CAL_OSC);
  _NOP();
  _NOP();
  _NOP();  
  TDC_CS_1;
}

void timeFlightStartTDCGP21(void)
{
  TDC_CS_0;
  _NOP();
  _NOP();
  _NOP();
  readAndWriteByteTDCGP21(START_TOF);
  _NOP();
  _NOP();
  _NOP();
  TDC_CS_1;
}

void timeFlightRestartTDCGP21(void)
{
  TDC_CS_0;
  _NOP();
  _NOP();
  _NOP();
  readAndWriteByteTDCGP21(START_TOF_RESTART);
  _NOP();
  _NOP();
  _NOP();
  TDC_CS_1;
}

void tempStartTDCGP21(void)
{
  TDC_CS_0;
  _NOP();
  _NOP();
  _NOP();
  readAndWriteByteTDCGP21(START_TEMP);
  _NOP();
  _NOP();
  _NOP();
  TDC_CS_1;
}


void configureRegisterTDCGP21(unsigned char opcode_address,unsigned long config_reg_data)
{
  unsigned char data_byte_lo;
  unsigned char data_byte_mid1;
  unsigned char data_byte_mid2;
  unsigned char data_byte_hi;
  
  data_byte_lo = config_reg_data;
  data_byte_mid1 = config_reg_data>>8;
  data_byte_mid2 = config_reg_data>>16;
  data_byte_hi = config_reg_data>>24;
  
  TDC_CS_0;
  _NOP();
  _NOP();
  _NOP();
  readAndWriteByteTDCGP21(opcode_address);
  readAndWriteByteTDCGP21(data_byte_hi);
  readAndWriteByteTDCGP21(data_byte_mid2);
  readAndWriteByteTDCGP21(data_byte_mid1);
  readAndWriteByteTDCGP21(data_byte_lo); 
  _NOP();
  _NOP();
  _NOP();
  TDC_CS_1;
}


unsigned int readStatusRegisterTDCGP21(void)
{
  unsigned int result = 0;
  
  TDC_CS_0;
  _NOP();
  _NOP();
  _NOP();
  readAndWriteByteTDCGP21(READ_STAT);
    //����һ���ֽ�
  result |= readAndWriteByteTDCGP21(DUMMY_DATA);
  result <<= 8;
    //���ڶ����ֽ�
  result |= readAndWriteByteTDCGP21(DUMMY_DATA);
  _NOP();
  _NOP();
  _NOP();
  TDC_CS_1;
  
  return result;
}

double readPW1STRegisterTDCGP22(void)
{
  double result = 0;
  unsigned char resultByte;
  
  TDC_CS_0;
  _NOP();
  _NOP();
  _NOP();
  readAndWriteByteTDCGP21(READ_PW1ST);
    //����һ���ֽ�
  resultByte = readAndWriteByteTDCGP21(DUMMY_DATA);
  _NOP();
  _NOP();
  _NOP();
  TDC_CS_1;
  
  if( (resultByte & 0x80) != 0x00 )
  {
    result += 1;
  }
  resultByte = resultByte & 0x7F;
  result = result + (double)(resultByte >> 3)/16 + (double)((resultByte << 1) & 0x0F)/256; 
  
  return result;
}

unsigned long readRegisterTDCGP21(unsigned char read_opcode_address)
{
  unsigned long result_read = 0;
  
  TDC_CS_0;
  _NOP();
  _NOP();
  _NOP();
  readAndWriteByteTDCGP21(read_opcode_address);
  //����һ���ֽ�
  result_read |= readAndWriteByteTDCGP21(DUMMY_DATA);
  result_read <<= 8;
  //���ڶ����ֽ�
  result_read |= readAndWriteByteTDCGP21(DUMMY_DATA);
  result_read <<= 8;
  //���������ֽ�
  result_read |= readAndWriteByteTDCGP21(DUMMY_DATA);
  result_read <<= 8;
  //�����ĸ��ֽ�
  result_read |= readAndWriteByteTDCGP21(DUMMY_DATA);
  _NOP();
  _NOP();
  _NOP();
  TDC_CS_1;
  
  return result_read;
}


unsigned char ID_Bytes[7];
//testcomunication();
void readIDbytesTDCGP21(void)
{ 
  TDC_CS_0;
  _NOP();
  _NOP();
  _NOP();
  readAndWriteByteTDCGP21(READ_IDBIT);
  //����һ���ֽ�
  ID_Bytes[0] = readAndWriteByteTDCGP21(0xff);
  //���ڶ����ֽ�
  ID_Bytes[1] = readAndWriteByteTDCGP21(0xff);
  //���������ֽ�
  ID_Bytes[2] = readAndWriteByteTDCGP21(0xff);
  //�����ĸ��ֽ�
  ID_Bytes[3] = readAndWriteByteTDCGP21(0xff);
  //��������ֽ�
  ID_Bytes[4] = readAndWriteByteTDCGP21(0xff);
  //���������ֽ�
  ID_Bytes[5] = readAndWriteByteTDCGP21(0xff); 
  //�����߸��ֽ�
  ID_Bytes[6] = readAndWriteByteTDCGP21(0xff);
  _NOP();
  _NOP();
  _NOP();
  TDC_CS_1;
}

void initConfigureRegisterTDCGP21( void )
{
  resetTDCGP21();
  powerOnResetTDCGP21();
  for(unsigned int i = 0; i < 65000; i++);
  initMeasureTDCGP21();
  configureRegisterTDCGP21( WRITE_REG0,0xd107ef00);//  0xd1c7f800
  //configureRegisterTDCGP21( WRITE_REG0, 0xd1c7d800 );
  //configureRegisterTDCGP21( WRITE_REG0, 0xD3C7E800 );     /// Configure TDC register.
	// ���õ���λ�����������������Ϊ20����            4M�մɾ���1MHz���壬����ʱ���źŲ��������Ƶ����Ϊ3=4��Ƶ��
	// ����У׼�մɾ�����Ҫ32.768KHz������Ϊ3=16������=488.28125us��                  ���ø��ٲο�ʱ��CLKHS��Ƶ����Ϊ0=����Ƶ��
	// ���ø��ٲο�ʱ��CLKHS�����Ϳ�ʼ����ǰ��ʱ����Ϊ1��һֱ����;               �����¶Ȳ����˿���Ϊ1=4���¶Ȳ����˿ڣ�
	// �����¶Ȳ�����ѭ��ʱ��Ϊ1=512us��              �����¶Ȳ���ǰ�ļٲ�������Ϊ1=7�Σ�
	// �����¶Ȳ����ο�ʱ���ź�Ϊ1=128*CLSHS��             ����ALU�е�У׼Ϊ1=У׼������
	// ����TDC�����е��Զ�У׼Ϊ0=�ڲ������Զ�У׼��               ���ò�����ΧΪ1=ѡ�������Χ2��500ns~4ms��,��Χ2����>2��s��ʱ�
	// ����stop1,stop2,start�ź�Ϊ0=�Ƿ��������źţ������أ���
  //configureRegisterTDCGP21( WRITE_REG1, 0x21444001 );
  configureRegisterTDCGP21( WRITE_REG1, 0x2142c001 );
  //configureRegisterTDCGP21( WRITE_REG1, 0x2144c001 );
  
	// ����ALU���ݴ���ļ��㹫ʽ����������Χ1��HIT1-HIT2��,��������Χ2��HIT2-start��
	// ����HIT2Ϊ2=1.Stop Ch1��Stopͨ��1�ĵ�һ�������źţ���                    ����HIT1Ϊ1=Start��Start�����źţ���
	// ���ÿ��ٳ�ʼ������Ϊ0=�رոù��ܣ�                  ������һ������λ��                    ����Stopͨ��2��Ԥ��������Ϊ0=�ر�Stopͨ��2��
	// ��������Stopͨ��1��Ԥ�����������Ϊ4=4������(ʵ��ֻ��3����Start��һ��)��           ����32.768KHz����ĵ͹���Ϊ0=�͵�����
	// ����fire������Ϊ����TDC_Start�ź�Ϊ1=Ӧ��fire�ڲ�����Start��          ����EN_START�ܽŹ���Ϊ1=START_TDC�ź������
	// ����FIRE_IN�ܽŹ���Ϊ7=32KHz �ź������
  //configureRegisterTDCGP21( WRITE_REG2, 0xA0140002 );
  //configureRegisterTDCGP21( WRITE_REG2, 0xa0050002 );//10us
  //configureRegisterTDCGP21( WRITE_REG2, 0xa0040002 );//8us
    configureRegisterTDCGP21( WRITE_REG2, 0xe0040002 );//8us
	// ����TimeOut(���)�жϴ�����Ч��EndHits(�ﵽԤ��������)�жϴ�����Ч��ALU(ALU���ݴ������)�жϴ�����Ч���ж϶�������
	// ����ͨ��2�ı���������Ϊ0=�����ػ����½���,�����Ӧ�������أ�
	// ����ͨ��1�ı���������Ϊ0=�����ػ����½���,�����Ӧ�������أ�
	// �����ڲ�����Stop1ͨ�����յ�1���źŵ��ӳ�ʱ��ΪDELVAL1=160.0,��һ��stop������40��s��Żᱻ����
	// (��19λ,����14λ��������,5λС������)��
  //configureRegisterTDCGP21( WRITE_REG3, 0xF8A24803 );
  //configureRegisterTDCGP21( WRITE_REG3, 0xf8718503 );
  configureRegisterTDCGP21( WRITE_REG3, 0xf0a24803 );
	// �����ڲ�����Χ2���Զ��������п�����õ�����TOF�����������ƽ��ֵд�����Ĵ���3���У�����Ϊ1=�����ù��ܣ�
        // �����Ƿ�����һ����⹦�ܣ�����Ϊ1=�����ù��ܣ�
	// ��������timeoutǿ��ALUд��0xFFFFFFFF������Ĵ����Ĺ���Ϊ1=�����ù���,��ֹ��ȡ״̬�Ĵ�����ʱ�䲻����
	// ���ò�����Χ2�����ʱ������Ϊ3=4096us,��4096��s�����û���ź�����������
	// �����ڵ�һ����⵽�󣬵�3��stop�ǽ��յڼ����ز����ڣ�����ΪA=��һ����ĵ�10���ز�Ϊ��3��stop;
	// �����ڵ�һ����⵽�󣬵�2��stop�ǽ��յڼ����ز����ڣ�����Ϊ9=��һ����ĵ�9���ز�Ϊ��2��stop;
        // �����ڵ�һ����⵽�󣬵�1��stop�ǽ��յڼ����ز����ڣ�����Ϊ8=��һ����ĵ�8���ز�Ϊ��1��stop;
  //configureRegisterTDCGP21( WRITE_REG4, 0x10004f04 );
  
  //configureRegisterTDCGP21( WRITE_REG4, 0x10004004 );
  
  
  //configureRegisterTDCGP21( WRITE_REG4, 0x20004a04 );
  
  
  
 //configureRegisterTDCGP21( WRITE_REG4, 0x1000bf04 );
  
   configureRegisterTDCGP21( WRITE_REG4, 0x1000a004 );
  
	// ������ʮ�������λ��
        // �����Ƿ�ر������Ȳ������ܣ�����Ϊ0=�����ù��ܣ�
        // ���õ�һ��ʶ��ı������У�����Ϊ0=�����أ�
        // �����Ƿ��������offset +20mV������Ϊ1=�����ù���;
        // �����Ƿ��������offset -20mV������Ϊ0=�رոù��ܣ�
	// ���ñȽ���offset����λΪ1mV, ����ΪF=+10mV;�����ܵ�offsetΪ (+20mV)+(+10mV) = +30mV;
  configureRegisterTDCGP21( WRITE_REG5, 0x80000005 );
  //configureRegisterTDCGP21( WRITE_REG5, 0x50000005 );
	// ���ùر�FIRE_UP���ȴ�FIRE_DOWN���β�����ʼ,����ĸ��������λ�������������������
	// ����Startͨ���Ƿ��������Ϊ0=�ر�������Ԫ��                �����Ƿ�ر���λ������ԪΪ0=������λ������λ��Ԫ��
	// �����������������������е��ظ�����Ϊ0=���ظ���������������               �����������е���λ�Ƿ���Ҫ�ı�Ϊ0=���ı�����λ��
  //configureRegisterTDCGP21( WRITE_REG6, 0xC0E06106 );
  //configureRegisterTDCGP21( WRITE_REG6, 0xd000e006 );
  configureRegisterTDCGP21( WRITE_REG6, 0xd0c0e006 );
	// �����Ƿ���������������������Ҫ��ģ���������Ϊ1=����ģ�ⲿ�ֲ�����
	// �����Ƿ�Ӧ���ڲ����¶Ȳ����õ�ʩ���ش�����Ϊ1=Ӧ���ڲ�ʩ���ش�������                          ������һ������λ��
        // ���ñȽ���offset������Ϊ0=0mV;
	// �����ڲ�ģ���·RC�ĵ��ݳ��ʱ��Ϊ3=300us����TW2 =3, 300��s �ӳ�������ͨ���ݽ��г�磻
	// �����Ƿ�����EEPROM���������ж�Ϊ1=����EEPROM ����������
  // ���ø��ٲο�ʱ��CLKHS�����Ϳ�ʼ����ǰ��ʱ���������λ������ֵΪ1:һֱ������
	// ����һ�������Ĳ��������ڴ����ڶ����¶Ȳ�����ʱ�䣬��50/60Hz����������Ϊ0=1.0����
	// ����һ�������Ĳ��������ڴ�������ʱ�������ʱ�䣬��50/60Hz����������Ϊ0=1.0����
	// ����һ�������Ĳ��������ڣ����β���֮����ӳ�ʱ���ǻ���50Hz����60Hz��Ϊ0=50Hz=20ms��
	// ����fire_up��fire_down�����ڷǻ״̬ʱ�ĵ�ƽ�����Ӧ��оƬ�ڲ���ģ�ⲿ�֣��������Ϊ1=�͵�ƽ��
	// ���ò�����Χ2�У��������������4����Ϊ1=������
	// ���ò�����Χ2�У����������ȼӱ���Ϊ0=����������������4����ģʽ��ʱ�����������Ϊ0��
	// �����¶Ȳ����˿ڲ���˳���Ƿ���Ϊ0=�����򣬲���˳��ΪPT1->PT2->PT3->PT4;
	// ���ø���λ�����������������Ϊ20����
  
  
}


void calibrateResonator( void )
{
  unsigned long temp;

  TDC_EN_START_1;  /// In calibrate resonator, EN_START pin must be to set 1.

  initMeasureTDCGP21();                                   /// Init TDC measure.
  calibrateResonatorTDCGP21();                            /// Calibrate resonator.

  while( 0 == g_tdcInterruptFlag );
  g_tdcInterruptFlag = 0;
  g_tdcStatusRegister = readStatusRegisterTDCGP21();
  
  temp = readRegisterTDCGP21(READ_RES0+(g_tdcStatusRegister&0x0007)-1);
  g_calibrateResult = dotHextoDotDec(temp);
  // 16*(1/32768)/0.25*1000000=1953.125
  //g_calibrateCorrectionFactor = 1953.125/g_calibrateResult;
  
   // 2*(1/32768)/0.25*1000000=1953.125
  g_calibrateCorrectionFactor = 244.140625/(g_calibrateResult+1);
}


void tempMeasure( void )
{
  unsigned long temp;

  tempStartTDCGP21();                                   

  while( 0 == g_tdcInterruptFlag );
  g_tdcInterruptFlag = 0;
  g_tdcStatusRegister = readStatusRegisterTDCGP21();
   if( (g_tdcStatusRegister & 0x1600) != 0x0000 )  /// Timeout error.
   {          
     return;  /// �¶�������Ч
   }
   else
   {     
     temp = readRegisterTDCGP21(READ_RES0);
     g_timeResult0 = dotHextoDotDec(temp); //PT1��RED PT1000��
     temp = readRegisterTDCGP21(READ_RES1);
     g_timeResult1 = dotHextoDotDec(temp); //PT2(BLUE ����)ֻ��һ���¶ȴ�����
     temp = readRegisterTDCGP21(READ_RES2);
     g_timeResult2 = dotHextoDotDec(temp); //PT3(�ο�����) 
     temp = readRegisterTDCGP21(READ_RES3);
     g_timeResult3 = dotHextoDotDec(temp); //PT4(�ο�����) 
     
     g_tempHot = g_timeResult0/g_timeResult2*1000-1000;
     //g_tempClod = g_timeResult1/g_timeResult3*1000-1000;
     if( g_tempHot<0 )
       g_tempHot= g_tempHot*(-1);
     
     if(g_tempHot>400) // �¶ȴ�������·
       g_tempHot = 0;

     while(g_tempHot<tempTable[g_tempTableP])
     {
       if(g_tempTableP>0)
       {
         g_tempTableP--;
       }
       else
       {
         break;
       } 
     }
     while(g_tempHot>tempTable[g_tempTableP])
     {
       if(g_tempTableP<999)
       {
         g_tempTableP++;
       }
       else
       {
         break;
       }         
     }
     g_tempValue = g_tempTableP;
   }
   
}

 
 
unsigned char displayFre;
unsigned char tempMeasureOk;
unsigned int time = 0;

unsigned int timecalibratorcount = 0;

void ultrasonicTimeOfFlightMeasure(void)
{
     unsigned int i;
     unsigned long temp,temp1;
     
     
     
     //unsigned char time=BCDtoDec(g_tagRTC.g_Second);
     /*if((time++%30==0)&&(tempMeasureOk==0)) //2 second���¶�һ��
     {
       tempMeasureOk = 1;
       tempMeasure();
     }
     else if(time%30==3)
     {
       tempMeasureOk = 0;
     }
     */
     
     g_downTimeOutFlag = 0;
     g_upTimeOutFlag = 0;
     initMeasureTDCGP21();
     //timeFlightRestartTDCGP21();
     timeFlightStartTDCGP21();
     while( (0 == g_tdcInterruptFlag) );
     g_tdcInterruptFlag = 0;
     g_tdcStatusRegister = readStatusRegisterTDCGP21();
     // Error
     if( ((g_tdcStatusRegister & 0x0600) != 0x0000) )  /// Timeout error.
     {          
       g_downTimeOutFlag = 1;  /// �չܶ�
     }
     else
     {     
       temp = readRegisterTDCGP21(READ_RES3);
       g_timeResult3 = dotHextoDotDec(temp);
       g_averageTimeResultDown = g_timeResult3;
       
       //temp = readRegisterTDCGP21(READ_RES0);
       //g_timeResultdown0 = dotHextoDotDec(temp);
       //temp = readRegisterTDCGP21(READ_RES1);
       //g_timeResultdown1 = dotHextoDotDec(temp);
       //temp = readRegisterTDCGP21(READ_RES2);
       //g_timeResultdown2 = dotHextoDotDec(temp);
     }
     configureRegisterTDCGP21( WRITE_REG5, 0x30000005 );//�л����β���
     DelayNS(1);//�ȴ�����2.8us 
     
     initMeasureTDCGP21();
     timeFlightStartTDCGP21();
     while( (0 == g_tdcInterruptFlag) );    
     g_tdcInterruptFlag = 0;
     g_tdcStatusRegister = readStatusRegisterTDCGP21();
     // Error
     if( ((g_tdcStatusRegister & 0x0600) != 0x0000) )  /// Timeout error.
     {          
       g_upTimeOutFlag = 1;   /// �չܶ�
     }
     else
     {          
       temp = readRegisterTDCGP21(READ_RES3);
       g_timeResult3 = dotHextoDotDec(temp);
       g_averageTimeResultUp = g_timeResult3;  
       
       //temp = readRegisterTDCGP21(READ_RES0);
       //g_timeResultup0 = dotHextoDotDec(temp);
       //temp = readRegisterTDCGP21(READ_RES1);
       //g_timeResultup1 = dotHextoDotDec(temp);
       //temp = readRegisterTDCGP21(READ_RES2);
       //g_timeResultup2 = dotHextoDotDec(temp);
     }
     configureRegisterTDCGP21( WRITE_REG5, 0x50000005 );//�л����β���
     DelayNS(1);//�ȴ�����2.8us 
     
     if( (0 == g_downTimeOutFlag) && (0 == g_upTimeOutFlag) )
     {
       if( ( (g_averageTimeResultDown<=60)||(g_averageTimeResultDown>=65) ) || ( (g_averageTimeResultUp<=60)||(g_averageTimeResultUp>=65) ) )
       {       
         Display_Alarm_Icon(1);
         //initConfigureRegisterTDCGP21();
         return;
       }
       
       
       g_PW1STValue = 0.0;
     
       g_PW1STValue = readPW1STRegisterTDCGP22();//��ȡ�ײ������
       
       if(g_PW1STValue<0.6){/////��� PW1ST < 0.3 �ź�̫��, �򷢳������źš�
         
         Display_Alarm_Icon(1);
         //initConfigureRegisterTDCGP21();
         return;
       }
       
       DelayNS(1);//�ȴ�����2.8us 
       
       calibrateResonator();
       
    
       //g_averageTimeResultDown = g_averageTimeResultDown*0.25/g_calibrateCorrectionFactor;
       //g_averageTimeResultUp = g_averageTimeResultUp*0.25/g_calibrateCorrectionFactor;
       //g_averageTimeResultDown = g_averageTimeResultDown*0.25;
       //g_averageTimeResultUp = g_averageTimeResultUp*0.25;
       //g_timeOfFlight = (long)((g_averageTimeResultDown - g_averageTimeResultUp)*1000000*0.25);    
       
       
       
       g_averageTimeResultDown = kalman_filterdown(g_averageTimeResultDown * g_calibrateCorrectionFactor);
       
       
       
       g_averageTimeResultUp = kalman_filterup(g_averageTimeResultUp * g_calibrateCorrectionFactor);
       
       
      ////////////////////////////////////////////////////////////////////////////// 
      for( i = 19; i > 0; i-- )  //������һ�εķ���ʱ�����������뻺������8 bits��
      {
        g_timeOfFlightUpBuffer[i] = g_timeOfFlightUpBuffer[i-1];
      }
      g_timeOfFlightUpBuffer[0] = g_averageTimeResultUp;

      
      for( i = 0; i < 20; i++ )
      {
        tempValueUpBuffer[i] = g_timeOfFlightUpBuffer[i];
      }
      unsigned jUp, kUp;
      double tempUpValue;
      for( jUp = 0; jUp < 20-1; jUp++ )
      {
        for( kUp = 0; kUp < 20-jUp-1; kUp++ )
        {
          if( tempValueUpBuffer[kUp] > tempValueUpBuffer[kUp+1] )
          {
            tempUpValue = tempValueUpBuffer[kUp];
            tempValueUpBuffer[kUp] = tempValueUpBuffer[kUp+1];
            tempValueUpBuffer[kUp+1] = tempUpValue;
          }
        }
      }
      double tempValueUpSum = 0;

      
      
      for( i = 5; i < 15; i++ )
      {
        tempValueUpSum += tempValueUpBuffer[i];
        
      }
      
      
      
      
      g_averageTimeResultUp = tempValueUpSum;
      
      
     
      
      //////////////////////////////////////////////////
       for( i = 19; i > 0; i-- )  //������һ�εķ���ʱ�����������뻺������8 bits��
      {
        g_timeOfFlightDownBuffer[i] = g_timeOfFlightDownBuffer[i-1];
      }
      g_timeOfFlightDownBuffer[0] = g_averageTimeResultDown;

      
      for( i = 0; i < 20; i++ )
      {
        tempValueDownBuffer[i] = g_timeOfFlightDownBuffer[i];
      }
      unsigned jDown, kDown;
      double tempDownValue;
      for( jDown = 0; jDown < 20-1; jDown++ )
      {
        for( kDown = 0; kDown < 20-jDown-1; kDown++ )
        {
          if( tempValueDownBuffer[kDown] > tempValueDownBuffer[kDown+1] )
          {
            tempDownValue = tempValueDownBuffer[kDown];
            tempValueDownBuffer[kDown] = tempValueDownBuffer[kDown+1];
            tempValueDownBuffer[kDown+1] = tempDownValue;
          }
        }
      }
      double tempValueDownSum = 0;

      
      
      for( i = 5; i < 15; i++ )
      {
        tempValueDownSum += tempValueDownBuffer[i];
        
      }
      g_averageTimeResultDown = tempValueDownSum;
       
      
      
      
      //g_calibrateCorrectionFactor = 1;
     
     
       
       
       double temp1 = ((g_averageTimeResultDown - g_averageTimeResultUp)*10000.0);    
        

       g_timeOfFlight = (long)temp1;
     
       if( g_timeOfFlight > 5000 || g_timeOfFlight < -5000 )
       {       
         Display_Alarm_Icon(1);
         //initConfigureRegisterTDCGP21();
         return;
       }
       
       
       DelayNS(1);//�ȴ�����2.8us 
       
       
       Display_Alarm_Icon(0);
       
       
       g_timeOfFlight = kalman_filter(g_timeOfFlight);
       

        for( i = 29; i > 0; i-- )  //������һ�εķ���ʱ�����������뻺������8 bits��
        {
          g_timeOfFlightBuffer[i] = g_timeOfFlightBuffer[i-1];
        }
        g_timeOfFlightBuffer[0] = g_timeOfFlight;

        
        for( i = 0; i < 30; i++ )
        {
          tempValueBuffer[i] = g_timeOfFlightBuffer[i];
        }
        unsigned j, k;
        long tempValue;
        for( j = 0; j < 30-1; j++ )
        {
          for( k = 0; k < 30-j-1; k++ )
          {
            if( tempValueBuffer[k] > tempValueBuffer[k+1] )
            {
              tempValue = tempValueBuffer[k];
              tempValueBuffer[k] = tempValueBuffer[k+1];
              tempValueBuffer[k+1] = tempValue;
            }
          }
        }
        long tempValueSum = 0;


        int count = 0;
        for( i = 10; i < 20; i++ )
        {
          tempValueSum += tempValueBuffer[i];
          
        }
        tempValueSum = tempValueSum/10;
        
        //tempValueSum = (tempValueBuffer[6] + tempValueBuffer[7] + tempValueBuffer[13] + tempValueBuffer[14]) /40;
        
        //tempValueSum = kalman_filter(tempValueSum);
        
        g_timeOfFlight_ave = tempValueSum;
        
        g_tofDisplay = tempValueSum;
        
        
        
        

        g_valueOpenFlag =1;
        if( 0 == g_valueOpenFlag ) //����δ����
        {
            //if( tempValueSum >= TofThresholdVale )
            //{
                    //g_valueOpenCount++;
                    //if( g_valueOpenCount >= 3 )
                    //{
                    //        g_valueOpenFlag = 1; //���ſ���
                    //        //Display_WaterRunning_Icon(1);
                    //        g_valueOpenCount = 0;
                    //        if(WfmInfo.State== WFM_STATE_IDLE)
                    //        {
                    //                WfmInfo.State = WFM_STATE_RUN;
                    //                InitTimerA0(WfmInfo.Interval); //����2s��ʱ��
                    //        }
                    //}
            //}
            //else
            //{
                    //g_valueOpenCount = 0;
            //}
        }
        else
        {
          // ����ʽ�ܶΣ������ٶȲ����ܶ���ƽ���ٶȹ�ʽ��V = L/2*(Ttof/Tdown/Tup),L = 0.06
          // �������ٶȵĵ�λΪ��mm/s
          //g_waterLineSpeed = 0.06/2*(tempValueSum/g_averageTimeResultDown/g_averageTimeResultUp)*1000;
          //g_waterLineSpeed = tempValueSum/(g_averageTimeResultDown-20)/(g_averageTimeResultUp-20)*30;
          //g_waterLineSpeed = 0.0183027*tempValueSum;
          // ���Թܶ�DN = 20mm
          // ���ٶ�ת��Ϊ���ٶ�����򵥵ش���Ϊ�����ٶ�=���ٶ�*3/4����ʵ��Ӧ���У�Ҫ�÷ֶ�ֱ��ȥ��ϣ�����ο����ģ�����������ˮ��������Էֶ�У���������о�������ˮ�� ��Ҧ�飬���ö��� ��ǿ��
          // ˲ʱ����qv = S*Lv = Kh*(3.1415926*0.02*0.02/4)*g_waterLineSpeed
          // �������ٶȵĵ�λΪ��L/s
          //g_waterSurfaceSpeed = 0.75*(3.1415926*0.02*0.02/4)*g_waterLineSpeed*1000;
          //g_waterSurfaceSpeed = g_waterLineSpeed*0.235619445;
          
          g_waterLineSpeed = 1450.0 *1450.0 *tempValueSum / 2 / 0.0045 / 0.707 /1000000/1000000 ;
          
          g_waterSurfaceSpeed = 0.75 * g_waterLineSpeed  * 0.5 * 0.5 * 60;
             /*if( tempValueSum < 300 )
            {
                    g_valueOpenCount++;
                    if( g_valueOpenCount >= 3 )
                    {
                            g_valueOpenFlag = 0; //���Źر�
                            Display_WaterRunning_Icon(0);
                            g_valueOpenCount = 1;
                            WfmInfo.State 	= WFM_STATE_IDLE;
                            
                            StopTimerA0();
                            g_waterSurfaceSpeed = 0;
                            g_waterSurfaceSpeedDisp = 0;

                    }
            }
            else
            {
                    g_valueOpenCount = 0;
            }*/
            
        }
       
        
        displayFre++;
        if( displayFre >= 1 )//��ʾ��ˢ��Ƶ��
        {
            displayFre = 0;
            
            if( g_displayState == DISP_STAT_MAIN )               
            {
              temp = WfmInfo.Normal.TotalVolume;
              //g_waterSurfaceSpeedDisp = g_waterSurfaceSpeed*3.6;//��λת��Ϊm3/h
              g_waterSurfaceSpeedDisp = g_waterSurfaceSpeed;//��λת��Ϊm3/h
              if( g_waterSurfaceSpeedDisp < 0 )
                g_waterSurfaceSpeedDisp *= -1;
              temp1 = (unsigned long)(g_waterSurfaceSpeedDisp);          
              
              Display_Else_Icon(DISP_STAT_MAIN,temp,temp1);
            
              if(WfmInfo.State == WFM_STATE_RUN)  
                Display_WaterRunning_Icon(1);
            }
            
            else if( g_displayState == DISP_STAT_TIME )                
            {
              Display_Else_Icon(DISP_STAT_TIME,0,0);
            }        

            else if( g_displayState == DISP_STAT_TEMP )                
            {
              temp = (unsigned long)g_tempValue;
              Display_Else_Icon(DISP_STAT_TEMP,temp,0);
            }        
            
            else if( g_displayState == DISP_STAT_TIMEF )          
            {
              if( g_timeOfFlight_ave < 0 )
              {
                g_timeOfFlight_ave *= -1;
                Display_unit_Icon(1);
                positiveFlag = 0;
                
              }else{
                Display_unit_Icon(0);
                positiveFlag = 1;
              }
              temp = (unsigned long)g_timeOfFlight_ave;          
              Display_Else_Icon(DISP_STAT_TIMEF,temp,0);
            }

            else if( g_displayState == DISP_STAT_TIMEU )          
            {
              if( g_averageTimeResultUp < 0 )
                g_averageTimeResultUp *= -1;
              temp = (unsigned long)(g_averageTimeResultUp*1000000);            
              if( 0 == g_averageTimeResultDown )
               Display_Else_Icon(DISP_STAT_TIMEU,0,0);  
              else
               Display_Else_Icon(DISP_STAT_TIMEU,temp,0);
            }
            
            else if( g_displayState == DISP_STAT_TIMED )          
            {
              if( g_averageTimeResultDown < 0 )
                g_averageTimeResultDown *= -1;
              temp = (unsigned long)(g_averageTimeResultDown*1000000);            
              if( 0 == g_averageTimeResultUp )
               Display_Else_Icon(DISP_STAT_TIMED,0,0);  
              else
               Display_Else_Icon(DISP_STAT_TIMED,temp,0);
            }    
       
            else if( g_displayState == DISP_STAT_IR )          
            {
              Display_Else_Icon(DISP_STAT_IR,0,0);
            }  
   
            Print_Information();
        }  
     }
     else
     {
       Display_Alarm_Icon(1);
       //initConfigureRegisterTDCGP21();
       
       //Print_TimeOut();
     } 
}


void ultrasonicWaterMeterParamInit(void)
{
	memset(&WFMCountInfo,0,sizeof(WFMCOUNTINFO));

	WfmInfo.Interval	= 2;	//��ʱ2s
	WfmInfo.State 	= WFM_STATE_IDLE;
	WfmInfo.Normal.InstantFlowRate = 0;
	WfmInfo.Normal.InstantFrequence = 0;
	WfmInfo.WaterFlow_Direction = WATERFLOW_NONE;
	WfmInfo.WaterFlow_Warn = WATERFLOW_WARN_NONE;
	WfmInfo.TotalNormalCount = 0;
	WfmInfo.TotalReverseCount = 0;

	WfmInfo.Config.m_coeff_count = 8;
	WfmInfo.Config.m_freq[0] = 8840;// 50
	WfmInfo.Config.m_freq[1] = 14700;// 100
	WfmInfo.Config.m_freq[2] = 30000;// 200
	WfmInfo.Config.m_freq[3] = 61200;// 400
	WfmInfo.Config.m_freq[4] = 75000; // 500
	WfmInfo.Config.m_freq[5] = 147500;// 1000
	WfmInfo.Config.m_freq[6] = 292000;// 2000
	WfmInfo.Config.m_freq[7] = 575000;// 4000

	WfmInfo.Config.m_velocity[0] = 961446;//
	WfmInfo.Config.m_velocity[1] = 975991;//
	WfmInfo.Config.m_velocity[2] = 1002589;//
	WfmInfo.Config.m_velocity[3] = 1038282;//
	WfmInfo.Config.m_velocity[4] = 1045908; //
	WfmInfo.Config.m_velocity[5] = 1054406;//
	WfmInfo.Config.m_velocity[6] = 1067315;//
	WfmInfo.Config.m_velocity[7] = 1082868;//

}


/***************************************************************************
	AmrTimerIntProcess

	description	: Amr timer interrupt Process
	argument	:
	return: 	:
***************************************************************************/
void AmrTimerIntProcess(void)
{
	double countgapN=0, countgapR=0;
	static unsigned char NormalExpired;

	// Normal
	if( g_waterSurfaceSpeed >= 0 )
	{
		countgapN = g_waterSurfaceSpeed*WfmInfo.Interval;
	}
	WfmInfo.TotalNormalCount += countgapN;
	WFMCountInfo.Normal_SumCnt	+= countgapN;

	if (countgapN != 0)
	{
		NormalExpired = 0;
	}
	WFMCountInfo.NormalDuration++;

	//Reverse
	if( g_waterSurfaceSpeed < 0 )
	{
		countgapR = (-1)*g_waterSurfaceSpeed*WfmInfo.Interval;
	}
	WfmInfo.TotalReverseCount += countgapR;
	WFMCountInfo.Reverse_SumCnt	+= countgapR;

	if ( WFMCountInfo.Normal_SumCnt >= AMRC_COUNTTHRESHOLD )
	{	//calculate freq
		if(WFMCountInfo.Normal_SumCnt >= WFMCountInfo.Reverse_SumCnt)
		{
			WFMCountInfo.Normal_SumCnt -= WFMCountInfo.Reverse_SumCnt;

			WfmInfo.WaterFlow_Direction = WATERFLOW_NORMAL;
			WfmInfo.Normal.InstantInterval = WFMCountInfo.NormalDuration;
			WfmInfo.Normal.InstantFrequence = WFMCountInfo.Normal_SumCnt;

			//calculate volume
			if( g_timeOfFlight_ave < 0 )
			{
				g_timeOfFlight_ave = (-1)*g_timeOfFlight_ave;
			}
			CalcVolume(WfmInfo.Normal.InstantFrequence,(unsigned long)g_timeOfFlight_ave);

			//Event_SetEvent(EVENT_FLOW,WfmInfo.WaterFlow_Direction,WfmInfo.WaterFlow_Warn);
		}
		// reset
		WFMCountInfo.NormalDuration = 0;
		WFMCountInfo.Normal_SumCnt = 0;
		WFMCountInfo.Reverse_SumCnt = 0;
	}
	else if (WFMCountInfo.NormalDuration >= WATERFLOW_MAXEXPIRED)
	{
		if (WFMCountInfo.Normal_SumCnt >= AMRC_COUNTTHRESHOLDCLOSE)
		{
			//calculate freq
			if(WFMCountInfo.Normal_SumCnt >= WFMCountInfo.Reverse_SumCnt)
			{
				WFMCountInfo.Normal_SumCnt -= WFMCountInfo.Reverse_SumCnt;

				WfmInfo.WaterFlow_Direction = WATERFLOW_NORMAL;
				WfmInfo.Normal.InstantFrequence = WFMCountInfo.Normal_SumCnt;
				WfmInfo.Normal.InstantInterval = WFMCountInfo.NormalDuration;

				//calculate volume
				if( g_timeOfFlight_ave < 0 )
				{
					g_timeOfFlight_ave = (-1)*g_timeOfFlight_ave;
				}
				CalcVolume(WfmInfo.Normal.InstantFrequence,(unsigned long)g_timeOfFlight_ave);

				//Event_SetEvent(EVENT_FLOW,WfmInfo.WaterFlow_Direction,WfmInfo.WaterFlow_Warn);
			}
		}
		else
		{
			//calculate freq
			if(WFMCountInfo.Normal_SumCnt >= WFMCountInfo.Reverse_SumCnt)
			{
				WFMCountInfo.Normal_SumCnt -= WFMCountInfo.Reverse_SumCnt;

				WfmInfo.WaterFlow_Direction = WATERFLOW_NORMAL;
				WfmInfo.Normal.InstantFrequence = WFMCountInfo.Normal_SumCnt;
				WfmInfo.Normal.InstantInterval = WFMCountInfo.NormalDuration;

				//calculate volume
				if( g_timeOfFlight_ave < 0 )
				{
					g_timeOfFlight_ave = (-1)*g_timeOfFlight_ave;
				}
				CalcVolume(WfmInfo.Normal.InstantFrequence,(unsigned long)g_timeOfFlight_ave);

				//Event_SetEvent(EVENT_FLOW,WfmInfo.WaterFlow_Direction,WfmInfo.WaterFlow_Warn);
			}                  

			NormalExpired = 1;
		}
		// reset
		WFMCountInfo.NormalDuration = 0;
		WFMCountInfo.Normal_SumCnt = 0;
		WFMCountInfo.Reverse_SumCnt = 0;
	}

	if (NormalExpired)
	{
                NormalExpired = 0;
                WfmInfo.State = WFM_STATE_IDLE;
		WfmInfo.WaterFlow_Direction = WATERFLOW_NONE;
		WfmInfo.Normal.InstantFrequence = 0;
		WfmInfo.Normal.InstantInterval = 0;

		StopTimerA0();
		g_valueOpenFlag = 0; //���Źر�
                g_waterSurfaceSpeed = 0;
                g_waterSurfaceSpeedDisp = 0;
                Display_WaterRunning_Icon(0);
		//Event_SetEvent(EVENT_FLOW,WfmInfo.WaterFlow_Direction,WfmInfo.WaterFlow_Warn);

	}

}


void CalcVolume(double frequence,unsigned long tof_ave)
{
	int i;
	unsigned long freq;
	unsigned long coeff;
	unsigned long coeff1;
	unsigned long vol  = 0;
	unsigned long vol1 = 0;
	unsigned long f;
	unsigned long frequence1;

	f = tof_ave;
	for(i=0;i<WfmInfo.Config.m_coeff_count;i++)
	{
		if(f < WfmInfo.Config.m_freq[i])
		{
			break;
		}
	}

	if(i>=WfmInfo.Config.m_coeff_count)
	{
		i = WfmInfo.Config.m_coeff_count - 1;
	}
	else if(i==0)
	{
		i = 1;
	}

	freq = WfmInfo.Config.m_freq[i] - WfmInfo.Config.m_freq[i-1];
	if(freq==0){
		coeff = WfmInfo.Config.m_velocity[i];
	}
	else{
		coeff = WfmInfo.Config.m_velocity[i] - WfmInfo.Config.m_velocity[i-1];
		coeff1 = (coeff*(f-WfmInfo.Config.m_freq[i-1]))/freq + WfmInfo.Config.m_velocity[i-1];
	}

	frequence1 = (unsigned long)(frequence*coeff1/1000000UL);
        //g_waterSurfaceSpeedDisp = (double)(g_waterSurfaceSpeed*coeff1/1000000UL);

	// WfmInfo.Normal.TotalVolume��λ��0.1m3��WfmInfo.Normal.TotalVolumeDecimal��λ��1mL
	vol = (unsigned long)frequence1+WfmInfo.Normal.TotalVolumeDecimal;
        //vol = (unsigned long)frequence+WfmInfo.Normal.TotalVolumeDecimal;
	vol1 = vol/100UL;
	WfmInfo.Normal.TotalVolumeDecimal = vol-vol1*100UL;
	WfmInfo.Normal.TotalVolume += vol1;
        if(vol1>0)
        {
          //�ۻ�����������
          //Display_Cum_Icon(WfmInfo.Normal.TotalVolume); 
          
          //Save_WaterCumData();
        }

	if(WfmInfo.Normal.TotalVolume >= MAX_TOTALVOLUME)
		WfmInfo.Normal.TotalVolume -= MAX_TOTALVOLUME;      
        
}



void DelayNS(unsigned long dly)
{
	unsigned int  i;
		for(i=0;i<dly;i++);
} 

#pragma vector=PORT2_VECTOR
__interrupt void PORT2 (void)
{
  if(P2IFG & BIT1)//TDCGP21 interrupt pin
  {
    g_tdcInterruptFlag = 1;
    P2IFG = 0;  //�ֶ����жϱ�־λ
    return;
  }
  P2IFG = 0;  //�ֶ����жϱ�־λ

}



//�������˲�
double q = 0.1;
double r = 2;

//״̬��ֵx�� ����������ֵw������p
static double x = 0;

static double w = 0;

static double p = 0;



long kalman_filter( long z){
  
  // Ԥ��
  double x_ = x + 0.003 + w;
  
  double p_ = p + q;
  
  double k = p_ / (p_ + r + 0.001);
  
  // ����
  x = x_ + k * (z - x_);
  
  p = (1-k) * p_;
  
  return (long)x;
    
}



//�������˲�
double qup = 0.1;
double rup = 2;

//״̬��ֵx�� ����������ֵw������p
static double xup = 0;

static double wup = 0;

static double pup = 0;



double kalman_filterup( double zup){
  
  // Ԥ��
  double xup_ = xup + 0 + wup;
  
  double pup_ = pup + qup;
  
  double kup = pup_ / (pup_ + rup + 0.001);
  
  // ����
  xup = xup_ + kup * (zup - xup_);
  
  pup = (1-kup) * pup_;
  
  return xup;
    
}


//�������˲�
double qdown = 0.1;
double rdown = 2;
//״̬��ֵx�� ����������ֵw������p
static double xdown = 0;

static double wdown = 0;

static double pdown = 0;



double kalman_filterdown( double zdown){
  
  // Ԥ��
  double xdown_ = xdown + 0 + wdown;
  
  double pdown_ = pdown + qdown;
  
  double kdown = pdown_ / (pdown_ + rdown + 0.001);
  
  // ����
  xdown = xdown_ + kdown * (zdown - xdown_);
  
  pdown = (1-kdown) * pdown_;
  
  return xdown;
    
}


































