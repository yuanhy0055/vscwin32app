#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "UART.h"
#include "time.h"
#include "detector.h"
#include "windows.h"
#include "flash.h"





unsigned char DT_Connection_CNT = DT_OutTimer;
unsigned char DT_Status;
bool AddF = false;
struct Detector_  DT;
unsigned int GZFlag = 0;
unsigned int yi = 0;

extern void WriteHexToUART(UART_MODULE id, const char* hex, int size);


// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************
void init_Detector(void)
{

    DT.ID = 1;
    DT.type = GammaRay;
    DT.Temperature = 0;
    DT.unit = 3;
    DT.Status = 0;
    DT.Value0 = 0;
    DT.Value1 = 0;
    DT.Value2 = 0;
    DT.Value3 = 0;
    DT.Accumulation1 = 0;
    DT.Accumulation2 = 0;
    DT.Alarm1 = 0;
    DT.Alarm2 = 0;
    DT.Sensitivity1 = 1;
    DT.Sensitivity2 = 1;
    DT.DeadTime = 0.000001;
    DT.Background = 0;
}


void RequestDetectorData(void)    //发送指令  取数据  
{
    if(( F_Acq )&&(model==0))//定时1s发一次，现在定时器不准
    {
        F_Acq = false;

        unsigned char str[10] = {'\0'};
        str[0] = 'R';
        str[1] = *((unsigned char*)&DT.ID);
        str[2] = *((unsigned char*)&DT.ID + 1);
        if( !AddF || !DT_Connection_CNT )
            str[3] = 'A';
        else //if( !(DT.unit ^ 3) )
            str[3] = 'R';
        //else
            //str[3] = 'C';
        WriteHexToUART(UART0, (const char*)str, 4);
    }
}



void USART0_RECEIVE(void)   // USART0  数据处理
{
    //unsigned short ii = 0;
    //unsigned char  CheckCode = 0;
    //unsigned char  Function = 0;
    //unsigned char  Dnumber = 0;
    //unsigned short address_t = 0;
    //unsigned int yi = 0;
    
    if(Uart0Receive.Finish)
      
    {
        if(Uart0Receive.data[3] == 'A')
        {
            AddF = true;
            DT_Connection_CNT = DT_OutTimer;
            *((unsigned char*)&DT.ID) = Uart0Receive.data[1];
            *((unsigned char*)&DT.ID+1) = Uart0Receive.data[2];
        }
        else
        {
            DT_Connection_CNT = DT_OutTimer;
            unsigned char str[20] = {'\0'};
            strxfrm((char*)str, (const char*)&Uart0Receive.data[3], 9);
            // DT.Value3 = ( !(DT.unit ^ 3) )
            //        ? atof((const char*)str)*1000000 : atof((const char*)str);
            DT.Value3 =  atof((const char*)str)*1000000; 
            if(DT.Value3 == 0)//连续3个数为0报故障
            {
              yi++;
              if(yi>=3) GZFlag = 1;
              if(yi>=65535) yi=3;
            }
            else
            {
              GZFlag = 0;
              yi = 0;
            }
        }
       
       if ( !(DT.unit ^ 3) )
       {
         uchar str[20]={'\0'};
         static uchar ii=true;
         if(ii)
         {
           ii=false;
           uchar str1[10]={'\0'};

           Read_flash((uchar *)(Flash_Segment_B), str1,10);
          
           DT.Accumulation1 = atof(str1);
         }
         
         //DT.Accumulation1 += 1;
         
         //if(DT.Accumulation1<10000)
         DT.Accumulation1 += (DT.Value3*(1.0/3600));   //uGy 剂量率
         //DT.Accumulation1 =  DT.Accumulation1/3600;
         //sprintf((char*)str,"%f", DT.Accumulation1);
         //sprintf((char*)str,"%8.3f",DT.Accumulation1 );
        
         //EraseFlash(Flash_Segment_B); 
         //Write_flash((uchar*)Flash_Segment_B, str, 10);         
       }
       //else
        //DT.Accumulation2 += 2;
        //DT.Accumulation2 += DT.Value3;   //count 计数率
       
       Uart0Receive.Finish = false;  
    }
}