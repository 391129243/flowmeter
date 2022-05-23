#ifndef FLASH_H
#define FLASH_H
/***********************���ݴ洢����**************************
---------------------ϵͳ���� ��0��--------------------------
*0x000000--0x00ffff    ϵͳ����(64KB)

-----------------ˮ������ۼ��� ��1��------------------------
*0x010000--0x01FFFF    �ۼ���(64KB)

------------------�ն�����¶����ۻ��� ��2��------------------------
*0x020000--0x02FFFF    ��1�����ۻ���(64KB)






**********************************************/


#define System_Params_ADDR                   0x000000 /// ϵͳ����
#define WaterCum_Data_ADDR                   0x000000 /// �ۻ���
#define MonthDayCumData__ADDR                0x020000 /// �ն�����¶���


#define READ_STATE_REGISTER    0x05	// ��ȡ״̬�Ĵ���
#define FLASH_FAST_READ        0x0B        //flash���ٶ�
#define FLASH_PAGE_PROG        0x02        //flashд
#define FLASH_WRITE_ENABLE     0x06        //FLASH����дָ��
#define FLASH_SECTOR_ERASE     0xD8        //������
#define FLASH_BULK_ERASE       0xC7        //��Ƭ����
#define FLASH_Deep_PowerDown   0xB9        //����͹���
#define FLASH_Release_DP       0xAB        //�˳��͹���

#define FLASH_CS_1 P10OUT |= BIT0                   //CS�øߵ�ƽ
#define FLASH_CS_0 P10OUT &= ~BIT0                  //CS�õ͵�ƽ

#define FLASH_POWER_OFF  P11OUT |= BIT0                   //�رյ�Դ
#define FLASH_POWER_ON   P11OUT &= ~BIT0                  //�򿪵�Դ


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