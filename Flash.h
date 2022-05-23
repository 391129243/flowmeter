#ifndef FLASH_H
#define FLASH_H
/***********************数据存储划分**************************
---------------------系统参数 第0区--------------------------
*0x000000--0x00ffff    系统参数(64KB)

-----------------水表读数累计量 第1区------------------------
*0x010000--0x01FFFF    累计量(64KB)

------------------日冻结和月冻结累积量 第2区------------------------
*0x020000--0x02FFFF    第1年月累积量(64KB)






**********************************************/


#define System_Params_ADDR                   0x000000 /// 系统参数
#define WaterCum_Data_ADDR                   0x000000 /// 累积量
#define MonthDayCumData__ADDR                0x020000 /// 日冻结和月冻结


#define READ_STATE_REGISTER    0x05	// 读取状态寄存器
#define FLASH_FAST_READ        0x0B        //flash快速读
#define FLASH_PAGE_PROG        0x02        //flash写
#define FLASH_WRITE_ENABLE     0x06        //FLASH允许写指令
#define FLASH_SECTOR_ERASE     0xD8        //擦除区
#define FLASH_BULK_ERASE       0xC7        //整片擦除
#define FLASH_Deep_PowerDown   0xB9        //进入低功耗
#define FLASH_Release_DP       0xAB        //退出低功耗

#define FLASH_CS_1 P10OUT |= BIT0                   //CS置高电平
#define FLASH_CS_0 P10OUT &= ~BIT0                  //CS置低电平

#define FLASH_POWER_OFF  P11OUT |= BIT0                   //关闭电源
#define FLASH_POWER_ON   P11OUT &= ~BIT0                  //打开电源


void Init_Flash(void);
void SPI_Flash_Read_Buffer(unsigned long startaddr,unsigned char * databuf,unsigned int buflen);
void SPI_Flash_Write_Buffer(unsigned long startaddr,unsigned char * databuf,unsigned int buflen);
void SPI_Flash_Erase_Sector(unsigned long addr);
void SPI_Flash_Erase_Bulk(void);
void SPI_Flash_Deep_PowerDown(void);
void SPI_Flash_Release_DP(void);
void SPI_Flash_Write_Enable(void);
void SPI_Flash_Wait_Busy(void);
void SPI_Flash_PowerOff(void);
void SPI_Flash_PowerOn(void);

#endif