#include "msp430x54x.h"
#include "functions.h"
#include "flash.h"

/*******************************************
*M25P16-VME6TG ��غ���
*��غ���Ӧ�����������е���,�����������ж���
*����:��ͦ
*******************************************/
//flash��ʼ��
void Init_Flash(void)
{
  P10SEL |= (BIT1 + BIT2 + BIT3);  //SPI��������
  P10DIR  |= (BIT1 + BIT3);  //MO,CLK��Ϊ��� 
  P10DIR &= ~BIT2; //MIΪ����
  P10DIR  |= BIT0;  //CS 
  
  UCB3CTL1 |= UCSWRST; 
  UCB3CTL0 |= UCMST+UCSYNC+UCCKPL+UCMSB;    //������ͬ��������״̬���Ǹߵ�ƽ����λ���ȷ���
  UCB3CTL1 |= UCSSEL_2;                     // SMCLK
  UCB3BR0 = 0x02;                           // /2
  UCB3BR1 = 0x00;                              
  UCB3CTL1 &= ~UCSWRST;                     // �Ӹ�λģʽ������
  UCB3IFG &= ~UCTXIFG;  
  //UCB3IE = UCRXIE;
  
  P10DIR |= BIT0;    //FLASH CS
  FLASH_CS_1;   // not choose flash chip
  
  P11DIR |= BIT0;    //FLASH Power
  FLASH_POWER_OFF;   
}

//SPI ����һ���ֽ�
char SPI_Flash_ReadByte(void)
{
  while(!(UCB3IFG & UCRXIFG));
  //UCB3IFG &= ~UCRXIFG; //�������ɱ�־λ(����ȡ���յ����ݺ󣬽�����ɱ�־λ�Զ���λ)
  return UCB3RXBUF;
}

//SPI дһ���ֽ�
void SPI_Flash_WriteByte(unsigned char ch)
{
  UCB3TXBUF = ch;
  while (!(UCB3IFG & UCTXIFG));
  //UCB3IFG &= ~UCTXIFG; //�巢����ɱ�־λ(��ִ��ָ�U0TXBUF = ch;���󣬷�����ɱ�־λ�Զ���λ)
}

/*char SPI_Flash_WriteByte(unsigned char ch)
{
  SPI_Flash_WriteByte_0(ch);    //430��SPI��ģ�鶼���ڷ��ͺ���ʱ�����ڶ�ȡ������0/1�źţ�������ջ���Ĵ�����
                               //���ԣ�ÿ�ε�д�룬�������ݶ�ȡ������һ���Ǵ��豸���ͻ����ģ�����ط���
                               //ʹ��430����ģʽ��ʱ�����ע�⣬�����׳���
                               //����ÿ����һ�ֽڣ���ȥ����һ���ֽڣ��������������ɱ�־
  return SPI_Flash_ReadByte();
}*/


//SPI дʹ��
void SPI_Flash_Write_Enable(void)
{
  FLASH_CS_0;
  _NOP();
  SPI_Flash_WriteByte(FLASH_WRITE_ENABLE);
  _NOP();
  FLASH_CS_1;
}


//�ȴ�flashд���
void SPI_Flash_Wait_Busy(void)
{
  unsigned char ch = 0xff;
  FLASH_CS_0;
  while(ch & 0x01)//�ȴ�д���WIP=0
  {
    SPI_Flash_WriteByte(READ_STATE_REGISTER);
    SPI_Flash_WriteByte(0x00); //ֻ��Ϊ�˸�SPI���ṩʱ����
    ch = SPI_Flash_ReadByte();
  }
  FLASH_CS_1;
}


/*******************************************************
*��ָ����ַ���ȡ��ȡָ���ֽڵ����ݵ�������
*
*******************************************************/
void SPI_Flash_Read_Buffer(unsigned long startaddr,unsigned char * databuf,unsigned int buflen)
{
  FLASH_CS_0;
  SPI_Flash_WriteByte(FLASH_FAST_READ);
  unsigned char ch = (startaddr>>16) & 0xff;
  SPI_Flash_WriteByte(ch);//��ַ���ֽ�   
  ch = (startaddr>>8) & 0xff;
  SPI_Flash_WriteByte(ch);//��ַ���ֽ�  
  ch = startaddr & 0xff;
  SPI_Flash_WriteByte(ch);//��ַ���ֽ�
  SPI_Flash_WriteByte(0x00); //Dummy Byte
  //������
  for(unsigned int i=0; i < buflen; i++)
  {
      SPI_Flash_WriteByte(0x00); //ֻ��Ϊ�˸�SPI���ṩʱ����
      databuf[i] = SPI_Flash_ReadByte();
  }
  FLASH_CS_1;
}

/*******************************************************
д���ݻ�����
startaddr:��ʼ��ַ
databuf:���ݴ�Ż���
buflen:���ݳ���
*******************************************************/
void SPI_Flash_Write_Buffer(unsigned long startaddr,unsigned char * databuf,unsigned int buflen)
{
  while(buflen)
  {
    SPI_Flash_Write_Enable();
    FLASH_CS_0;
    SPI_Flash_WriteByte(FLASH_PAGE_PROG);
    unsigned char ch = (startaddr>>16) & 0xff;
    SPI_Flash_WriteByte(ch);//��ַ���ֽ�   
    ch = (startaddr>>8) & 0xff;
    SPI_Flash_WriteByte(ch);//��ַ���ֽ�  
    ch = startaddr & 0xff;
    SPI_Flash_WriteByte(ch);//��ַ���ֽ�
    //дһ�ֽ�
    SPI_Flash_WriteByte(*databuf);
    databuf++;
    buflen--;
    startaddr++;
    while(buflen && ((startaddr & 0x0ff) != 0))//δ��ҳ
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
*����ָ��������
*
******************************************************/
void SPI_Flash_Erase_Sector(unsigned long addr)
{
    SPI_Flash_Write_Enable();
    FLASH_CS_0;
    SPI_Flash_WriteByte(FLASH_SECTOR_ERASE);
    unsigned char ch = (addr>>16) & 0xff;
    SPI_Flash_WriteByte(ch);//��ַ���ֽ�   
    ch = (addr>>8) & 0xff;
    SPI_Flash_WriteByte(ch);//��ַ���ֽ�  
    ch = addr & 0xff;
    SPI_Flash_WriteByte(ch);//��ַ���ֽ�
    FLASH_CS_1;
    SPI_Flash_Wait_Busy();
}


/******************************************************
*������Ƭflash
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
*����͹���ģʽ
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
*�˳��͹���ģʽ
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
*�Ͽ���Դ
*
******************************************************/
void SPI_Flash_PowerOff(void)
{
    FLASH_POWER_OFF;
}

/******************************************************
*�򿪵�Դ
*
******************************************************/
void SPI_Flash_PowerOn(void)
{
    FLASH_POWER_ON;
    delay_ms(1);
}



