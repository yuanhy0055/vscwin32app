#include <msp430.h>
#include "flash.h"

#define uchar unsigned char

uchar FlashValue;    //��FLASH��д���ݵ���ʱ����
extern uchar Number;


/********��ȡEEPROM ,���Ϊ��ַ***********/

unsigned char ReadFlash(unsigned int address)
{    
   // WDTCTL = WDTPW + WDTHOLD; 
    unsigned char  *p=(unsigned char  *)(address);
       
   // WDTCTL = WDT_ARST_1000; //1s���Ź���������
    return (*p);
     
}


/******����EEPROM ,��ڵ�ַΪҪ�����ĵ�Ԫ*********/

void EraseFlash(unsigned int address)
{
    unsigned char  *p;
   // WDTCTL = WDTPW + WDTHOLD;                //ֹͣ���Ź�  ����+ֹͣλ
    _DINT();
    p=(unsigned char  *)(address);
    while(FCTL3&BUSY);
   // _DINT();
    FCTL1=0XA500+ERASE;            //����һ��
    FCTL3=0XA500;                            //����
    *p=0;
    while(FCTL3&BUSY);
    _EINT();
    //WDTCTL = WDT_ARST_1000; //1s���Ź���������

}





void Flash_erase(unsigned int *pc_word)  //FLASH�β���
{
  WDTCTL = WDTPW + WDTHOLD;                //ֹͣ���Ź�  ����+ֹͣλ
  _DINT();
 
 while(FCTL3 & BUSY);                       //�������æ״̬����ȴ�
 FCTL3 = FWKEY ;                          //���LOCK��־������
 FCTL1 = FWKEY + ERASE ;                 //����β���
 *pc_word = 0;                         //����..����..
 while(FCTL3 & BUSY);  
 FCTL3 = FWKEY + LOCK ;             //����
 
 _EINT();
 //WDTCTL = WDT_ARST_1000;          //1s���Ź���������
}


void WriteFlash0(unsigned int address)   //���ܣ�������д�뵽ָ���ĵ�ַ��
{
    unsigned char *p;
    p=(unsigned char *)(address);       //д�뵱ǰ����
    WDTCTL = WDTPW + WDTHOLD;        
    while(FCTL3&BUSY);
    _DINT();
    FCTL3=0XA500;                      //����
    FCTL1=0XA500+WRT;
    *p=Number;
    while(FCTL3&BUSY);
    FCTL1=0XA500;
    FCTL3=0XA500+LOCK;               //������������д
    //WDTCTL = WDT_ARST_1000;        //1s���Ź���������
    _EINT();
     
}

//���ܣ����Ƚ���A�����ݿ�������B�У�����A������Ȼ���ٽ���B�����ݿ�����A��
void ACopyB(unsigned int address)
{
   unsigned char i; 
   EraseFlash(Flash_Segment_B);     //������B
   WDTCTL = WDTPW + WDTHOLD;
   while(FCTL3&BUSY);
   for(i=0;i<128;i++)      //д��B
   {
      WDTCTL = WDTPW + WDTHOLD;
      FlashValue=ReadFlash(Flash_Segment_A+i);
      WriteFlash0(Flash_Segment_B+i);
      //WDTCTL = WDT_ARST_1000;
     
    }
   EraseFlash(Flash_Segment_A);      //����A
   WDTCTL = WDTPW + WDTHOLD;
   while(FCTL3&BUSY);
   for(i=0;i<128;i++)     //д��A
   {
    WDTCTL = WDTPW + WDTHOLD;
      FlashValue=ReadFlash(Flash_Segment_B+i); //1s���Ź���������
      if((i|Flash_Segment_A)==address) continue;  //��ǰ��ַ��д
      WriteFlash0(Flash_Segment_A+i);
       //WDTCTL = WDT_ARST_1000; 
   }
}



void Copy(unsigned int address1,unsigned int L,unsigned int address2)  //��address1��address1+L֮������ݸ��Ƶ�address3��
{    
     unsigned char i,temp;
     _DINT();
     EraseFlash(Flash_Segment_A+64);// address2������Flash_Segment_A�ĵڶ���
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


/*Write EEProm,��EE�в�Ϊ��ʱ������ACopyB����д*/
void WriteFlash(unsigned int address,unsigned char  da)
{
   //WDTCTL = WDTPW + WDTHOLD;
   ACopyB(address);
   FlashValue=da;
   WriteFlash0(address);
   //WriteFlash0(address+0x80);    //ͬʱ����Ҳ���浽FLASH_B��
   //WDTCTL = WDT_ARST_1000;       //1s���Ź��������� */
}



//******************************************************************************
//��FLASH��Ϣ������ָ���������ֽ�����
//unsigned int*pc_word :��Ϣ������ָ��
//unsigned char *array :�������ݴ����������,8λ��
//unsigned char amount :���ٵ�����,��Χ0-127
//*******************************************************************************
void Read_flash(unsigned char *pc_byte, unsigned char *array,unsigned char amount)
{ 
  unsigned char i;
  WDTCTL = WDTPW + WDTHOLD;                //ֹͣ���Ź�  ����+ֹͣλ
   _DINT();
   
   if(amount<=128)
  {
    for(i=0;i<amount;i++,pc_byte ++)
    {
      *array = *pc_byte; //�����ݣ�������ʱ��flash��ַ�Զ��� 1 
      array++;          //���ջ�������ַ�� 1   
    }
  }
  
  _EINT();
  //WDTCTL = WDT_ARST_1000; //1s���Ź���������
}
 





void Write_flash(unsigned char *pc_byte,unsigned char *array, unsigned char amount)
{ 
 unsigned char i;
 WDTCTL = WDTPW + WDTHOLD;            //ֹͣ���Ź�  ����+ֹͣλ
 _DINT();
 
 //ACopyB(Flash_Segment_A);
 while(FCTL3 & BUSY);                 //�������æ״̬����ȴ�
 FCTL3 = FWKEY ;                     //���LOCK��־
 FCTL1 = FWKEY  + WRT ;              //д���������̣�+ BLKWRT;
 for(i=0;i<amount;i++)
  {   
    while(FCTL3 & BUSY);                    //�������æ״̬����ȴ�
    *pc_byte = *array;
     array++;                     //���ͻ�������ַ�� 1  
     pc_byte++;                   //дflashʱ����ַ��Ϊ�� 1
  }
 FCTL1 = FWKEY;           //д������ɣ�����������λ WRT��BLKWRT 
 FCTL3 = FWKEY + LOCK;
 _EINT();
 //WDTCTL = WDT_ARST_1000; //1s���Ź��������� 
}



void WritePassWord(void)
{
    WriteFlash(Flash_Segment_A,0x55);//0X1000:Flash��ʼ����־0X55
    WriteFlash(Flash_Segment_A+1,8); //0X1001:�����볤��
    WriteFlash(Flash_Segment_A+2,8); //0X1002:�����볤��
    WriteFlash(Flash_Segment_A+3,1); //0X1003:ϵͳģʽ
    WritePasswd( Flash_Segment_A,1, "12345678            " ); //��д������ 
    WritePasswd( Flash_Segment_A,2, "88888888            " ); //��д������ 
    WritePasswd( Flash_Segment_A,3, "01234567899876543210" ); //��д��߹�����
    
}


















//******************************************************************************
//��FLASH��Ϣ��д��ָ���������ֽ�����
//unsigned char *pc_byte ��Ϣ������ָ��
//unsigned char *array :�������ݴ����������,8λ��
//unsigned char amount :���ٵ�����,��Χ0-127
//******************************************************************************
//2014.7.09�����ÿ�����ʱдflash����
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



//********************����Ϣ��ָ������ʼ��ַ��д��ָ��ID�ŵ� ����*********************
//address:   ��Ϣ���ĵ�ַ
//index:    �����ID������
//*pw :     Ҫд�뵽��Ϣ�������� 
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

//*********************��ָ����ַ�ж���ָ��ID�ŵ�����----------------
//address:  ��Ϣ���ĵ�ַ
//index:    ָ����������
//*pw:      ���������ݷ������������

int ReadPasswd(unsigned int address, uchar index, uchar *pw ) 
{
	
  	uchar j;
  	unsigned int  i;
        j=0;
        for(i=address+index*24;i<address+index*24+24;i++)   //��24������Ϊ��λ����
        {
          
          pw[j]=ReadFlash(i+1);
          j++;
          //WDTCTL = WDT_ARST_1000; //1s���Ź��������� 
        }
  	
  	return 1;
}


void WritePassWord(void)
{
    WriteFlash(Flash_Segment_A,0x55);//0X1000:Flash��ʼ����־0X55
    WriteFlash(Flash_Segment_A+1,8); //0X1001:�����볤��
    WriteFlash(Flash_Segment_A+2,8); //0X1002:�����볤��
    WriteFlash(Flash_Segment_A+3,1); //0X1003:ϵͳģʽ
    WritePasswd( Flash_Segment_A,1, "12345678            " ); //��д������ 
    WritePasswd( Flash_Segment_A,2, "88888888            " ); //��д������ 
    WritePasswd( Flash_Segment_A,3, "01234567899876543210" ); //��д��߹�����
    
}

*/
