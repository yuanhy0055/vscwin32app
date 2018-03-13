
#include <msp430.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "windows.h"
#include "delay.h"
#include "key.h"
#include "SSD1306.h"

//#include "Timers_.h"
//#include "windows.h"


// *****************************************************************************
// *****************************************************************************
// Section: Data
// *****************************************************************************
// *****************************************************************************


void Keyboard(void)
{

    
    unsigned char myKey = '\0';

   // if( bak10mS ^ F_10mS )
    if (1)
    {
       // bak10mS = F_10mS;

        myKey = Keyboard_Hardware();
        if( myKey )
        {
            my_key_val = myKey;

        }
        
      
    }
}


//*****************按键************************//

unsigned char Keyboard_Hardware(void)
{
   // Button1 ---------- P2.0
   // Button2 ---------- P2.1
   // Button3 ---------- P2.2
   // Button4 ---------- P2.3

    //#define CNTshort 2    //对键值进行两次判断
    //#define CNTouat 100
    //#define CNTlong 150
    unsigned short ii = 0;
    char key_RAM = '\0';
    char key_PUSHflang[6] = {0, 0, 0, 0, 0, 0};
    //static unsigned long key_CNT[6] = {0, 0, 0, 0, 0, 0};
    static char key_CHANGE[6] = {0, 0, 0, 0, 0, 0};
    //static char key_OUTflang[6] = {0, 0, 0, 0, 0, 0};    //短按输出值
    //static char key_OUTflang2[6] = {0, 0, 0, 0, 0, 0};   //长按输出值
    
    static unsigned int L=0,L1=0;
    static unsigned char flag = 0,flag1 = 0;
    
    P2SEL &= ~(BIT0 | BIT1 | BIT2 | BIT3); //
    P2DIR &= ~(BIT0 | BIT1 | BIT2 | BIT3); //    P2OUT |= BIT0 | BIT1 | BIT2 | BIT3; 
    P2REN |= BIT0 | BIT1 | BIT2 | BIT3;    //   上拉电阻
    key_RAM = P2IN;
    key_PUSHflang[0] = !(key_RAM & BIT0);//ESC
    key_PUSHflang[1] = !(key_RAM & BIT1);  //上
    key_PUSHflang[2] = !(key_RAM & BIT3);  //下
    key_PUSHflang[3] = !(key_RAM & BIT2);//ENTER
    key_RAM = '\0';
   
  
 for( ii=0; ii<4; ii++ )
    {
        if( key_PUSHflang[ii] )
          {  
              if( !key_CHANGE[ii] )   
                {  key_CHANGE[ii] = true;
                   switch(ii)            //短按键值判断键值
                    {
                        case 0:
                           key_RAM = '1';//ESC
                           break;
                        case 1:
                           key_RAM = '2';//+  上
                           break;
                        case 2:
                           key_RAM = '3';//-  下
                           break;
                        case 3:
                           key_RAM = '4';//ENTER
                           break;
                        case 4:
                           key_RAM = '5';//
                           break;
                        case 5:
                           key_RAM = '6';//
                           break;
                     }                                       
                 }
           }
        else  
          {  key_CHANGE[ii] = false;  }  //  当没有任何按键按下时  解锁
        
    }
    //while(!(P2IN&0x0f)==0x0f);
    //return(key_RAM);
      
    
  if(key_PUSHflang[0])      //长按进入菜单
   { 
     if(!flag) 
     { L++;
       if(L==100)       //原200，按键时间缩短
       { //flag=1;      //自锁
         key_RAM = 'a';
       }
       
       if(L==500)     //菜单退出，原1000，按键时间缩短
       { flag=1;        //自锁
         key_RAM = 'c';
       } 
     }
   }
  else
  {
    flag=0;
    L=0;   
  }
 
 
 
 
 if(key_PUSHflang[1])      //长按报警阈值复位
   { 
     if(!flag1) 
     { L1++;
       if(L1==500)    //或者 if(L==20000)
       { flag1=1;      //自锁
         key_RAM = 'b';
       }
     }
   }
  else
  {
    flag1=0;
    L1=0;
    
  }

 
 
  return(key_RAM);  
}


