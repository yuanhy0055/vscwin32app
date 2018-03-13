/*
2017.11.30:1������ʱ�Ƿ��ʼ��flash�жϣ�2���ػ�������������˵���Ҳ�ǳ����ػ���
2017.12.14:1.����ʱ��ָʾ�ƣ��ղ���ģ�����ݱ����ϣ�����3����Ϊ0�����ϣ��������
2017.12.15:1.�������������


*/

#include <msp430.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "UART.h"
#include "delay.h"
#include "SSD1306.h"
#include "key.h"
#include "detector.h"
#include "windows.h"
#include "iic.h"
#include "rx8025.h"	
#include "time.h"
#include "w25x16.h"
#include "flash.h"
#include "AD.h"
//#include "character.h"





#define uchar unsigned char



unsigned char Key_guanji=0;
unsigned char Kaiji_flag=0;
uchar Number=123;                //��FLASH��д�������
uchar Num[5]={0};
uchar alarm1[10]="1.000E+06";    //�����ʸ߱���ֵ
uchar alarm2[10]="1.000E+03";    //�����ʵͱ���ֵ
uchar alarm3[10]="5.000E+04";    //�ۻ������߱���ֵ
uchar alarm4[10]="2.000E+04";    //�ۻ������ͱ���ֵ

uchar alarm11[10]="0.000E+00";    //��ʼֵ
uchar alarm22[10]="0.000E+00";
uchar alarm33[10]="0.000E+00";
uchar alarm44[10]="0.000E+00";


void kaiji();
void guanji();
void baojing();
void peak_value();
void alarm_start();
void alarm_end();
void Reset_w25X16();



extern struct Detector_ DT;
extern unsigned char charater[];
extern unsigned char picture[];
extern float results;
extern void Init_clk(void);         //�ⲿ��������


//unsigned char Rchaxun[4]={'R',0x00,0x00,'A'};//��ѯ���ָ��
//unsigned char Rjll[4]={'R',0x02,0x00,'R'};//��������ָ��
//unsigned char Rjsl[4]={'R',0x02,0x00,'C'};//��������ָ��

unsigned int str[10] = {'\0'};


bool his_clear = false;
bool his_reset = false;
unsigned char BJ_flag_start=0;
unsigned char BJ_flag_end=1;

uchar SNum[10]={0,0,0,0,0,0,0,0,0,0};   //��ʼ���ⲿflash


uchar SNum1[12]={0x55,2,2,2,2,2,2,2,2,2}; 
uchar SNum2[12]={0xaa,3,3,3,3,3,3,3,3,3}; 
uchar SNum3[12]={11,0x55,0xaa,1,1,1,1,1,1,1};

uchar ceshi1[12];
uchar ceshi2[12]; 
uchar ceshi3[12]; 
uchar ceshi4[12];
uchar ceshi5[12];


int main( void )
{
  
  WDTCTL = WDTPW + WDTHOLD;  // Stop watchdog timer to prevent time out reset

  Init_clk();                //��ʼ��������ʱ��
  kaiji();
  //InitOled();
  init_UART0();
  init_UART1();
  init_Detector();
  init_TimerA();
  InitRX8025SA();
  battery_AD ();   //��ص�ѹ���
  SPI_IO_Init();   //��ʼSPI����
  
  //kaiji();
   
  //uchar Timepresent[8]={0x17,0x10,0x19,0x11,0x30,0x00};
  //SetRX8025SACurrentTime( Timepresent );
  
    Read_flash((uchar *)(Flash_Segment_A), Num,1);
    if(Num[0] == 0xff)//�Ƿ��ʼ���ڲ�flash
    {
      WriteFlash0(Flash_Segment_A);   //��Flashд�������
      
      Write_flash((uchar *)Flash_Segment_A+1,alarm1, 9);  //��Flashд������ֵ
      Write_flash((uchar *)Flash_Segment_A+11,alarm2, 9);
      Write_flash((uchar *)Flash_Segment_A+21,alarm3, 9);
      Write_flash((uchar *)Flash_Segment_A+31,alarm4, 9);
      
      Write_flash((uchar *)Flash_Segment_A+70,alarm11, 9);  //��ʼ��
      Write_flash((uchar *)Flash_Segment_A+80,alarm22, 9);
      Write_flash((uchar *)Flash_Segment_A+90,alarm33, 9);
      Write_flash((uchar *)Flash_Segment_A+100,alarm44, 9);
    }
     
    if(0xff == Read_Byte(0x00000000))
    {
      Erase_Page(0x00000000);
      while(Read_StatReg()&0x01);
      Write_Enable(); 
      Write_Date(0x0000000,SNum,10);

      Erase_Page(0x00002000);
      while(Read_StatReg()&0x01);
      Write_Enable(); 
      Write_Date(0x00002000,SNum,10); 

      Erase_Page(0x00004000);
      while(Read_StatReg()&0x01);
      Write_Enable(); 
      Write_Date(0x00004000,SNum,10); 
    } 
    

  P4DIR |= BIT0;
  P4DIR |= BIT1;
  P4DIR |= BIT2;//�����ã��ȴ򿪵�Դ
  
  //P4OUT |=  BIT0; //����
  //P4OUT |=  BIT1; //LED
  //P4OUT |=  BIT2; //������
  
  P4OUT &=  ~BIT0; //����
  P4OUT &=  ~BIT1; //LED
  P4OUT &=  ~BIT2; //������
  
  _EINT();
 
 
  // __bis_SR_register(LPM4_bits);
  //LPM4;
  //LPM0;
  //LPM0;
  //LPM0_EXIT;
   
  
  
  
  WDTCTL = WDTPW | WDTCNTCL | WDT_ARST_1000;
  while(1) 
  {
    WDTCTL = (WDTCTL & 0xff) | WDTPW | WDTCNTCL;//ι��
    
    Window();
    Keyboard(); 
    RequestDetectorData();    
    USART0_RECEIVE();
    baojing();
    peak_value();
    
    alarm_start();
    alarm_end();
    Reset_w25X16();
    
    guanji();
    
    
    
    //if(FF)   ADC12CTL0 |= ADC12SC; // ��ADת��  5S
    
    
    
    
     
    
    ceshi2[0]=Read_Byte(0x00002000);
 
  }
 
}




#pragma vector=PORT2_VECTOR
  __interrupt void Port_2(void)
  
  {   
    LPM0_EXIT;
    //Window();
    
    P2IFG &= ~BIT0; //�����־λ
    
  }





void kaiji()
{
  unsigned char i,j,k;
  unsigned char temp=5;
  
  for(i=0;i<100;i++)
  { 
    for(j=0;j<200;j++)
    { 
      for(k=0;k<100;k++);
    }
  }
  for(i=0;i<100;i++)
  { 
    for(j=0;j<200;j++)
    { 
      for(k=0;k<100;k++);
    }
  }

  P4DIR |= BIT7;  
  P4OUT |= BIT7;  //��������
  for(i=0;i<100;i++)
  { 
    for(j=0;j<200;j++)
    { 
      for(k=0;k<100;k++);
    }
  }
  InitOled();
  OLED_ShowCHinese(charater,2,32,47,1);    //���ڿ���
  OLED_ShowCHinese(charater,2,48,48,1);
  OLED_ShowCHinese(charater,2,64,49,1);
  OLED_ShowCHinese(charater,2,80,50,1);


  for(temp=5;temp>0;temp--)
  {
    sprintf((char*)str,"%d",temp );
    const fontSize size = {0x0810};  //��   ��
    const position pos = {0x283c};   //λ�� X Y
    OledDisplayStr((const char*)str , size, pos ); 
    
    for(i=0;i<100;i++)
    { 
      for(j=0;j<200;j++)
      { 
        for(k=0;k<100;k++);
      }
    }  
  }  
  
  
  OledClearScreen();        //����
}

void guanji()
{
  unsigned char i,j,k,GJflag=0;
  static unsigned char p=0;
  static unsigned char OFF=0;  //�ػ���־
  
    P5DIR &= ~BIT4;      //   p5.4 key_B ����
    P5REN |= BIT4;       //   ��������
    
    //flag = P5IN&BIT4;
    if(GJflag == 0){
      if(P5IN&BIT4)
      {
        delay_ms(20);
        if(P5IN&BIT4) GJflag = 1;
        else GJflag =0;
      }
      else GJflag = 0;
    }

 if(GJflag)
 { 
   p++;
   if (p==100)
   {
     p = 0;
     OledClearScreen();        //����

     OLED_ShowCHinese(charater,3,32,43,1);     //�Ƿ�ػ�
     OLED_ShowCHinese(charater,3,48,44,1);       
     OLED_ShowCHinese(charater,3,64,51,1);     
     OLED_ShowCHinese(charater,3,80,50,1);     
     
     
     while((Key_guanji!='1')&&(Key_guanji!='4')) 
     {
       Key_guanji = Keyboard_Hardware();  
     } 
     
     
     switch( Key_guanji )
     {  
        case '1': 
            Key_guanji =0;
            OledClearScreen();        //����
            desktops.window = window_desktop;
            break;
        case '2':  
            break;
        case '3': 
            break;
        case '4':      
            OFF=1;
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

    
   if(OFF==1)  
   {
     unsigned char temp;
     uchar str[20]={'\0'};
     
     sprintf((char*)str,"%8.3f",DT.Accumulation1 );
     EraseFlash(Flash_Segment_B); 
     Write_flash((uchar*)Flash_Segment_B, str, 10);
     
     OledClearScreen();        //����
     
     OLED_ShowCHinese(charater,2,32,47,1);    //���ڹػ�
     OLED_ShowCHinese(charater,2,48,48,1); 
     OLED_ShowCHinese(charater,2,64,51,1);     
     OLED_ShowCHinese(charater,2,80,50,1);
     
     for(temp=5;temp>0;temp--)
     {
       sprintf((char*)str,"%d",temp );
       const fontSize size = {0x0810};  //��   ��
       const position pos = {0x283c};   //λ�� X Y
       OledDisplayStr((const char*)str , size, pos ); 
    
       for(i=0;i<100;i++)
       { 
         for(j=0;j<200;j++)
         { 
           for(k=0;k<100;k++);
         }
       }  
     }  
     OledClearScreen();        //����
     P4OUT &= ~BIT7;  //�ػ�
     
   }  
     
  }
 }
 
 else p = 0;
 
}



void baojing()
{
  //extern struct Detector_ DT;
  
   float ala1,ala2,ala3,ala4;
   bool flag1,flag2,flag3,flag4;
  
   uchar baojing1[10];   //�����ʸ߱���ֵ
   uchar baojing2[10];   //�����ʸ߱���ֵ
   uchar baojing3[10];   //�ۻ������߱���ֵ
   uchar baojing4[10];   //�ۻ������ͱ���ֵ
   

   Read_flash((uchar *)(Flash_Segment_A)+70, baojing1,9);  //��ȡ�����ʸ߱���ֵ
   Read_flash((uchar *)(Flash_Segment_A)+80, baojing2,9);  //��ȡ�����ʵͱ���ֵ
   Read_flash((uchar *)(Flash_Segment_A)+90, baojing3,9);  //��ȡ�ۻ������߱���ֵ
   Read_flash((uchar *)(Flash_Segment_A)+100,baojing4,9);  //��ȡ�ۻ������ͱ���ֵ
  
    ala1 = atof(baojing1);
    ala2 = atof(baojing2);
    ala3 = atof(baojing3);
    ala4 = atof(baojing4);

 //****** DT.Accumulation1*******   ��ǰ�ۻ�����
 //****** DT.Value3***** ********   ��ǰ������
    
    if(ala1 <= DT.Value3)   //�����ʸ߱�
    {
      flag1 = true;
      //��������źţ���¼����ֵ�Ͷ�Ӧʱ��
      
      if (F_Second)  
      {
        P4OUT |=  BIT2;
        P4OUT |=  BIT1; //��LED
      }
      if (!F_Second)   
      {
        P4OUT &=  ~BIT2;
        P4OUT &=  ~BIT1; //��LED
      }
    }
    else flag1 = false;
    
    
    
    if((DT.Value3 >= ala2)&&( DT.Value3 <= ala1))   //�����ʵͱ�  
    {
      flag2 = true;
      if (Flashing)  
      { 
        P4OUT |=  BIT2;
        P4OUT |=  BIT1; //��LED
      }
      if (!Flashing) 
      { 
        P4OUT &=  ~BIT2;
        P4OUT &=  ~BIT1; //��LED
      }
    }
    else flag2 = false;
    
    
    if(ala3 <= DT.Accumulation1)   //�ۻ������߱�
    {
      flag3 = true;
      if (F_Second)   
      {
        P4OUT |=  BIT2;
        P4OUT |=  BIT1; //��LED
      }
      if (!F_Second) 
      { 
        P4OUT &=  ~BIT2;
        P4OUT &=  ~BIT1; //��LED
      }
    
    }
    
    else flag3 = false;
    

    if((DT.Accumulation1 >= ala4) &&(DT.Accumulation1 <= ala3))   //�ۻ������ͱ�  
    {
      flag4 = true;
      if (F_Second)
      {
        P4OUT |=  BIT2;
        P4OUT |=  BIT1; //��LED
      }
      if (!F_Second)  
      { 
        P4OUT &=  ~BIT2; 
        P4OUT &=  ~BIT1; //��LED
      }
    }
    else flag4 = false;
    
   if(!(flag1||flag2||flag3||flag4))  
   {
     P4OUT &=  ~BIT2; //�رձ���
     P4OUT &=  ~BIT1; //��LED
   }
}



void peak_value()   //��ֵ
{
  static float  max = 0;
  unsigned char str_peak[15]={0};
  unsigned char peak_time[6]={'/0'}; 
  unsigned char data[10]={0};
  
  
  union Un
  {
    float Max;
    unsigned  char  Char[5];
   };
  
  union Un T;

  if(his_clear)
  { 
    his_clear =false;
    max = 0;      //����
  }
   
  if(DT.Value3>10000) DT.Value3 = 0;
  if(DT.Value3>max)  
  { 
     max = DT.Value3;
     T.Max = max;  //��������
          
     ReadRX8025SAClockData(peak_time);
          
     peak_time[0] = ( ( peak_time[0]/16 )*10 +(peak_time[0]&0xf));//BCD��ת��ʮ����
     peak_time[1] = ( ( peak_time[1]/16 )*10 +(peak_time[1]&0xf));
     peak_time[2] = ( ( peak_time[2]/16 )*10 +(peak_time[2]&0xf));
     peak_time[3] = ( ( peak_time[3]/16 )*10 +(peak_time[3]&0xf));
     peak_time[4] = ( ( peak_time[4]/16 )*10 +(peak_time[4]&0xf));
     peak_time[5] = ( ( peak_time[5]/16 )*10 +(peak_time[5]&0xf));
  
      
    Erase_Page(0x00000000);
    while(Read_StatReg()&0x01);
    Write_Enable(); 
    Write_Date(0x00000000,peak_time,6);  //дʱ��
    
    while(Read_StatReg()&0x01);
    Write_Enable();
    Write_Date(0x00000006,T.Char,4);     //д��ֵ
  }

}



void alarm_start()     //   ���뱨��
{
  static float  start=0;
  float  low_alarm;
  unsigned  char  baojing[10];
  unsigned  char  start_time[6]={'/0'};
   
  union Un
  {
     float BJ_start;
     unsigned  char  BJ_kaishi[5];
   };
  union Un S;
   

  Read_flash((uchar *)(Flash_Segment_A)+80, baojing,9);  //��ȡ�����ʵͱ���ֵ
  low_alarm = atof(baojing); 

  if(BJ_flag_end)
  {
    
    if(DT.Value3 >= low_alarm)  //�����ʵͱ� 
    {
      BJ_flag_end = 0;
      BJ_flag_start = 1;
      
      start = DT.Value3;
      S.BJ_start = start;  //��������
          
      ReadRX8025SAClockData(start_time);
          
      start_time[0] = ( ( start_time[0]/16 )*10 +(start_time[0]&0xf));//BCD��ת��ʮ����
      start_time[1] = ( ( start_time[1]/16 )*10 +(start_time[1]&0xf));
      start_time[2] = ( ( start_time[2]/16 )*10 +(start_time[2]&0xf));
      start_time[3] = ( ( start_time[3]/16 )*10 +(start_time[3]&0xf));
      start_time[4] = ( ( start_time[4]/16 )*10 +(start_time[4]&0xf));
      start_time[5] = ( ( start_time[5]/16 )*10 +(start_time[5]&0xf));
     

      Erase_Page(0x00002000);
      while(Read_StatReg()&0x01);
      Write_Enable(); 
      Write_Date(0x00002000,start_time,6);  //дʱ��
    
      while(Read_StatReg()&0x01);
      Write_Enable();
      Write_Date(0x00002006,S.BJ_kaishi,4);     
  
    }
  }
}




void alarm_end()    //      �˳�����
{
  static float  end=0;
  float  low_alarm;
  unsigned  char  baojing[10];
  unsigned  char  end_time[6]={'/0'};

  union Un
  {
     float BJ_end;
     unsigned  char  BJ_jieshu[5];
   };
  union Un E;
   
  
  Read_flash((uchar *)(Flash_Segment_A)+80, baojing,9);  //��ȡ�����ʵͱ���ֵ
  low_alarm = atof(baojing); 
  

  if(BJ_flag_start)
  {
   if(DT.Value3 <= low_alarm)  //�����ʵͱ� 
   {
     BJ_flag_start = 0;
     BJ_flag_end = 1;
     
     end = DT.Value3;
     E.BJ_end = end;  //��������
          
     ReadRX8025SAClockData(end_time);
          
     end_time[0] = ( ( end_time[0]/16 )*10 +(end_time[0]&0xf));//BCD��ת��ʮ����
     end_time[1] = ( ( end_time[1]/16 )*10 +(end_time[1]&0xf));
     end_time[2] = ( ( end_time[2]/16 )*10 +(end_time[2]&0xf));
     end_time[3] = ( ( end_time[3]/16 )*10 +(end_time[3]&0xf));
     end_time[4] = ( ( end_time[4]/16 )*10 +(end_time[4]&0xf));
     end_time[5] = ( ( end_time[5]/16 )*10 +(end_time[5]&0xf));
     

    Erase_Page(0x00004000);
    while(Read_StatReg()&0x01);
    Write_Enable(); 
    Write_Date(0x00004000,end_time,6);        //дʱ��
    
    while(Read_StatReg()&0x01);
    Write_Enable();
    Write_Date(0x00004006,E.BJ_jieshu,4);     
   }  
  }
}


void Reset_w25X16()
{
  if(his_reset)
  {
     his_reset = false;
     
     Erase_Page(0x00002000);
     while(Read_StatReg()&0x01);
     Write_Enable(); 
     Write_Date(0x00002000,SNum,10); 

     Erase_Page(0x00004000);
     while(Read_StatReg()&0x01);
     Write_Enable(); 
     Write_Date(0x00004000,SNum,10); 
  
  }
}