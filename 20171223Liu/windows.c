#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "SSD1306.h"
#include "key.h"
#include "windows.h"
#include "detector.h"
#include "character.h"
#include "iic.h"
#include "rx8025.h"
#include "time.h"
#include "flash.h"
#include "UART.h"

  
Desktops desktops = {
    true,
    Desktop_Instantaneous,
    0,
};




void (*window_desktop)(void) = Desktop_Instantaneous;
unsigned char my_key_val;
extern struct Detector_  DT;

unsigned char model=0;
extern unsigned int GZFlag;
unsigned int GZFlag1 = 0;
unsigned int GZFlag2 = 0;

void Window(void)
{
    //static bool Second = 0;
    void (*window_Current)(void) = desktops.window;
    static void (*window_copy)(void) = Desktop_Instantaneous;

    if( window_copy != desktops.window )
    {
        desktops.Update = false;
        
        window_copy = desktops.window;
        desktops.Update = true;
        OledClearScreen();        //清屏
    }

    window_Current();
}

void Desktop_Instantaneous(void)    //界面显示  
{
   static bool Second = false;
   static unsigned int flag_guzhang = 0;
   //unsigned int GZFlag1 = 0;
   // if(model==0)
   
   unsigned char str[30] = {'\0'};
   //DT.Value3 = 1231245.678;
   if(DT.Value3 < 1000)
     {
       sprintf((char*)str, (const char*)"%8.3f", DT.Value3);
       //if( !(DT.unit ^ 3) )
       strcat((char*)str, " uGy/h");
     }
    else if((DT.Value3 >= 1000)&&(DT.Value3 < 1000000))
      {
        DT.Value3 = DT.Value3/1000;
        sprintf((char*)str, (const char*)"%8.3f", DT.Value3);
        //if( !(DT.unit ^ 3) )
        strcat((char*)str, " mGy/h");
      }
    else if(DT.Value3 >= 1000000)
      {
        DT.Value3 = DT.Value3/1000000;
        sprintf((char*)str, (const char*)"%8.3f", DT.Value3);
        //if( !(DT.unit ^ 3) )
        strcat((char*)str, " Gy/h");
      
      }
        //else
        //    strcat((char*)str, " CPS  ");
        //if( !DT_Connection_CNT )
        //   strcpy((char*)str, "   error       ");
        str[14] = '\0';
        const fontSize size = {0x0810};
        const position pos = {0x1800};
        OledDisplayStr((const char*)str , size, pos );
   
    if(!Uart0Receive.Finish) //判断是否故障
    {
      flag_guzhang++;
      if(flag_guzhang>=65535) flag_guzhang=500;//防止溢出
      if(flag_guzhang>=500)
      {
        GZFlag2 = 1;
      }
    
    }
    else 
    {
      flag_guzhang = 0;
      GZFlag2 = 0;
    }
      
    if((GZFlag)||(GZFlag2))
      {
        SSD_DataBlockWrite( charater+57*32,16, 0xb6, 0x3f00 );   //故障
        SSD_DataBlockWrite( charater+57*32+16,16, 0xb7, 0x3f00 );
        SSD_DataBlockWrite( charater+58*32,16, 0xb6, 0x4f0f ); 
        SSD_DataBlockWrite( charater+58*32+16,16, 0xb7, 0x4f0f );      
        GZFlag1 = 1;
      }
    else 
    {
      if(GZFlag1)
      {
        OledClearScreen();//能否清除故障呢
        GZFlag1 = 0;
      }
    }
    

   unsigned char Time[8]; 
   unsigned int Tstr[10];
      
   if( F_Second ^ Second )     //刷新 
    {
      Second = F_Second;   
    
      ReadRX8025SAClockData(Time);
          
      Time[0] = ( ( Time[0]/16 )*10 +(Time[0]&0xf));//BCD码转换十进制
      Time[1] = ( ( Time[1]/16 )*10 +(Time[1]&0xf));
      Time[2] = ( ( Time[2]/16 )*10 +(Time[2]&0xf));
      Time[3] = ( ( Time[3]/16 )*10 +(Time[3]&0xf));
      Time[4] = ( ( Time[4]/16 )*10 +(Time[4]&0xf));
      Time[5] = ( ( Time[5]/16 )*10 +(Time[5]&0xf));

          
      sprintf((char*)Tstr, "%02d:%02d:%02d",Time[3],Time[4],Time[5]);

      const fontSize size = {0x0810};  //宽  高
      position pos = {0x0000};         //位置 X Y
      OledDisplayStr((const char*)Tstr , size, pos ); //时间显示在左上角
    }
   
   
   extern  float results; 
  
   if(results>=1.9)  //电池电量显示
    { //SSD_DataBlockWrite( picture,32, 0x0100, 0x7f70 );
      SSD_DataBlockWrite( picture,16, 0xb0, 0x7f6c );
      SSD_DataBlockWrite( picture+16,16, 0xb1, 0x7f6c );
    }
    if(results<1.9 & results>=1.7)
    {  
      SSD_DataBlockWrite( picture+32,16, 0xb0, 0x7f6c );
      SSD_DataBlockWrite( picture+48,16, 0xb1, 0x7f6c ); 
    } 
    if(Flashing & results<1.7)
    {
      SSD_DataBlockWrite( picture+64,16, 0xb0, 0x7f6c );
      SSD_DataBlockWrite( picture+80,16, 0xb1, 0x7f6c );
    }
    if(!Flashing & results<1.7)
    {   
      SSD_DataBlockWrite( picture+96,16, 0xb0, 0x7f6c );
      SSD_DataBlockWrite( picture+112,16, 0xb1, 0x7f6c ); 
    }
   
   
    unsigned char temp_KeybCode = my_key_val;
    my_key_val = '\0';
    switch( temp_KeybCode )
    {
        case '1': 
            break;
        case '2': 
            //DT.unit = 3;
            break;
        case '3':
            //DT.unit = 2;
            break;
        case '4':
            break;
        case 'a':
            window_desktop = desktops.window;
            desktops.window = Menu_Main;
            break;
        case 'b':
            break;
        case 'c':
            break;
        case 'd':
            break;
    }
}



/***************菜单***************/

void Menu_Main(void)
{
    static bool init = true;
    static unsigned char page = 0;
    static unsigned char tab = 0;
    static unsigned char pageCpy = 0;
      
    unsigned char a=0 ;
    unsigned char b=1 ;
    unsigned char c1,c2,c3,c4,c5,c6 ;

    if( init )
    {
        init = false;
        tab = 0;
        page = tab/3;
    }

    if( desktops.Update )
    {
        desktops.Update = false;

        if( pageCpy ^ page )
        {         
          pageCpy = page;
          OledClearScreen();
        }
        
        OLED_ShowCHinese(charater,0,45,0,1);    //"选择"
        OLED_ShowCHinese(charater,0,68,1,1);

        if( !(page ^ 1) )      
        {  
            c4 = !(tab ^ 3) ? a : b;
                OLED_ShowCHinese(shuzi,2,0,3,1);       //"4."
                OLED_ShowCHinese(charater,2,16,30,c4); //工作模式
                OLED_ShowCHinese(charater,2,32,31,c4);
                OLED_ShowCHinese(charater,2,48,32,c4);
                OLED_ShowCHinese(charater,2,64,33,c4);
               
            c5 = !(tab ^ 4) ? a : b;
                OLED_ShowCHinese(shuzi,4,0,4,1);       //"5."    
                OLED_ShowCHinese(charater,4,16,22,c5); //累积剂量
                OLED_ShowCHinese(charater,4,32,23,c5);
                OLED_ShowCHinese(charater,4,48,24,c5);
                OLED_ShowCHinese(charater,4,64,25,c5);
                
            c6 = !(tab ^ 5) ? a : b;
                OLED_ShowCHinese(shuzi,6,0,5,1);       //"6."
                OLED_ShowCHinese(charater,6,16,26,c6); //历史记录
                OLED_ShowCHinese(charater,6,32,27,c6);
                OLED_ShowCHinese(charater,6,48,28,c6);
                OLED_ShowCHinese(charater,6,64,29,c6);
              
        }
        else
        {
 
            c1 = !(tab) ? a : b ;
                OLED_ShowCHinese(shuzi,2,0,0,1);      //"1."
                OLED_ShowCHinese(charater,2,16,2,c1); //时间日期
                OLED_ShowCHinese(charater,2,32,3,c1);
                OLED_ShowCHinese(charater,2,48,4,c1);
                OLED_ShowCHinese(charater,2,64,5,c1);
            c2 = !(tab ^ 1) ? a : b;
                OLED_ShowCHinese(shuzi,4,0,1,1);      //"2."
                OLED_ShowCHinese(charater,4,16,6,c2); //报警阈值
                OLED_ShowCHinese(charater,4,32,7,c2);
                OLED_ShowCHinese(charater,4,48,8,c2);
                OLED_ShowCHinese(charater,4,64,9,c2);
            c3 = !(tab ^ 2) ? a : b;
                OLED_ShowCHinese(shuzi,6,0,2,1);       //"3."
                OLED_ShowCHinese(charater,6,16,15,c3); //设备编号
                OLED_ShowCHinese(charater,6,32,16,c3);
                OLED_ShowCHinese(charater,6,48,17,c3);
                OLED_ShowCHinese(charater,6,64,18,c3);
 
        
        }
        
    }

    unsigned char temp_KeybCode = my_key_val;
    my_key_val = '\0';
    switch( temp_KeybCode )
    {
        case '1':
            //init = true;
            //desktops.window = window_desktop;
            break;
        case '2':
            if( --tab > 5)
                tab = 5;
            page = tab/3;
            desktops.Update = true;
            break;
        case '3':
            if( ++tab > 5)
                tab = 0;
            page = tab/3;
            desktops.Update = true;
            break;
        case '4':
            switch( tab )
            {
                case 0:
                    desktops.window = Desktop_Time;
                    break;
                case 1:
                    desktops.window = Desktop_Alarm;
                    break;
                case 2:
                    desktops.window = Desktop_Number;
                    break;
                case 3:
                    desktops.window = Desktop_Workmodel;
                    break;
                case 4:
                    desktops.window = Desktop_Accumulative;
                    break;
                case 5:
                    desktops.window = Desktop_Historyrecord;
                    break;
            }
            break;
        case 'a':
            //init = true;
            //desktops.window = window_desktop;
            break;
        case 'b':
            break;
        case 'c':
            init = true;
            desktops.window = window_desktop;
            break;
        case 'd':
            break;
    }
}



//**********二级菜单************//

void Desktop_Time(void)     //时间设置
{ 
   static unsigned char flag = 1;
   static unsigned char T = 0;
   //unsigned char Time[8];
   static unsigned char Time[8];

  
    if (1)
    {
        unsigned int Timestr[50] = {'\0'};
        unsigned int Timestr1[3] = {'\0'};
        unsigned int Timestr2[3] = {'\0'};
        unsigned int Timestr3[3] = {'\0'};
        unsigned int Timestr4[3] = {'\0'};
        unsigned int Timestr5[3] = {'\0'};
        unsigned int Timestr6[3] = {'\0'};
         
        const fontSize size = {0x0810};  //宽  高
        position pos = {0x0820};         //位置 X Y
        
        if(flag)
          {
            flag=0;
            ReadRX8025SAClockData(Time);//  读取当前时间
                   
            Time[0] = ( ( Time[0]/16 )*10 +(Time[0]&0xf) ); //BCD码转十进制
            Time[1] = ( ( Time[1]/16 )*10 +(Time[1]&0xf) );
            Time[2] = ( ( Time[2]/16 )*10 +(Time[2]&0xf) );
            Time[3] = ( ( Time[3]/16 )*10 +(Time[3]&0xf) );
            Time[4] = ( ( Time[4]/16 )*10 +(Time[4]&0xf) );
            Time[5] = ( ( Time[5]/16 )*10 +(Time[5]&0xf) );
          } 
            
        
        /*
        sprintf((char*)str, "%02d:%02d:%02d",Time[3],Time[4],Time[5]);
        sprintf((char*)str1, "%02d-%02d-%02d",Time[0],Time[1],Time[2]);
        strcat((char*)str,"\R\n\n");
        strcat((char*)str,(char*)str1);
        const fontSize size = {0x0810};  //宽  高
        position pos = {0x0820};         //位置 X Y
        OledDisplayStr((const char*)str , size, pos ); 
       */
        

        (!T) ? strcpy((char*)Timestr, "\235") : strcpy((char*)Timestr, "\230");        
         sprintf((char*)Timestr1, "%02d",Time[3]);   // 时
         strcat((char*)Timestr,(char*)Timestr1);
        
         strcat((char*)Timestr, "\230"":"); 
               
        !(T ^ 1) ? strcat((char*)Timestr, "\235") : strcat((char*)Timestr, "\230");
         sprintf((char*)Timestr2, "%02d",Time[4]);   // 分
         strcat((char*)Timestr,(char*)Timestr2);
         
         strcat((char*)Timestr, "\230"":");  
         
        !(T ^ 2) ? strcat((char*)Timestr, "\235") : strcat((char*)Timestr, "\230");
         sprintf((char*)Timestr3, "%02d",Time[5]);   // 秒
         strcat((char*)Timestr,(char*)Timestr3);
         
         strcat((char*)Timestr,"\R\n\n");     //换行
         
        !(T ^ 3) ? strcat((char*)Timestr, "\235") : strcat((char*)Timestr, "\230");
         sprintf((char*)Timestr4, "%02d",Time[0]);   // 年
         strcat((char*)Timestr,(char*)Timestr4);
         
         strcat((char*)Timestr, "\230""-");       
         
        !(T ^ 4) ? strcat((char*)Timestr, "\235") : strcat((char*)Timestr, "\230");
         sprintf((char*)Timestr5, "%02d",Time[1]);   // 月
         strcat((char*)Timestr,(char*)Timestr5);
         
         strcat((char*)Timestr, "\230""-"); 
         
        !(T ^ 5) ? strcat((char*)Timestr, "\235") : strcat((char*)Timestr, "\230");
         sprintf((char*)Timestr6, "%02d",Time[2]);   // 日
         strcat((char*)Timestr,(char*)Timestr6);
               
         OledDisplayStr((const char*)Timestr , size, pos );        
     }
    

    unsigned char temp_KeybCode = my_key_val;
    my_key_val = '\0';
    switch( temp_KeybCode )
    {
        case '1':
                T=0;
                flag=1;
                Time[3]=((Time[3]/10)<<4)+((Time[3]%10)&0x0F);//十进制转换BCD码
                Time[4]=((Time[4]/10)<<4)+((Time[4]%10)&0x0F);
                Time[5]=((Time[5]/10)<<4)+((Time[5]%10)&0x0F);
                Time[0]=((Time[0]/10)<<4)+((Time[0]%10)&0x0F);
                Time[1]=((Time[1]/10)<<4)+((Time[1]%10)&0x0F);
                Time[2]=((Time[2]/10)<<4)+((Time[2]%10)&0x0F);
                
                SetRX8025SACurrentTime( Time );
                desktops.window = Menu_Main;
            break;
        case '2':
          switch(T)
          {
            case 0:
              Time[3]++;    //时
              if (Time[3]>23)
                Time[3]=0; 
              
              break;
            case 1:
              Time[4]++;    //分
              if (Time[4]>59)
                Time[4]=0;
              
              break;
            case 2:
              Time[5]++;    //秒
              if (Time[5]>59)
                Time[5]=0;
              
              break;
            case 3:
              Time[0]++;    //年
              if(Time[0]>99)
                Time[0]=0;
              
              break;
            case 4:
              Time[1]++;    //月
              if (Time[1]>12)
                Time[1]=1;
              
              break;
            case 5:                       
              Time[2]++;    //日
              if (Time[2]>31)
                Time[2]=1;
              
              break;          
          }
              
            break;
        case '3':
           switch(T)
          {
            case 0:
              Time[3]--;    //时
              if (Time[3]>23)
                Time[3]=0; 
              
              break;
            case 1:
              Time[4]--;    //分
              if (Time[4]>59)
                Time[4]=0;
              
              break;
            case 2:
              Time[5]--;    //秒
              if (Time[5]>59)
                Time[5]=0;
              
              break;
            case 3:
              Time[0]--;    //年
              if(Time[0]>99)
                Time[0]=0;
              
              break;
            case 4:
              Time[1]--;    //月
              if (Time[1]==0)
                Time[1]=1;
              break;
            case 5:         
              Time[2]--;    //日
              if (Time[2]==0)
                Time[2]=1;
              break;          
          }
 
            break;
        case '4':      
            T++;
            if (T>5)
              T=0;
            break;
        case 'a':
            break;
        case 'b':
            break;
        case 'c':
            break;
        case 'd':
            break;
    }
}



static unsigned char N=0;  

void Desktop_Alarm(void)    //报警阈值
{
  //static unsigned char N=0;
  unsigned char c1,c2,c3,c4;
  
    
      c1 = !N ? 0:1;              
        OLED_ShowCHinese(shuzi,0,0,0,1);       //"1."
        OLED_ShowCHinese(charater,0,16,10,c1); //剂量率高报
        OLED_ShowCHinese(charater,0,32,11,c1);
        OLED_ShowCHinese(charater,0,48,12,c1);
        OLED_ShowCHinese(charater,0,64,13,c1);
        OLED_ShowCHinese(charater,0,80,6,c1);
         
     c2 = !(N^1) ? 0:1;
        OLED_ShowCHinese(shuzi,2,0,1,1);       //"2."
        OLED_ShowCHinese(charater,2,16,10,c2); //剂量率低报
        OLED_ShowCHinese(charater,2,32,11,c2);
        OLED_ShowCHinese(charater,2,48,12,c2);
        OLED_ShowCHinese(charater,2,64,14,c2);
        OLED_ShowCHinese(charater,2,80,6,c2);
        
     c3 = !(N^2) ? 0:1;
        OLED_ShowCHinese(shuzi,4,0,2,1);       //"3."
        OLED_ShowCHinese(charater,4,16,22,c3); //累积剂量高报
        OLED_ShowCHinese(charater,4,32,23,c3);
        OLED_ShowCHinese(charater,4,48,24,c3);
        OLED_ShowCHinese(charater,4,64,25,c3);
        OLED_ShowCHinese(charater,4,80,13,c3);
        OLED_ShowCHinese(charater,4,96,6,c3);
        
     c4 = !(N^3) ?0:1;
        OLED_ShowCHinese(shuzi,6,0,3,1);      //"4."
        OLED_ShowCHinese(charater,6,16,22,c4); //累积剂量低报
        OLED_ShowCHinese(charater,6,32,23,c4);
        OLED_ShowCHinese(charater,6,48,24,c4);
        OLED_ShowCHinese(charater,6,64,25,c4);
        OLED_ShowCHinese(charater,6,80,14,c4);
        OLED_ShowCHinese(charater,6,96,6,c4);

   
    unsigned char temp_KeybCode = my_key_val;
    my_key_val = '\0';
    switch( temp_KeybCode )
    {
        case '1':
            N=0;
            desktops.window = Menu_Main;
            break;
        case '2':
            N--;
            if(N>3)
              N=3;
            break;
        case '3':
            N++;
            if(N>3)
              N=0;
            break;
        case '4':
            desktops.window =Desktop_alarmvalue;
            // if(N==0)desktops.window =Desktop_alarmvalue;
            // if(N==1)desktops.window =Desktop_alarmvalue; 
            // if(N==2)desktops.window =Desktop_alarmvalue;
            // if(N==3)desktops.window =Desktop_alarmvalue;
            
            break;
        case 'a':
            break;
        case 'b':
          desktops.window =Desktop_resetalarmvalue;
            break;
        case 'c':
            break;
        case 'd':
            break;
    }
}


void Desktop_resetalarmvalue(void)     // 三级     复位报警阈值
{
  static unsigned char F=1,T=0;
  unsigned char c1,c2;
   if(F)
   {  
     OLED_ShowCHinese(charater,1,0,40,1); //复位报警阈值?
     OLED_ShowCHinese(charater,1,16,41,1);
     OLED_ShowCHinese(charater,1,32,6,1); 
     OLED_ShowCHinese(charater,1,48,7,1);
     OLED_ShowCHinese(charater,1,64,8,1);
     OLED_ShowCHinese(charater,1,80,9,1);
     OLED_ShowCHinese(charater,1,96,42,1);
      
     c1 = !T ? 0 : 1;
     c2 = !(T^1) ? 0 : 1;
     
     OLED_ShowCHinese(charater,4,16,44,c1);
     OLED_ShowCHinese(charater,4,48,43,c2);
   }
     
     
    unsigned char temp_KeybCode = my_key_val;
    my_key_val = '\0';
    switch( temp_KeybCode )
    {
        case '1':
            T = 0;
            F = 1;
            desktops.window = Desktop_Alarm;
            break;
        case '2': 
            T=0;
            break;
        case '3':
            T=1;
            break;
        case '4':
            if(T==1)
            {
              F = 0;
              Copy(Flash_Segment_A+1,40,Flash_Segment_A+70);
              OledClearScreen();        //清屏
              OLED_ShowCHinese(charater,3,40,45,1);
              OLED_ShowCHinese(charater,3,72,46,1);
            }
            break;
        case 'a':
            break;
        case 'b':
            break;
        case 'c':
            break;
        case 'd':
            break;
    }
}



void Desktop_alarmvalue(void)     // 三级     设置报警阈值
{
 static unsigned char flag=1, T = 0;
 unsigned char kk[10][2] = {{'1'},{'0'},{'0'},{'0'},{'+'},{'0'},{'0'}};
 
 static unsigned char kk1[10][2] = {{'2'},{'0'},{'0'},{'0'},{'+'},{'0'},{'0'}};
 static unsigned char kk2[10][2] = {{'3'},{'0'},{'0'},{'0'},{'+'},{'0'},{'0'}};
 static unsigned char kk3[10][2] = {{'4'},{'0'},{'0'},{'0'},{'+'},{'0'},{'0'}};
 static unsigned char kk4[10][2] = {{'5'},{'0'},{'0'},{'0'},{'+'},{'0'},{'0'}};
 
 static unsigned char alarmvalue1[10] = {'0'}; 
 static unsigned char alarmvalue2[10] = {'0'};
 static unsigned char alarmvalue3[10] = {'0'};
 static unsigned char alarmvalue4[10] = {'0'};
 unsigned char *p=0 ;
 unsigned char i,j,k;
 
   
    if(flag)
    {
      flag=0;
      Read_flash((uchar *)(Flash_Segment_A+70), alarmvalue1,9);  // 读flash中报警阈值
      Read_flash((uchar *)(Flash_Segment_A+80), alarmvalue2,9);
      Read_flash((uchar *)(Flash_Segment_A+90), alarmvalue3,9);
      Read_flash((uchar *)(Flash_Segment_A+100), alarmvalue4,9);
  
        /*
        kk1[0][0] = alarmvalue1[0];
        kk1[1][0] = alarmvalue1[2];
        kk1[2][0] = alarmvalue1[3];
        kk1[3][0] = alarmvalue1[4];
        kk1[4][0] = alarmvalue1[6]; //+ -
        kk1[5][0] = alarmvalue1[7];
        kk1[6][0] = alarmvalue1[8];*/
        

        for(j=0,k=0;j<9;j++,k++)
        {
          if(j==1||j==5)  j++;
          kk1[k][0] = alarmvalue1[j];
        }
        
        for(j=0,k=0;j<9;j++,k++)
        {
          if(j==1||j==5)  j++;
          kk2[k][0] = alarmvalue2[j];
        }
        
        for(j=0,k=0;j<9;j++,k++)
        {
          if(j==1||j==5)  j++;
          kk3[k][0] = alarmvalue3[j];
        }
        for(j=0,k=0;j<9;j++,k++)
        {
          if(j==1||j==5)  j++;
          kk4[k][0] = alarmvalue4[j];
        }
    }
 
 
    
   switch(N)        //值传给KK[]，显示对应报警阈值
   {
      case 0:
        p = alarmvalue1;
        for(i=0;i<7;i++)    
         {          
            kk[i][0]=kk1[i][0]; 
         }
       break; 
      case 1:
        p = alarmvalue2;
        for(i=0;i<7;i++)    
         {          
            kk[i][0]=kk2[i][0]; 
         }
       break;
      case 2:
        p = alarmvalue3;
        for(i=0;i<7;i++)    
         {          
            kk[i][0]=kk3[i][0]; 
         }
       break;
      case 3:
        p = alarmvalue4;
        for(i=0;i<7;i++)    
         {          
            kk[i][0]=kk4[i][0]; 
         }
       break;
      default:
       break;
   }
 
 
        unsigned char str[60] = {'\0'};
        const fontSize size = {0x0810};
        position pos = {0x1800};
           
        (!T) ? strcpy((char*)str, "\235") : strcpy((char*)str, "\230");        
        strcat((char*)str, &kk[0][0]);
        
        strcat((char*)str, "\230"".");   //小数点
        
        !(T ^ 1) ? strcat((char*)str, "\235") : strcat((char*)str, "\230");
         strcat((char*)str, &kk[1][0]);
        !(T ^ 2) ? strcat((char*)str, "\235") : strcat((char*)str, "\230");
         strcat((char*)str, &kk[2][0]);
        !(T ^ 3) ? strcat((char*)str, "\235") : strcat((char*)str, "\230");
         strcat((char*)str, &kk[3][0]); 
         
         strcat((char*)str, "\230""E");
          
        !(T ^ 4) ? strcat((char*)str, "\235") : strcat((char*)str, "\230");
         strcat((char*)str, &kk[4][0]);      // -+号
         
        !(T ^ 5) ? strcat((char*)str, "\235") : strcat((char*)str, "\230");
         strcat((char*)str, &kk[5][0]); 
        !(T ^ 6) ? strcat((char*)str, "\235") : strcat((char*)str, "\230");
         strcat((char*)str, &kk[6][0]);
         
        if(N<2) strcat((char*)str, " uGy/h ") ;//显示单位20171219
          else strcat((char*)str, " uGy ") ;
        OledDisplayStr((const char*)str , size, pos );
        
    

    
    unsigned char temp_KeybCode = my_key_val;
    my_key_val = '\0';
    switch( temp_KeybCode )
    {
        case '1':
            T = 0; 
            flag=1;
            
            strcpy((char*)p, &kk[0][0]);
            strcat((char*)p, ".");
            strcat((char*)p, &kk[1][0]);
            strcat((char*)p, &kk[2][0]);
            strcat((char*)p, &kk[3][0]);
            strcat((char*)p, "E");   
            strcat((char*)p, &kk[4][0]);  //+ -
            strcat((char*)p, &kk[5][0]);
            strcat((char*)p, &kk[6][0]);  
                    
            EraseFlash(Flash_Segment_A+64);
   
            Write_flash((uchar *)Flash_Segment_A+70,alarmvalue1, 9);
            Write_flash((uchar *)Flash_Segment_A+80,alarmvalue2, 9);
            Write_flash((uchar *)Flash_Segment_A+90,alarmvalue3, 9);
            Write_flash((uchar *)Flash_Segment_A+100,alarmvalue4, 9);
            desktops.window = Desktop_Alarm;
            break;
        case '2':       
          if(T!=4) 
          {
            if(kk[T][0]=='9')
              kk[T][0] = '0';
            else 
              kk[T][0]++;
          }
          else 
          {
            if(kk[T][0] == '+')
              kk[T][0] = '-';
            else
              kk[T][0] = '+';
          }
            break;
        case '3':
          if(T!=4) 
          {
            if(kk[T][0]=='0')
              kk[T][0] = '9';
            else 
              kk[T][0]--;
          }
         else 
         {
           if(kk[T][0] == '+')
             kk[T][0] = '-';
           else
             kk[T][0] = '+';
         }  
         
            break;
        case '4':     
            if( ++T > 6)
                T = 0;         
            break;
        case 'a':
            break;
        case 'b':
            break;
        case 'c':
            break;
        case 'd':
            break;   
    }
    
    
          switch(N)         //修改后的值传给对应报警数组
            {
               case 0:
                 for(i=0;i<7;i++)    
                 {          
                    kk1[i][0]=kk[i][0]; 
                 }
                break; 
               case 1: 
                 for(i=0;i<7;i++)    
                 {          
                    kk2[i][0]=kk[i][0]; 
                 }
                break;
              case 2: 
                for(i=0;i<7;i++)    
                {          
                    kk3[i][0]=kk[i][0];
                }
                break;
              case 3:  
                for(i=0;i<7;i++)    
                 {          
                    kk4[i][0]=kk[i][0]; 
                 }
                break;
              default:
                break;
           }
}



void Desktop_Number(void)    //设备编号
{ 
  
    unsigned int str[5] = {'\0'};
    unsigned char Num[3] = {'\0'};
    Read_flash((uchar *)(Flash_Segment_A), Num,1);
    sprintf((char*)str, (const char*)"%d", Num[0]);
    const fontSize size = {0x0810};  //宽  高
    position pos = {0x1230};   //位置 X Y
    OledDisplayStr((const char*)str , size, pos );

    
    unsigned char temp_KeybCode = my_key_val;
    my_key_val = '\0';
    switch( temp_KeybCode )
    {
        case '1':
            desktops.window = Menu_Main;
            break;
        case '2':             
              break;
        case '3':
            break;
        case '4':
            //desktops.window = Menu_Main;       
            break;
        case 'a':
            break;
        case 'b':
            break;
        case 'c':
            break;
        case 'd':
            break;
    }
}


/*********工作模式*********/

void Desktop_Workmodel(void)
{
    static unsigned char V=0;
    unsigned char c1,c2,c3;
    unsigned char temp_KeybCode;
     
      c1 = !V ? 0:1;              
        OLED_ShowCHinese(shuzi,1,0,0,1);       //"1."
        OLED_ShowCHinese(charater,1,24,34,c1); //测量
        OLED_ShowCHinese(charater,1,40,35,c1);
        
     c2 = !(V^1) ? 0:1;
        OLED_ShowCHinese(shuzi,3,0,1,1);       //"2."
        OLED_ShowCHinese(charater,3,24,36,c2); //通讯
        OLED_ShowCHinese(charater,3,40,37,c2);
        
     c3 = !(V^2) ? 0:1;
        OLED_ShowCHinese(shuzi,5,0,2,1);       //"3."
        OLED_ShowCHinese(charater,5,24,38,c3); //刻度
        OLED_ShowCHinese(charater,5,40,39,c3);
        

    temp_KeybCode = my_key_val;
    my_key_val = '\0';
    switch( temp_KeybCode )
    {
        case '1':
            V = 0;
            desktops.window = Menu_Main;
            break;
        case '2':       
            V--;
            if(V>2)
               V=2;
            break;
        case '3':
            V++;
            if(V>2)
               V=0;
            break;        
        case '4':
            switch( V )
            {
                case 0:
                    model = 0;
                    //V = 0;
                    //window_desktop = desktops.window;
                    desktops.window = Menu_Main;
                    break;
                case 1:
                    //model = 1;
                    V = 0;
                    desktops.window = Menu_Main;//通讯中
                    break;
                case 2:
                    model = 2;
                    V = 0;
                    desktops.window = Desktop_Kedu;//刻度中
                    //OledClearScreen();
                    //OLED_ShowCHinese(charater,5,24,38,1); //刻度
                    //OLED_ShowCHinese(charater,5,40,39,1);
                    break;
                
            }
            //desktops.window = window_desktop;
            break;
        case 'a':
            break;
        case 'b':
            break;
        case 'c':
            break;
        case 'd':
            break;
     }
}

void Desktop_Kedu(void)//刻度模式
{
    unsigned char temp_KeybCode;
    
    if( desktops.Update )
      {
        desktops.Update = false;
        OLED_ShowCHinese(charater,4,32,47,1);    //正在
        OLED_ShowCHinese(charater,4,48,48,1);
        OLED_ShowCHinese(charater,4,64,38,1);    //刻度
        OLED_ShowCHinese(charater,4,80,39,1);
      } 

    temp_KeybCode = my_key_val;
    my_key_val = '\0';
    switch( temp_KeybCode )
    {
        case '1':
            model = 0;
            desktops.window = Menu_Main;
            break;
        case '2':       
            break;
        case '3':
            break;        
        case '4':
            break;
        case 'a':
            break;
        case 'b':
            break;
        case 'c':
            break;
        case 'd':
            break;
     }
  
  
}

/*******累积剂量**********/

void Desktop_Accumulative(void)     //累积剂量
{
   
    if( desktops.Update )
    //if(1)
    {
        desktops.Update = false;

        unsigned char str[20] = {'\0'};
        //sprintf((char*)str, (const char*)"%8.3f", DT.Accumulation1);
        if( !(DT.unit ^ 3) )
        { 
          float temp=0;
          unsigned char Acc[20] = {'\0'};
          //Read_flash((uchar *)(Flash_Segment_B), Acc,10);//读取累积剂量
          //strcpy((char*)str, Acc);
          temp = DT.Accumulation1;
          //DT.Accumulation1 = atof(Acc);
          //temp = atof((const char*)Acc);
           
           //temp =temp/3600;//已经处理过了
          
          if(temp>1000)
          {
            //DT.Accumulation1=DT.Accumulation1/1000;  
            sprintf((char*)str, (const char*)"%8.3f", temp/1000);
            strcat((char*)str, " mGy ") ;
          }
          else
          {
            sprintf((char*)str, (const char*)"%8.3f", temp);
            strcat((char*)str, " uGy ") ;
          }
          
        }
        else
        {
          sprintf((char*)str, (const char*)"%8.3f", DT.Accumulation2);
          strcat((char*)str, " count");
        }
        const fontSize size = {0x0810};
        const position pos = {0x1800};  //1010
        OledDisplayStr((const char*)str , size, pos );
    }

    SSD_DataBlockWrite( charater+59*32,16, 0xb6, 0x3f00 );  //按上键清零 
    SSD_DataBlockWrite( charater+59*32+16,16, 0xb7, 0x3f00 );
    SSD_DataBlockWrite( charater+60*32,16, 0xb6, 0x4f0f ); 
    SSD_DataBlockWrite( charater+60*32+16,16, 0xb7, 0x4f0f );
    SSD_DataBlockWrite( charater+61*32,16, 0xb6, 0x5f1e );   
    SSD_DataBlockWrite( charater+61*32+16,16, 0xb7, 0x5f1e );
    SSD_DataBlockWrite( charater+62*32,16, 0xb6, 0x6f2d ); 
    SSD_DataBlockWrite( charater+62*32+16,16, 0xb7, 0x6f2d );
    SSD_DataBlockWrite( charater+63*32,16, 0xb6, 0x7f3c );   
    SSD_DataBlockWrite( charater+63*32+16,16, 0xb7, 0x7f3c );

    unsigned char temp_KeybCode = my_key_val;
    my_key_val = '\0';
    switch( temp_KeybCode )
    {
        case '1':
            desktops.window = Menu_Main;
            break;
        case '2':
            unsigned char strstr[20] = {'\0'};
            DT.Accumulation1 = 0;     //清零
            
            sprintf((char*)strstr, "%8.3f",DT.Accumulation1);
            //sprintf((char*)strstr, "%8.3f",0.000);
            strcat((char*)strstr, " uGy ") ;
           // strcy((char*)strstr, "   0.000 uGy ") ;
            const fontSize size = {0x0810};
            position pos = {0x1800};  
            OledClearScreen();        //清屏
            OledDisplayStr((const char*)strstr , size, pos );
          
          
            EraseFlash(Flash_Segment_B);
            //DT.Accumulation2 = 0;
            //desktops.Update = true;
            break;
        case '3':
            break;
        case '4':
            break;
        case 'a':
            break;
        case 'b':
            break;
        case 'c':
            break;
        case 'd':
            break;
    }
}



/*********历史记录*********/

void Desktop_Historyrecord(void)    //历史记录
{
  static unsigned char H_Flag = 1;
  static unsigned int NN=0;
  
  static unsigned char His_data[10]={0};
  static unsigned char His_data1[10]={0};
  static unsigned char His_data2[10]={0};
  unsigned char History_time[20]={0};
  unsigned char History_value[15]={0};
  unsigned char His_biaoshi[5]={0};
  
  union 
  {
    float History;
    unsigned  char  History_Char[5];
   };
  
  union 
  {
    float History1;
    unsigned  char  History_Char1[5];
   };
  
  union 
  {
    float History2;
    unsigned  char  History_Char2[5];
   };
  
  
  
  if(H_Flag)
  {
    H_Flag = 0;
   
    if(NN==0)
    {
      while(Read_StatReg()&0x01); 
      for(char lishi1_i=0;lishi1_i<6;lishi1_i++) 
      {
        His_data[lishi1_i]=Read_Byte(0x00000000+lishi1_i);
      }

      while(Read_StatReg()&0x01); 
      for(char lishi1_k=0;lishi1_k<4;lishi1_k++) 
      {
        History_Char[lishi1_k]=Read_Byte(0x00000006+lishi1_k);  
      }  

      sprintf((char*)History_value, "%8.3f",History);
      sprintf((char*)History_time, "%02d-%02d-%02d  %02d:%02d",His_data[0],His_data[1],His_data[2],His_data[3],His_data[4]);
      /*
       strcpy((char*)His_biaoshi, "Max");
       const fontSize size = {0x0810}; 
       position pos = {0x3000};   //位置 X Y
       OledClearScreen();        //清屏
       OledDisplayStr((const char*)His_biaoshi, size, pos ); 
       */
       OledClearScreen();        //清屏
       
       SSD_DataBlockWrite( charater+52*32,16, 0xb6, 0x1f00 );  //最高剂量率
       SSD_DataBlockWrite( charater+52*32+16,16, 0xb7, 0x1f00 ); 
       SSD_DataBlockWrite( charater+13*32,16, 0xb6, 0x2f0f );
       SSD_DataBlockWrite( charater+13*32+16,16, 0xb7, 0x2f0f );
       SSD_DataBlockWrite( charater+10*32,16, 0xb6, 0x3f1f );
       SSD_DataBlockWrite( charater+10*32+16,16, 0xb7, 0x3f1f );   
       SSD_DataBlockWrite( charater+11*32,16, 0xb6, 0x4f2f );  
       SSD_DataBlockWrite( charater+11*32+16,16, 0xb7, 0x4f2f ); 
       SSD_DataBlockWrite( charater+12*32,16, 0xb6, 0x5f3f );
       SSD_DataBlockWrite( charater+12*32+16,16, 0xb7, 0x5f3f );
    
     }
    
    
    if(NN==1)
    {
      while(Read_StatReg()&0x01); 
      for(char lishi2_i=0;lishi2_i<6;lishi2_i++) 
      {
        His_data1[lishi2_i]=Read_Byte(0x00002000+lishi2_i);
      }
     
      while(Read_StatReg()&0x01); 
      for(char lishi2_k=0;lishi2_k<4;lishi2_k++) 
      {
        History_Char1[lishi2_k]=Read_Byte(0x00002006+lishi2_k);  
      }  
      
      sprintf((char*)History_value, "%8.3f",History1);
      sprintf((char*)History_time, "%02d-%02d-%02d  %02d:%02d",His_data1[0],His_data1[1],His_data1[2],His_data1[3],His_data1[4]);
      
       OledClearScreen();        //清屏
       //SSD_DataBlockWrite( charater+6*32,32, 0xb6, 0x7f0f );  //报警进入
       SSD_DataBlockWrite( charater+6*32,16, 0xb6, 0x1f00 );    //报警进入
       SSD_DataBlockWrite( charater+6*32+16,16, 0xb7, 0x1f00 );
       SSD_DataBlockWrite( charater+7*32,16, 0xb6, 0x2f0f );
       SSD_DataBlockWrite( charater+7*32+16,16, 0xb7, 0x2f0f );
       SSD_DataBlockWrite( charater+53*32,16, 0xb6, 0x3f1f );
       SSD_DataBlockWrite( charater+53*32+16,16, 0xb7, 0x3f1f );
       SSD_DataBlockWrite( charater+54*32,16, 0xb6, 0x4f2f );
       SSD_DataBlockWrite( charater+54*32+16,16, 0xb7, 0x4f2f );
    }
    
    if(NN==2)
    {
      while(Read_StatReg()&0x01); 
      for(char lishi3_i=0;lishi3_i<6;lishi3_i++) 
      {
        His_data2[lishi3_i]=Read_Byte(0x00004000+lishi3_i);
      }
     
      while(Read_StatReg()&0x01); 
      for(char lishi3_k=0;lishi3_k<4;lishi3_k++) 
      {
        History_Char2[lishi3_k]=Read_Byte(0x00004006+lishi3_k);  
      }  
      
      sprintf((char*)History_value, "%8.3f",History2);
      sprintf((char*)History_time, "%02d-%02d-%02d  %02d:%02d",His_data2[0],His_data2[1],His_data2[2],His_data2[3],His_data2[4]);
      
       OledClearScreen();        //清屏     
       SSD_DataBlockWrite( charater+6*32,16, 0xb6, 0x1f00 );   //报警退出
       SSD_DataBlockWrite( charater+6*32+16,16, 0xb7, 0x1f00 );
       SSD_DataBlockWrite( charater+7*32,16, 0xb6, 0x2f0f );
       SSD_DataBlockWrite( charater+7*32+16,16, 0xb7, 0x2f0f );
       SSD_DataBlockWrite( charater+55*32,16, 0xb6, 0x3f1f );
       SSD_DataBlockWrite( charater+55*32+16,16, 0xb7, 0x3f1f );
       SSD_DataBlockWrite( charater+56*32,16, 0xb6, 0x4f2f ); 
       SSD_DataBlockWrite( charater+56*32+16,16, 0xb7, 0x4f2f );
    }
   
    //sprintf((char*)History_value, "%8.3f",History);
    strcat((char*)History_value, " uGy/h");
    const fontSize size = {0x0810};  //高   宽
    position pos = {0x1800};   //位置 X Y
    //OledClearScreen();        //清屏
    OledDisplayStr((const char*)History_value , size, pos );
     
    //sprintf((char*)History_time, "%02d-%02d-%02d  %02d:%02d",data[0],data[1],data[2],data[3],data[4]);
    pos.x=0x00;
    pos.y=0x00;
    OledDisplayStr((const char*)History_time , size, pos ); //时间显示在顶上
  }

  
  
    //unsigned char w25x[10]={0,0,0,0,0,0,0,0,0,0}; 
    extern bool his_clear,his_reset;
    unsigned char temp_KeybCode = my_key_val;
    my_key_val = '\0';
    switch( temp_KeybCode )
    {
        case '1':
            H_Flag = 1;
            NN=0;
            desktops.window = Menu_Main;
            break;
        case '2':
            his_clear = true;
            his_reset = true;
            
            /*
             Erase_Page(0x00002000);
             while(Read_StatReg()&0x01);
             Write_Enable(); 
             Write_Date(0x00002000,w25x,10); 

             Erase_Page(0x00004000);
             while(Read_StatReg()&0x01);
             Write_Enable(); 
             Write_Date(0x00004000,w25x,10); 
            */
        
            
            strcpy((char*)History_value, "   0.000 uGy/h");
            const fontSize size = {0x0810}; 
            position pos = {0x2000}; 
            OledClearScreen();        //清屏
            OledDisplayStr((const char*)History_value , size, pos );
            break;
        case '3':
            H_Flag = 1;
            NN++;
            if(NN>2)   
             NN=0; 
            break;
        case '4':
            break;
        case 'a':
            break;
        case 'b':
            break;
        case 'c':
            break;
        case 'd':
            break;
    }



}


