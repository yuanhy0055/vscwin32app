
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


//*****************����************************//

unsigned char Keyboard_Hardware(void)
{
   // Button1 ---------- P2.0
   // Button2 ---------- P2.1
   // Button3 ---------- P2.2
   // Button4 ---------- P2.3

    //#define CNTshort 2    //�Լ�ֵ���������ж�
    //#define CNTouat 100
    //#define CNTlong 150
    unsigned short ii = 0;
    char key_RAM = '\0';
    char key_PUSHflang[6] = {0, 0, 0, 0, 0, 0};
    //static unsigned long key_CNT[6] = {0, 0, 0, 0, 0, 0};
    static char key_CHANGE[6] = {0, 0, 0, 0, 0, 0};
    //static char key_OUTflang[6] = {0, 0, 0, 0, 0, 0};    //�̰����ֵ
    //static char key_OUTflang2[6] = {0, 0, 0, 0, 0, 0};   //�������ֵ
    
    static unsigned int L=0,L1=0;
    static unsigned char flag = 0,flag1 = 0;
    
    P2SEL &= ~(BIT0 | BIT1 | BIT2 | BIT3); //
    P2DIR &= ~(BIT0 | BIT1 | BIT2 | BIT3); //    P2OUT |= BIT0 | BIT1 | BIT2 | BIT3; 
    P2REN |= BIT0 | BIT1 | BIT2 | BIT3;    //   ��������
    key_RAM = P2IN;
    key_PUSHflang[0] = !(key_RAM & BIT0);//ESC
    key_PUSHflang[1] = !(key_RAM & BIT1);  //��
    key_PUSHflang[2] = !(key_RAM & BIT3);  //��
    key_PUSHflang[3] = !(key_RAM & BIT2);//ENTER
    key_RAM = '\0';
   
  
 for( ii=0; ii<4; ii++ )
    {
        if( key_PUSHflang[ii] )
          {  
              if( !key_CHANGE[ii] )   
                {  key_CHANGE[ii] = true;
                   switch(ii)            //�̰���ֵ�жϼ�ֵ
                    {
                        case 0:
                           key_RAM = '1';//ESC
                           break;
                        case 1:
                           key_RAM = '2';//+  ��
                           break;
                        case 2:
                           key_RAM = '3';//-  ��
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
          {  key_CHANGE[ii] = false;  }  //  ��û���κΰ�������ʱ  ����
        
    }
    //while(!(P2IN&0x0f)==0x0f);
    //return(key_RAM);
      
    
  if(key_PUSHflang[0])      //��������˵�
   { 
     if(!flag) 
     { L++;
       if(L==100)       //ԭ200������ʱ������
       { //flag=1;      //����
         key_RAM = 'a';
       }
       
       if(L==500)     //�˵��˳���ԭ1000������ʱ������
       { flag=1;        //����
         key_RAM = 'c';
       } 
     }
   }
  else
  {
    flag=0;
    L=0;   
  }
 
 
 
 
 if(key_PUSHflang[1])      //����������ֵ��λ
   { 
     if(!flag1) 
     { L1++;
       if(L1==500)    //���� if(L==20000)
       { flag1=1;      //����
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


