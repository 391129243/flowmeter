#include "msp430x54x.h"
#include "functions.h"
#include "flash.h"

/*******************************************
*M25P16-VME6TG 相关函数
*相关函数应在主函数体中调用,不建议用于中断中
*作者:吴挺
*******************************************/
//flash初始化
void Init_Flash(void)
{
  P10SEL |= (BIT1 + BIT2 + BIT3);  //SPI引脚启用
  P10DIR  |= (BIT1 + BIT3);  //MO,CLK设为输出 
  P10DIR &= ~BIT2; //MI为输入
  P10DIR  |= BIT0;  //CS 
  
  UCB3CTL1 |= UCSWRST; 
  UCB3CTL0 |= UCMST+UCSYNC+UCCKPL+UCMSB;    //主机，同步，空闲状态下是高电平，搞位首先发送
  UCB3CTL1 |= UCSSEL_2;                     // SMCLK
  UCB3BR0 = 0x02;                           // /2
  UCB3BR1 = 0x00;                              
  UCB3CTL1 &= ~UCSWRST;                     // 从复位模式下运行
  UCB3IFG &= ~UCTXIFG;  
  //UCB3IE = UCRXIE;
  
  P10DIR |= BIT0;    //FLASH CS
  FLASH_CS_1;   // not choose flash chip
  
  P11DIR |= BIT0;    //FLASH Power
  FLASH_POWER_OFF;   
}

//SPI 接收一个字节
char SPI_Flash_ReadByte(void)
{
  while(!(UCB3IFG & UCRXIFG));
  //UCB3IFG &= ~UCRXIFG; //清接收完成标志位(当读取接收的数据后，接收完成标志位自动复位)
  return UCB3RXBUF;
}

//SPI 写一个字节
void SPI_Flash_WriteByte(unsigned char ch)
{
  UCB3TXBUF = ch;
  while (!(UCB3IFG & UCTXIFG));
  //UCB3IFG &= ~UCTXIFG; //清发送完成标志位(当执行指令“U0TXBUF = ch;”后，发送完成标志位自动复位)
}

/*char SPI_Flash_WriteByte(unsigned char ch)
{
  SPI_Flash_WriteByte_0(ch);    //430的SPI主模块都会在发送后半个时钟周期读取采样的0/1信号，存入接收缓冲寄存器，
                               //所以，每次的写入，均有数据读取，但不一定是从设备发送回来的，这个地方在
                               //使用430主机模式的时候必须注意，很容易出错
                               //这里每发送一字节，都去接收一个字节，用以清除接收完成标志
  return SPI_Flash_ReadByte();
}*/


//SPI 写使能
void SPI_Flash_Write_Enable(void)
{
  FLASH_CS_0;
  _NOP();
  SPI_Flash_WriteByte(FLASH_WRITE_ENABLE);
  _NOP();
  FLASH_CS_1;
}


//等待flash写完成
void SPI_Flash_Wait_Busy(void)
{
  unsigned char ch = 0xff;
  FLASH_CS_0;
  while(ch & 0x01)//等待写完成WIP=0
  {
    SPI_Flash_WriteByte(READ_STATE_REGISTER);
    SPI_Flash_WriteByte(0x00); //只是为了给SPI读提供时钟用
    ch = SPI_Flash_ReadByte();
  }
  FLASH_CS_1;
}


/*******************************************************
*从指定地址起读取读取指定字节的数据到缓冲区
*
*******************************************************/
void SPI_Flash_Read_Buffer(unsigned long startaddr,unsigned char * databuf,unsigned int buflen)
{
  FLASH_CS_0;
  SPI_Flash_WriteByte(FLASH_FAST_READ);
  unsigned char ch = (startaddr>>16) & 0xff;
  SPI_Flash_WriteByte(ch);//地址高字节   
  ch = (startaddr>>8) & 0xff;
  SPI_Flash_WriteByte(ch);//地址中字节  
  ch = startaddr & 0xff;
  SPI_Flash_WriteByte(ch);//地址低字节
  SPI_Flash_WriteByte(0x00); //Dummy Byte
  //读数据
  for(unsigned int i=0; i < buflen; i++)
  {
      SPI_Flash_WriteByte(0x00); //只是为了给SPI读提供时钟用
      databuf[i] = SPI_Flash_ReadByte();
  }
  FLASH_CS_1;
}

/*******************************************************
写数据缓冲区
startaddr:起始地址
databuf:数据存放缓冲
buflen:数据长度
*******************************************************/
void SPI_Flash_Write_Buffer(unsigned long startaddr,unsigned char * databuf,unsigned int buflen)
{
  while(buflen)
  {
    SPI_Flash_Write_Enable();
    FLASH_CS_0;
    SPI_Flash_WriteByte(FLASH_PAGE_PROG);
    unsigned char ch = (startaddr>>16) & 0xff;
    SPI_Flash_WriteByte(ch);//地址高字节   
    ch = (startaddr>>8) & 0xff;
    SPI_Flash_WriteByte(ch);//地址中字节  
    ch = startaddr & 0xff;
    SPI_Flash_WriteByte(ch);//地址低字节
    //写一字节
    SPI_Flash_WriteByte(*databuf);
    databuf++;
    buflen--;
    startaddr++;
    while(buflen && ((startaddr & 0x0ff) != 0))//未换页
    {
      SPI_Flash_WriteByte(*databuf);
      databuf++;
      buflen--;
      startaddr++;
    }
    FLASH_CS_1;
    SPI_Flash_Wait_Busy();
  }
}


/******************************************************
*擦除指定的扇区
*
******************************************************/
void SPI_Flash_Erase_Sector(unsigned long addr)
{
    SPI_Flash_Write_Enable();
    FLASH_CS_0;
    SPI_Flash_WriteByte(FLASH_SECTOR_ERASE);
    unsigned char ch = (addr>>16) & 0xff;
    SPI_Flash_WriteByte(ch);//地址高字节   
    ch = (addr>>8) & 0xff;
    SPI_Flash_WriteByte(ch);//地址中字节  
    ch = addr & 0xff;
    SPI_Flash_WriteByte(ch);//地址低字节
    FLASH_CS_1;
    SPI_Flash_Wait_Busy();
}


/******************************************************
*擦除整片flash
*
******************************************************/
void SPI_Flash_Erase_Bulk(void)
{
    SPI_Flash_Write_Enable();
    FLASH_CS_0;
    SPI_Flash_WriteByte(FLASH_BULK_ERASE);
    FLASH_CS_1;
    SPI_Flash_Wait_Busy();
}

/******************************************************
*进入低功耗模式
*
******************************************************/
void SPI_Flash_Deep_PowerDown(void)
{
    SPI_Flash_Write_Enable();
    FLASH_CS_0;
    SPI_Flash_WriteByte(FLASH_Deep_PowerDown);
    FLASH_CS_1;
}

/******************************************************
*退出低功耗模式
*
******************************************************/
void SPI_Flash_Release_DP(void)
{
    FLASH_CS_0;
    SPI_Flash_WriteByte(FLASH_Release_DP);
    FLASH_CS_1;
    _NOP();
    _NOP();
    _NOP();
}

/******************************************************
*断开电源
*
******************************************************/
void SPI_Flash_PowerOff(void)
{
    FLASH_POWER_OFF;
}

/******************************************************
*打开电源
*
******************************************************/
void SPI_Flash_PowerOn(void)
{
    FLASH_POWER_ON;
    delay_ms(1);
}



