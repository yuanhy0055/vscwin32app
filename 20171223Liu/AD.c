#include <msp430.h>
#include"AD.h"
#include "time.h"


unsigned int S;
float results;



void  battery_AD ()
{
    
    P6SEL |= 0x08;     //P6.3电池电压采集
    ADC12CTL0 |= SHT0_2 + REF2_5V + REFON + ADC12ON;   //MSC重复转换 参考电压2.5V
    
    ADC12CTL1 |=  SHP + CONSEQ_0 +SHS_0;  //单通道单次  CSTARTADD0通道选择  默认0通道
    
    //ADC12MCTL0 |= SREF0 + INCH_3 + EOS;
    ADC12MCTL0 |= INCH_3 + SREF0 ;
    ADC12IE |= 0x01;  //0通道允许中断
    ADC12CTL0 |= ENC;
    

    ADC12CTL0 |= ADC12SC;
    //_BIS_SR(LPM0_bits + GIE);
}
 



#pragma vector = ADC12_VECTOR
__interrupt void ADC12_ISR (void)
{
    static unsigned int i=0; 
    static unsigned int AD=0;
  
   // results = ADC12MEM0; //0通道转换结果
        
    i++;
   // S += ADC12MEM0;

    if (FF)
    { 
      S += ADC12MEM0;
      
       
        FF = false;
        results = S * 2.5 / 4095; 
      
     // ADC12CTL0 |= ADC12SC;
    }
    
    S += ADC12MEM0;
    if (i>9)
    {
      i=0;
      results = 0;
      results = S * 2.5 / 4095/10;
      S = 0;
    }
    
    
    // __bic_SR_IRQ(LPM0_bits);  
    
   // ADC12CTL0 |= ADC12SC;
    
}