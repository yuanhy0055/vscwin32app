#include <msp430.h>
#include <stdbool.h>

#include "detector.h"
#include "time.h"

// *****************************************************************************
// *****************************************************************************
// Section: Data
// *****************************************************************************
// *****************************************************************************
bool F_Second = false;
bool F_Acq = false;
bool IsOneSecondUp = false;
bool Flashing = false;
bool F_10mS = false;
bool FF = true;

extern unsigned char Kaiji_flag;
//extern unsigned char DT_Connection_CNT;


// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************
void init_TimerA(void)
{
  TACCR0 = 10000;        //2.5mS
  TACTL = TASSEL_2 + ID_1 + MC_1 + TACLR;    //SMCLK =8MHz, 1:2
  TACCTL0 = CCIE;

  return;
}

// *****************************************************************************
// Timer_A interrupt handler
// *****************************************************************************
#pragma vector = TIMERA0_VECTOR
__interrupt void TimerA_ISR(void)
{
  static unsigned short count1 = 0;
  static unsigned short count2 = 0;
  static unsigned short count3 = 0;
  static unsigned short count4 = 0;
  
  F_10mS = !F_10mS;

  if( !(count1--) )
  {    
       count1 = 100;  //0.25s  

       F_Second = !F_Second;
       //F_Acq = true;
      // if( DT_Connection_CNT )
       //    DT_Connection_CNT--;
  }

  if( !(count2--) )
  {
       count2 = 400;   //1s秒发一次数
       F_Acq = true;
      // Flashing = !Flashing;
  }
  
  if( !(count3--) )
  {
      count3 = 200;
      Flashing = !Flashing;   //0.5秒
      //ADC12CTL0 |= ADC12SC; // 开AD转换
  }
  
  if( !(count4--) )
  {
      count4 = 2000;//5s
      //FF = !FF;   //秒
      //FF = true;   //秒
     // FF = false;   //秒
      //Kaiji_flag = 1;
      ADC12CTL0 |= ADC12SC; // 开AD转换
  }
  
  
  
  
  
}