#ifndef __FLASH_H
#define __FLASH_H
//#include <msp430.h>
//#include <MSP430x14x.h> 

#define uchar unsigned char




//FLASH数据存放区
#define   Flash_Segment_A           0X1000     // 
#define   Flash_Segment_B           0X1080     //

unsigned char ReadFlash(unsigned int address);
void EraseFlash(unsigned int address);
void WriteFlash0(unsigned int address);
void ACopyB(unsigned int address);
void WriteFlash(unsigned int address,unsigned char  da);
void Flash_erase(unsigned int *pc_word);
void Read_flash(unsigned char *pc_byte, unsigned char *array,unsigned char amount);
void Write_flash(unsigned char *pc_byte,unsigned char *array, unsigned char amount);
int WritePasswd( unsigned int address,unsigned char index, uchar *pw);
int ReadPasswd(unsigned int address, unsigned char index, uchar *pw );
void WritePassWord(void);
void ReadPassWord(void);
//void FlashInt(void);


//uchar str_len( uchar * str );

void Copy(unsigned int address1,unsigned int L,unsigned int address2);


#endif

