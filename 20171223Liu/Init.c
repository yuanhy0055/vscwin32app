#include <msp430.h>
#include "Init.h"

extern void Reprograms_Dco(void);

void Init_clk(void)
{
//    if (CALBC1_8MHZ==0xFF)					// If calibration constant erased
//    {											
//      while(1);                               // do not load, trap CPU!!	
//    }
//    DCOCTL = 0;                               // Select lowest DCOx and MODx settings
//    BCSCTL1 = CALBC1_8MHZ;                    // Set DCO to 8MHz
//    DCOCTL = CALDCO_8MHZ;
    
    //while (CALBC1_8MHZ==0xFF)					// If calibration constant erased
    {											
//      while(1);                               // do not load, trap CPU!!	
      //Reprograms_Dco();
    }

    DCOCTL = 0;                               // Select lowest DCOx and MODx settings
    BCSCTL1 = CALBC1_8MHZ;                    // Set DCO to 8MHz
    DCOCTL = CALDCO_8MHZ;
    
  
    
}