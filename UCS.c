#include "msp430x54x.h"
#include "UCS.h" 


//��ʼ��UCS
void initUCS(void)
{
  //����
  P7SEL |= BIT0 + BIT1;  //���� XT1-32.768KHz
  
  //��ʼ��ϵͳʱ��
  UCSCTL6 &= ~(XT1OFF + XT1BYPASS);                 //����XT1ѡ���ⲿ�����ṩ,��������XT1
  
  
  UCSCTL1 = DCORSEL_6;                             // ѡ��FLL�ο�Դ��REFO
  //UCSCTL1 = DCORSEL_3;
  UCSCTL2 |= 0x79;                                 // Set DCO Multiplier for 8MHz; fdcoclk=D*(N+1)*(fFLLREFCLK/n);
  //UCSCTL2 |= 0x15;                                 // Set DCO Multiplier for 1.048576MHz; fdcoclk=D*(N+1)*(fFLLREFCLK/n);
 
  _EINT(); // �����ж� 
  _BIS_SR(GIE);  
 
  //�������ʧЧ��־
   while((SFRIFG1 &OFIFG))
  {    
      UCSCTL7 &= ~(XT1LFOFFG + DCOFFG); //�������XT1��ʧЧ��־
      SFRIFG1 &= ~OFIFG; //�������ʧЧ��־
      //����������Ҫ�У�����������Ҳ�������ɹ�
  }
  
  UCSCTL4 = SELM__DCOCLK + SELS__DCOCLK + SELA__XT1CLK; //ʱ��ѡ�� MCLK = DCOC,SMCLK =DCOC,ACLK=XT1
  
  _DINT(); // �ر����ж� 
}
