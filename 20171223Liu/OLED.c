#include <msp430.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "SSD1306.h"
//#include "character.h"
#include "iic.h"



#define uchar unsigned char

//extern unsigned char recv_count;
//extern unsigned char recv_buf[16];


//extern void SSD_byte_Write( const unsigned char* data , unsigned short page, unsigned short column,unsigned char flag);
//extern void OLED_ShowCHinese(const unsigned char* data ,unsigned short page,unsigned short column,unsigned short no,unsigned char flag);

 //uchar Time[8];

//uchar Timepresent[8]={0x17,0x09,0x15,0x15,0x40,0x30};

void oled_main(void) 
{       
        uchar Time[8]; 
  
        unsigned int str[30] = {'\0'};
        unsigned int str1[10] = {'\0'};
        
        //strcpy((char*)str,"123" );
        //OledClearScreen();
        //sprintf((char*)str, (const char*)"%8.3f", recv_buf);
        //sprintf((char*)str, (const char*)"%8.3f", a);
        //strcpy((char*)str, "Connect\r\n to the computer");
        //const fontSize size = {0x0810};  //宽  高
        //const position pos = {0x0820};   //位置 X Y
        //OledDisplayStr((const char*)str , size, pos ); 
        
        
      //  SetRX8025SACurrentTime( Timepresent );
        
        ReadRX8025SAClockData(Time);
          
        
        
        Time[0] = ( ( Time[0]/16 )*10 +(Time[0]&0xf));
        Time[1] = ( ( Time[1]/16 )*10 +(Time[1]&0xf));
        Time[2] = ( ( Time[2]/16 )*10 +(Time[2]&0xf));
        Time[3] = ( ( Time[3]/16 )*10 +(Time[3]&0xf));
        Time[4] = ( ( Time[4]/16 )*10 +(Time[4]&0xf));
        Time[5] = ( ( Time[5]/16 )*10 +(Time[5]&0xf));
        
        
        
        sprintf((char*)str, "%02d:%02d:%02d",Time[3],Time[4],Time[5]);
        sprintf((char*)str1, "%02d-%02d-%02d",Time[0],Time[1],Time[2]);
        strcat((char*)str,"\R\n\n");
        strcat((char*)str,(char*)str1);
        
        const fontSize size = {0x0810};  //宽  高
        position pos = {0x0820};   //位置 X Y
        OledDisplayStr((const char*)str , size, pos ); 
        
        
        
   /*
   if(results>=1.5)
     OLED_ShowCHinese(picture,0,108,0,1);
   if(results<1.5 & results>=1.3)
     OLED_ShowCHinese(picture,0,108,1,1);
   if(Flashing & results<1.3)
     OLED_ShowCHinese(picture,0,108,2,1);
   if(!Flashing & results<1.3) 
     OLED_ShowCHinese(picture,0,108,3,1);
   */
        
        
}


