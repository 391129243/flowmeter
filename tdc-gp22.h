#ifndef TDCGP22_H
#define TDCGP22_H

/* --- definition --- */
#define	DUMMY_DATA		            0xFF

#define POWER_ON_RESET                      0x50                   /// Power on reset
#define INIT_MEASURE                        0x70                   /// Init measure

#define WRITE_REG0                          0x80                   /// Write register0 operation address
#define WRITE_REG1                          0x81                   /// Write register1 operation address
#define WRITE_REG2                          0x82                   /// Write register2 operation address
#define WRITE_REG3                          0x83                   /// Write register3 operation address
#define WRITE_REG4                          0x84                   /// Write register4 operation address
#define WRITE_REG5                          0x85                   /// Write register5 operation address
#define WRITE_REG6                          0x86                   /// Write register6 operation address
#define READ_RES0                           0xB0                   /// Read 32bit result register0 address
#define READ_RES1                           0xB1                   /// Read 32bit result register1 address
#define READ_RES2                           0xB2                   /// Read 32bit result register2 address
#define READ_RES3                           0xB3                   /// Read 32bit result register3 address
#define READ_STAT                           0xB4                   /// Read 16bit state register address
#define READ_REG5                           0xB5                   /// Read 8bit what write to reg1 data, use to test communication
//#define READ_REG6                           0xB6                   /// Read register6 operation address
#define READ_IDBIT                          0xB7                   /// Read TDC ID bit(56 bits)
#define READ_PW1ST                          0xB8                   /// Read TDC PW1ST(8 bits)

#define REG_TO_EEPROM                       0xC0                   /// Write configure register to EEPROM
#define EEPROM_TO_REG                       0xF0                   /// Read configure register to EEPROM
#define EEPROM_REG_COMPARE                  0xC5                   /// Compare configure register with EEPROM

#define START_TOF                           0x01                   /// Start TOF measure
#define START_TEMP                          0x02                   /// Start temperature measure
#define START_CAL_OSC                       0x03                   /// Calibrate oscillator
#define START_CAL_TDC                       0x04                   /// Calibrate TDC
#define START_TOF_RESTART                   0x05                   /// Restart TOF measure
#define START_TEMP_RESTART                  0x06                   /// Restart temperature measure

#define TDC_CS_1                            P9OUT |= BIT4         /// TDC chip cs control signal.
#define TDC_CS_0                            P9OUT &= ~BIT4
#define TDC_START_1                         P9OUT |= BIT7         /// TDC chip start control signal.
#define TDC_START_0                         P9OUT &= ~BIT7
#define TDC_EN_START_1                      P9OUT |= BIT6         /// TDC chip en_start control signal.
#define TDC_EN_START_0                      P9OUT &= ~BIT6
#define TDC_FIRE_IN_1                       P9OUT |= BIT5         /// TDC chip fire_in control signal.
#define TDC_FIRE_IN_0                       P9OUT &= ~BIT5
#define TDC_RESET_1                         P9OUT |= BIT0         /// TDC chip reset control signal.
#define TDC_RESET_0                         P9OUT &= ~BIT0
#define TDC_EN_STOP2_1                      P8OUT |= BIT7         /// TDC chip en_stop2 control signal.
#define TDC_EN_STOP2_0                      P8OUT &= ~BIT7
#define TDC_EN_STOP1_1                      P8OUT |= BIT6          /// TDC chip en_stop1 control signal.
#define TDC_EN_STOP1_0                      P8OUT &= ~BIT6
#define TDC_Channel_Switch2_1               P8OUT |= BIT5         /// TDC chip Channel_Switch2 control signal.
#define TDC_Channel_Switch2_0               P8OUT &= ~BIT5
#define TDC_Channel_Switch1_1               P8OUT |= BIT4         /// TDC chip Channel_Switch1 control signal.
#define TDC_Channel_Switch1_0               P8OUT &= ~BIT4

#define SoundVelocity                       1450



/// 如何得到500ps阈值：以线速度阈值为10mm/s=0.01m/s，TOF = 0.06/(1450-0.01)-0.06/(1450+0.01) = 572ps,取500ps;
#define TofThresholdVale	50 /// 以飞行时间差大于1000ps判断水龙头是否已开启





#define WATERFLOW_CALC_VOLUME 1

#define AMRC_CNF_NONE 		0
#define AMRC_CNF_FACTORY 	1
#define AMRC_CNF_CALI 		2
#define AMRC_CNF_START 		3
#define AMRC_CNF_ERR 		4

#define AMRC_CW					0
#define AMRC_CCW				1

#define AMRC_EVENT_TRG_NORMAL	0
#define AMRC_EVENT_TRG_REVERSE	1
#define AMRC_COUNT_SRC_REVERSE	1
#define AMRC_COUNT_SRC_STOP		0

#define AMRC_TRGCYC_1HZ			0
#define AMRC_TRGCYC_2HZ			1
#define AMRC_TRGCYC_4HZ			2
#define AMRC_TRGCYC_8HZ			3
#define AMRC_TRGCYC_16HZ		4
#define AMRC_TRGCYC_32HZ		5
#define AMRC_TRGCYC_64HZ		6
#define AMRC_TRGCYC_128HZ		7
#define AMRC_TRGCYC_256HZ		8
#define AMRC_TRGCYC_512HZ		9
#define AMRC_TRGCYC_1024HZ		0x0a
#define AMRC_TRGCYC_2048HZ		0x0b
#define AMRC_TRGCYC_4096HZ		0x0c
#define AMRC_TRGCYC_341HZ		0x0d
#define AMRC_TRGCYC_682HZ		0x0e
#define AMRC_TRGCYC_1365HZ		0x0f

#define AMRC_INT_UCNTIF			0x1000
#define AMRC_INT_CNT2IF			0x400
#define AMRC_INT_CNT1IF			0x200
#define AMRC_INT_CNT0IF			0x100
#define AMRC_INT_DIF1IF			0x80
#define AMRC_INT_DIF0IF			0x20
#define AMRC_INT_RSKIPIF		0x8
#define AMRC_INT_STPIF			0x4
#define AMRC_INT_REVRIF			0x2
#define AMRC_INT_NMLRIF			0x1

#define MAX_WAIT_TIME			30
#define MAX_BELOW_TIME			3600	//2

#define WATERFLOW_NONE			0
#define WATERFLOW_NORMAL		1
#define WATERFLOW_REVERSE		2

#define WATERFLOW_WARN_NONE			0
#define WATERFLOW_WARN_UPPERLIMIT	1
#define WATERFLOW_WARN_BELOWLIMIT	2
#define WATERFLOW_WARN_NOISE		0x80

// State
#define	WFM_STATE_STOP				0
#define	WFM_STATE_IDLE				1
#define	WFM_STATE_RUN				2

// Parameter
#define	WATERFLOW_COEFF_COUNT 16
#define	WATERFLOW_COEFF_MAX_COUNT 16

#define WATERFLOW_MAXEXPIRED 14 //28s钟流量未达到56mL

#define MAX_TOTALVOLUME		1000000UL //count by Liter

//Config by user
#define AMRC_TRGCYC_LOWFREQ				AMRC_TRGCYC_16HZ
#define AMRC_TRGCYC_HIGHFREQ			AMRC_TRGCYC_682HZ
#define AMRC_FLOWVOLUMEUNIT				8
#define AMRC_COUNTTHRESHOLD				5 //大于等于56mL进行累加
#define AMRC_COUNTTHRESHOLDCLOSE		8 //28s未达到7ml，判断为管阀(1L/h=0.278ml/s,28*0.25=7.78ml)
#define AMRC_REVERSE_FLOWCOEFF 11000


extern void initSPI(void);
extern unsigned char readAndWriteByteTDCGP21(unsigned char data);
extern void resetTDCGP21(void);
extern void powerOnResetTDCGP21(void);
extern void initMeasureTDCGP21(void);
extern void calibrateResonatorTDCGP21(void);
extern void timeFlightStartTDCGP21(void);
extern void timeFlightRestartTDCGP21(void);
extern void configureRegisterTDCGP21(unsigned char opcode_address,unsigned long config_reg_data);
extern unsigned int readStatusRegisterTDCGP21(void);
extern double readPW1STRegisterTDCGP22(void);
extern unsigned long readRegisterTDCGP21(unsigned char read_opcode_address);
extern void readIDbytesTDCGP21(void);
extern void initConfigureRegisterTDCGP21( void );
extern void calibrateResonator( void );
extern void ultrasonicTimeOfFlightMeasure(void);

extern void ultrasonicWaterMeterParamInit(void);
extern void AmrTimerIntProcess(void);
extern void CalcVolume(double frequence,unsigned long tof_ave);

extern void DelayNS(unsigned long dly);

#endif