#include <msp430.h>
#include "flash.h"

#define uchar unsigned char

uchar FlashValue;    //向FLASH中写数据的临时变量
extern uchar Number;


/********读取EEPROM ,入口为地址***********/

unsigned char ReadFlash(unsigned int address)
{    
   // WDTCTL = WDTPW + WDTHOLD; 
    unsigned char  *p=(unsigned char  *)(address);
       
   // WDTCTL = WDT_ARST_1000; //1s看门狗功能启动
    return (*p);
     
}


/******擦除EEPROM ,入口地址为要擦除的单元*********/

void EraseFlash(unsigned int address)
{
    unsigned char  *p;
   // WDTCTL = WDTPW + WDTHOLD;                //停止看门狗  口令+停止位
    _DINT();
    p=(unsigned char  *)(address);
    while(FCTL3&BUSY);
   // _DINT();
    FCTL1=0XA500+ERASE;            //擦除一段
    FCTL3=0XA500;                            //解锁
    *p=0;
    while(FCTL3&BUSY);
    _EINT();
    //WDTCTL = WDT_ARST_1000; //1s看门狗功能启动

}





void Flash_erase(unsigned int *pc_word)  //FLASH段擦除
{
  WDTCTL = WDTPW + WDTHOLD;                //停止看门狗  口令+停止位
  _DINT();
 
 while(FCTL3 & BUSY);                       //如果处于忙状态，则等待
 FCTL3 = FWKEY ;                          //清出LOCK标志，解锁
 FCTL1 = FWKEY + ERASE ;                 //允许段擦除
 *pc_word = 0;                         //擦除..擦除..
 while(FCTL3 & BUSY);  
 FCTL3 = FWKEY + LOCK ;             //加锁
 
 _EINT();
 //WDTCTL = WDT_ARST_1000;          //1s看门狗功能启动
}


void WriteFlash0(unsigned int address)   //功能：是数据写入到指定的地址中
{
    unsigned char *p;
    p=(unsigned char *)(address);       //写入当前内容
    WDTCTL = WDTPW + WDTHOLD;        
    while(FCTL3&BUSY);
    _DINT();
    FCTL3=0XA500;                      //开锁
    FCTL1=0XA500+WRT;
    *p=Number;
    while(FCTL3&BUSY);
    FCTL1=0XA500;
    FCTL3=0XA500+LOCK;               //加锁，避免误写
    //WDTCTL = WDT_ARST_1000;        //1s看门狗功能启动
    _EINT();
     
}

//功能：是先将段A的内容拷贝到段B中，将段A擦除，然后再将段B的内容拷到段A中
void ACopyB(unsigned int address)
{
   unsigned char i; 
   EraseFlash(Flash_Segment_B);     //擦除段B
   WDTCTL = WDTPW + WDTHOLD;
   while(FCTL3&BUSY);
   for(i=0;i<128;i++)      //写段B
   {
      WDTCTL = WDTPW + WDTHOLD;
      FlashValue=ReadFlash(Flash_Segment_A+i);
      WriteFlash0(Flash_Segment_B+i);
      //WDTCTL = WDT_ARST_1000;
     
    }
   EraseFlash(Flash_Segment_A);      //擦段A
   WDTCTL = WDTPW + WDTHOLD;
   while(FCTL3&BUSY);
   for(i=0;i<128;i++)     //写段A
   {
    WDTCTL = WDTPW + WDTHOLD;
      FlashValue=ReadFlash(Flash_Segment_B+i); //1s看门狗功能启动
      if((i|Flash_Segment_A)==address) continue;  //当前地址不写
      WriteFlash0(Flash_Segment_A+i);
       //WDTCTL = WDT_ARST_1000; 
   }
}



void Copy(unsigned int address1,unsigned int L,unsigned int address2)  //将address1到address1+L之间的内容复制到address3中
{    
     unsigned char i,temp;
     _DINT();
     EraseFlash(Flash_Segment_A+64);// address2必须在Flash_Segment_A的第二段
     while(FCTL3&BUSY);
     for(i=0;i<L;i++)     
     {
       unsigned char *p;
       
       temp = ReadFlash(address1+i);     
       p=(unsigned char *)(address2+i);         
       while(FCTL3&BUSY);
       _DINT();
       FCTL3=0XA500;                     
       FCTL1=0XA500+WRT;
       *p=temp;
       while(FCTL3&BUSY);
       FCTL1=0XA500;
       FCTL3=0XA500+LOCK;               
     
       _EINT();   
     }
}


/*Write EEProm,当EE中不为空时，调用ACopyB后再写*/
void WriteFlash(unsigned int address,unsigned char  da)
{
   //WDTCTL = WDTPW + WDTHOLD;
   ACopyB(address);
   FlashValue=da;
   WriteFlash0(address);
   //WriteFlash0(address+0x80);    //同时数据也保存到FLASH_B中
   //WDTCTL = WDT_ARST_1000;       //1s看门狗功能启动 */
}



//******************************************************************************
//向FLASH信息区读出指定数量的字节数据
//unsigned int*pc_word :信息区数据指针
//unsigned char *array :读出数据存放数据数组,8位长
//unsigned char amount :读操的数量,范围0-127
//*******************************************************************************
void Read_flash(unsigned char *pc_byte, unsigned char *array,unsigned char amount)
{ 
  unsigned char i;
  WDTCTL = WDTPW + WDTHOLD;                //停止看门狗  口令+停止位
   _DINT();
   
   if(amount<=128)
  {
    for(i=0;i<amount;i++,pc_byte ++)
    {
      *array = *pc_byte; //读数据，读数据时，flash地址自动加 1 
      array++;          //接收缓冲区地址加 1   
    }
  }
  
  _EINT();
  //WDTCTL = WDT_ARST_1000; //1s看门狗功能启动
}
 





void Write_flash(unsigned char *pc_byte,unsigned char *array, unsigned char amount)
{ 
 unsigned char i;
 WDTCTL = WDTPW + WDTHOLD;            //停止看门狗  口令+停止位
 _DINT();
 
 //ACopyB(Flash_Segment_A);
 while(FCTL3 & BUSY);                 //如果处于忙状态，则等待
 FCTL3 = FWKEY ;                     //清出LOCK标志
 FCTL1 = FWKEY  + WRT ;              //写操作，块编程，+ BLKWRT;
 for(i=0;i<amount;i++)
  {   
    while(FCTL3 & BUSY);                    //如果处于忙状态，则等待
    *pc_byte = *array;
     array++;                     //发送缓冲区地址加 1  
     pc_byte++;                   //写flash时，地址人为加 1
  }
 FCTL1 = FWKEY;           //写操作完成，清除编程允许位 WRT，BLKWRT 
 FCTL3 = FWKEY + LOCK;
 _EINT();
 //WDTCTL = WDT_ARST_1000; //1s看门狗功能启动 
}



void WritePassWord(void)
{
    WriteFlash(Flash_Segment_A,0x55);//0X1000:Flash初始化标志0X55
    WriteFlash(Flash_Segment_A+1,8); //0X1001:常用码长度
    WriteFlash(Flash_Segment_A+2,8); //0X1002:管理码长度
    WriteFlash(Flash_Segment_A+3,1); //0X1003:系统模式
    WritePasswd( Flash_Segment_A,1, "12345678            " ); //重写常用码 
    WritePasswd( Flash_Segment_A,2, "88888888            " ); //重写管理码 
    WritePasswd( Flash_Segment_A,3, "01234567899876543210" ); //重写最高管理码
    
}


















//******************************************************************************
//向FLASH信息区写入指定数量的字节数据
//unsigned char *pc_byte 信息区数据指针
//unsigned char *array :读出数据存放数据数组,8位长
//unsigned char amount :读操的数量,范围0-127
//******************************************************************************
//2014.7.09，设置卡设置时写flash程序
/*
void Write_flash(unsigned char *pc_byte,unsigned char *array, unsigned char amount)
{
        uchar temp_array[24]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	uchar length;
        unsigned int i;
	uchar j=0;

        length = amount;
        for ( i = 0; i <length; i++ ) 
  	{
                //WDTCTL = WDT_ARST_1000;
    		temp_array[i] = array[j];
    		j++;
  	}
        j=0;
        for(i=Flash_Segment_A+4*24;i<=Flash_Segment_A+4*24+23;i++)   
        {
          
             WDTCTL = WDTPW + WDTHOLD; 
             WriteFlash(i,temp_array[j]);
             j++;
             //WDTCTL = WDT_ARST_1000;
        }
  	//return 1;
}



//********************向信息区指定的起始地址，写入指定ID号的 密码*********************
//address:   信息区的地址
//index:    密码的ID索引号
//*pw :     要写入到信息区的数据 
int WritePasswd( unsigned int address,unsigned char index, uchar *pw) 
{
        uchar array[24]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	uchar length;
        unsigned int i;
	uchar j=0;
       	
        length = str_len(pw);
  	array[0]= index;
        for ( i = 1; i <=length; i++ ) 
  	{
                
    		array[i] = pw[j];
    		j++;
  	}
        j=0;
        for(i=address+index*24;i<=address+index*24+23;i++)   
        {
          
             WDTCTL = WDTPW + WDTHOLD; 
             WriteFlash(i,array[j]);
             j++;
             //WDTCTL = WDT_ARST_1000;
        }
  	return 1;
}

//*********************从指定地址中读出指定ID号的密码----------------
//address:  信息区的地址
//index:    指定的索引号
//*pw:      读出的数据放入这个数组中

int ReadPasswd(unsigned int address, uchar index, uchar *pw ) 
{
	
  	uchar j;
  	unsigned int  i;
        j=0;
        for(i=address+index*24;i<address+index*24+24;i++)   //以24个数据为单位读出
        {
          
          pw[j]=ReadFlash(i+1);
          j++;
          //WDTCTL = WDT_ARST_1000; //1s看门狗功能启动 
        }
  	
  	return 1;
}


void WritePassWord(void)
{
    WriteFlash(Flash_Segment_A,0x55);//0X1000:Flash初始化标志0X55
    WriteFlash(Flash_Segment_A+1,8); //0X1001:常用码长度
    WriteFlash(Flash_Segment_A+2,8); //0X1002:管理码长度
    WriteFlash(Flash_Segment_A+3,1); //0X1003:系统模式
    WritePasswd( Flash_Segment_A,1, "12345678            " ); //重写常用码 
    WritePasswd( Flash_Segment_A,2, "88888888            " ); //重写管理码 
    WritePasswd( Flash_Segment_A,3, "01234567899876543210" ); //重写最高管理码
    
}

*/
