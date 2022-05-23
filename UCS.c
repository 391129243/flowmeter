#include "msp430x54x.h"
#include "UCS.h" 


//初始化UCS
void initUCS(void)
{
  //晶振
  P7SEL |= BIT0 + BIT1;  //启用 XT1-32.768KHz
  
  //初始化系统时钟
  UCSCTL6 &= ~(XT1OFF + XT1BYPASS);                 //晶振XT1选择外部晶振提供,开启晶振XT1
  
  
  UCSCTL1 = DCORSEL_6;                             // 选择FLL参考源自REFO
  //UCSCTL1 = DCORSEL_3;
  UCSCTL2 |= 0x79;                                 // Set DCO Multiplier for 8MHz; fdcoclk=D*(N+1)*(fFLLREFCLK/n);
  //UCSCTL2 |= 0x15;                                 // Set DCO Multiplier for 1.048576MHz; fdcoclk=D*(N+1)*(fFLLREFCLK/n);
 
  _EINT(); // 开总中断 
  _BIS_SR(GIE);  
 
  //清除振荡器失效标志
   while((SFRIFG1 &OFIFG))
  {    
      UCSCTL7 &= ~(XT1LFOFFG + DCOFFG); //清除振荡器XT1的失效标志
      SFRIFG1 &= ~OFIFG; //清除振荡器失效标志
      //上面语句必须要有，否则起振了也不会分配成功
  }
  
  UCSCTL4 = SELM__DCOCLK + SELS__DCOCLK + SELA__XT1CLK; //时钟选择 MCLK = DCOC,SMCLK =DCOC,ACLK=XT1
  
  _DINT(); // 关闭总中断 
}
