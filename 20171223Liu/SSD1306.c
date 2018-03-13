
//
#include <msp430.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Font.h"
#include "SSD1306.h"


#define Uint8 unsigned int
#define U8 unsigned int


unsigned char SSDRAM[9][128];

void InitOled( void )
{
    SSD_PW_DIR(true);
    SSD_RES_DIR(true);//set output
    SSD_CS_DIR(true); 
    SSD_DC_DIR(true);
    SSD_WR_DIR(true);
    SSD_RD_DIR(true);
    SSD_D_DIR(false);//set input

    //
    SSD_RES_O(true);  //高电平
    SSD_CS_O(true);
    SSD_DC_O(true);
    SSD_WR_O(true);
    SSD_RD_O(true);
    //SSD_PW_O(true);
    SSD_PW_O(false);

    //
    SSD_RES_O(false);//reset
    _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
    _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
    _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
    _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
    _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
    SSD_RES_O(true); //normal

    //
    unsigned char command[60] = {
        0xAE,//Set Display Off
        0xD5,//Set Display Clock Divide Ratio/ Oscillator Frequency
        0x80,
        0xA8,//Set Multiplex Ratio
        0x3F,
        0xD3,//SetDisplayOffset
        0x00,
        0x40,//Set Display Start Line
        0x8D,//Charge Pump Setting
        0x14,
        0xA1,//Set Segment Re-map, A0:Column0-->SEG0, A1:Column127-->SEG0   ??
        0xC8,//SetCOMOutputScanDirection C0:COM0-->COMn, C8:-->COMn-->COM0  ??
        0xDA,//Set COM Pins Hardware Configuration  ??
        0x12,
        0x81,//Set Contrast Control for BANK0
        0xCF,
        0xD9,//Set Pre-charge Period
        0xF1,
        0xDB,//Set聽VCOMH聽Deselect聽Level
        0x40,
        0xA4,//A4:GDDRAM, A5:Entire Display ON
        0xA6,//Set Normal(A6)/Inverse(A7) Display

        //0xB0,//Set Page Start Address for Page Addressing Mode
        //0x00,//Set Lower Column Start Address for Page Addressing Mode
        //0x10,//Set Higher Column Start Address for Page Addressing Mode
        0x20,//Horizontal addressing mode
        0x00,
        0x21,//Set Column Address
        0x00,
        0x7F,
        0x22,//Set Page Address
        0x00,
        0x07,

        //0xAF,//Set Display On
    };
    SSD_Write(command, 22+8, SSDcommand);

    //Clear Screen
    unsigned char* pointer = (unsigned char*)SSDRAM;
    memset(pointer, 0x00, sizeof(SSDRAM));  //将s所指向的某一块内存中的前n个 字节的内容全部设置为ch指定的ASCII值
    SSD_Write((const unsigned char*)SSDRAM, 8*128, SSDdata);

    command[0] = 0xAF;//Set Display On
    SSD_Write(command, 1, SSDcommand);

    //logo
    //pointer = (unsigned char*)SSDRAM;
    //memset(pointer, 0x55, sizeof(SSDRAM));
    //SSD_DataBlockWrite( (const unsigned char*)SSDRAM, (0x05+1-0x02)*(0x5F+1-0x20), 0x0502, 0x5F20 );

    //test
    SSD_DataBlockRead( (unsigned char*)SSDRAM, 8*128, 0x0700, 0x7F00 );
    SSD_DataBlockWrite( (const unsigned char*)SSDRAM, 8*128, 0x0700, 0x7F00 );

    //const fontSize fs = {0x0810};
    //const position pos = {0x0D09};
    //OledDisplayStr((const char*)"RuGuo\nZheYang\r\nBuHao!", fs, pos );
}

void SSD_Write( const unsigned char* data, unsigned short len, DCM DC )
{
    SSD_D_DIR(true);

    SSD_CS_O(false);
    SSD_DC_O(DC && 1);
    while(len--)
    {
        SSD_WR_O(false);
        SSD_D_O( *data++ );
        _NOP();_NOP();
        SSD_WR_O(true);
        _NOP();_NOP();
    }
    SSD_DC_O(true);
    SSD_CS_O(true);

    SSD_D_DIR(false);
}

void SSD_Read( unsigned char* data, unsigned short len, DCM DC )
{
    SSD_D_DIR(false);

    SSD_CS_O(false);
    SSD_DC_O(DC && 1);
    while(len--)
    {
        SSD_RD_O(false);
        _NOP();_NOP();
        *data++ = SSD_D_I();
        _NOP();_NOP();
        SSD_RD_O(true);
    }
    SSD_DC_O(true);
    SSD_CS_O(true);
}

void SSD_DataBlockWrite( const unsigned char* data,unsigned short len, unsigned short page, unsigned short column )
{
    unsigned short WRlen = 6;
    unsigned char command[6] = {0x22, 0x00, 0x00, 0x21, 0x00 ,0x00};
    const unsigned char* pointer = command;

    command[1] = *((unsigned char*)&page);
    command[2] = *((unsigned char*)&page + 1);
    command[4] = *((unsigned char*)&column);
    command[5] = *((unsigned char*)&column + 1);

    SSD_D_DIR(true);

    SSD_CS_O(false);
    SSD_DC_O(false);   // 值为0     高为数据、低为指令
    while(WRlen--)
    {
        SSD_WR_O(false);
        SSD_D_O( *pointer++ );
        _NOP();_NOP();
        SSD_WR_O(true);
        _NOP();_NOP();
    }

    //
    SSD_DC_O(true);  // 值为1 写数据
    while(len--)
    {
        SSD_WR_O(false);
        SSD_D_O( *data++ );
        _NOP();_NOP();
        SSD_WR_O(true);
        _NOP();_NOP();
    }
    SSD_CS_O(true);

    SSD_D_DIR(false);
}




//*********写16个字节*********/

void SSD_byte_Write( const unsigned char* data , unsigned short page, unsigned short column,unsigned char flag)
{
    
    unsigned short len = 16;   //16个数据
    unsigned short WRlen = 3;
    unsigned char  a,b,c;

    unsigned char ccommand[3] = {0,0,0};
    const unsigned char* ppointer = ccommand;
    
     a=page+0xb0;                 //设置页地址
     b=column&0x0f;               //设置列低4位
     c=((column&0xf0)>>4)|0x10;   //设置列高4位
    
    
    
     ccommand[0] = *((unsigned char*)&a);
     ccommand[1] = *((unsigned char*)&b);
     ccommand[2] = *((unsigned char*)&c);
    
   
    SSD_D_DIR(true);
    SSD_CS_O(false); 
    
 
    SSD_DC_O(false);     //写指令     // 值为0     高为数据、低为指令
  while(WRlen--)
    {
        SSD_WR_O(false);
        SSD_D_O( *ppointer++ );
        _NOP();_NOP();
        SSD_WR_O(true);
        _NOP();_NOP();
    }
  
    if(flag==0)                         
   {     
      SSD_DC_O(true) ; // 值为1 写数据
      while(len--)
      {
        SSD_WR_O(false);
        SSD_D_O( ~(*data++) );     //反显
        _NOP();_NOP();
        SSD_WR_O(true);
        _NOP();_NOP();
      }
   }
    if(flag==1)
    {     
      SSD_DC_O(true) ; // 值为1 写数据
      while(len--)
      {
        SSD_WR_O(false);
        SSD_D_O( *data++ );     //正显
        _NOP();_NOP();
        SSD_WR_O(true);
        _NOP();_NOP();
       }
    }
    SSD_CS_O(true);

    SSD_D_DIR(false);
}

/********显示一个汉字***********/

void OLED_ShowCHinese(const unsigned char* ddata,unsigned short ppage,unsigned short ccolumn,unsigned short no, unsigned char flag)
{
    SSD_byte_Write( ddata+16*2*no , ppage, ccolumn,flag);
    SSD_byte_Write(ddata+16*2*no+16 ,ppage+1,ccolumn,flag);
}




void SSD_DataBlockRead( unsigned char* data, unsigned short len, unsigned short page, unsigned short column )
{
    unsigned short WRlen = 6;
    unsigned char command[6] = {0x22, 0x00, 0x07, 0x21, 0x00 ,127};
    const unsigned char* pointer = command;

    command[1] = *((unsigned char*)&page);        //Set page Address
    command[2] = *((unsigned char*)&page + 1);
    command[4] = *((unsigned char*)&column);      //Set Column Address
    command[5] = *((unsigned char*)&column + 1);

    SSD_D_DIR(true);

    SSD_CS_O(false);
    SSD_DC_O(false);
    while(WRlen--)
    {
        SSD_WR_O(false);
        SSD_D_O( *pointer++ );
        _NOP();_NOP();
        SSD_WR_O(true);
        _NOP();_NOP();
    }
    SSD_DC_O(true);

    SSD_D_DIR(false);
    _NOP();_NOP();

    //
    SSD_DC_O(true);
    {   //Dummy read
        SSD_RD_O(false);
        _NOP();_NOP();
        SSD_RD_O(true);
    }
    while(len--)
    {
        SSD_RD_O(false);
        _NOP();_NOP();
        *data++ = SSD_D_I();
        _NOP();_NOP();
        SSD_RD_O(true);
    }
    SSD_CS_O(true);
}

void OledClearScreen( void )
{
    unsigned short ii = 0;
    unsigned char* pointer = (unsigned char*)SSDRAM;
    for(ii=9*128; ii; ii--)
        *pointer++ = 0x00;
    SSD_DataBlockWrite( (const unsigned char*)SSDRAM, 8*128, 0x0700, 0x7F00 );
}






void OledDisplayStr(const char* data, fontSize size, position pos )
{   //142'143~148, 152~159, 164'165~170, 173'174~177,
    //x8e'x8f~x94, x98~x9f, xa4'xa5~xaa, xad'xae~xb1 //0x
    //216'217~224, 230~237, 245'246~252, 255'256~261 //o

    unsigned short fontDataLen = (size.width * size.high)>>3;
    unsigned char page1 = 0;
    unsigned char row1 = 0;
    unsigned char column1 = 0;
    unsigned char page2 = 0;
    unsigned char row2 = 0;
    unsigned char column2 = 0;
    unsigned short page;
    unsigned short column;
    unsigned int ii = 0;
    bool Negative = false;

    while( *data )
    {
        //
        switch( *data )
        {   
            case '\R':
                pos.x = 0x20;
                
                data++;
                continue;
          
            case '\r':
                pos.x = 0;
                data++;
                continue;
            case '\n':
                pos.y += size.high;
                data++;
                continue;
            case 0230: //Disable above attributes
                Negative = false;
                data++;
                continue;
            case 0231: //Display cursor
                data++;
                continue;
            case 0232: //Blink character
                data++;
                continue;
            case 0233: //Blinking cursor
                data++;
                continue;
            case 0234: //Alternate character with cursor
                data++;
                continue;
            case 0235: //Negative display
                Negative = true;
                data++;
                continue;
        }
        
        const unsigned char* pointerFont = Ascll2FontData( *data, size);
        
      
        if(pos.x + size.width > 128)
        {
            pos.x = 0;
            pos.y += size.high;  //移到下一行
        }
         
        
        //
        page1 = pos.y>>3 ;
        row1 = pos.y & 0x07;
        column1 = pos.x;
        page2 = (pos.y + size.high - 1)>>3;
        row2 = (pos.y + size.high) & 0x07;
        column2 = pos.x + size.width - 1;

        *((unsigned char*)&page) = page1;
        *((unsigned char*)&page + 1) = page2;
        *((unsigned char*)&column) = column1;
        *((unsigned char*)&column + 1) = column2;
        
       // SSD_DataBlockRead( (unsigned char*)SSDRAM, (page2+1-page1)*(column2+1-column1), page, column );

        //Erase   
        unsigned char* pointer;
        pointer = (unsigned char*)SSDRAM;
        for(ii=size.width; ii; ii--)
            *pointer++ &= 0xFF >> (8 - row1);
        
        pointer = (unsigned char*)SSDRAM + fontDataLen;
        for(ii=size.width; ii; ii--)
            *pointer++ &= 0xFF << row2;
        
        pointer = (unsigned char*)SSDRAM + size.width;
        for(ii= fontDataLen - size.width; ii; ii--)
            *pointer++ &= 0x00;
        
        
        //Fill
        unsigned char FontData = 0;
        const unsigned char* pointer1;
        pointer = (unsigned char*)SSDRAM;
        pointer1 = pointerFont;       
        for(ii=fontDataLen; ii; ii--)
        {  
            FontData = Negative ? ~(*pointer1++) : *pointer1++;
            *pointer++ |= FontData << (row1);

        }

        
        pointer = (unsigned char*)SSDRAM + size.width;
        pointer1 = pointerFont;
        for(ii=fontDataLen; ii; ii--)
        {
            FontData = Negative ? ~(*pointer1++) : *pointer1++;
            *pointer++ |= FontData >> (8 - row2);
        }
        
       
        SSD_DataBlockWrite( (const unsigned char*)SSDRAM, (page2+1-page1)*(column2+1-column1), page, column );

        pos.x += size.width;
        data++;
    }
}


unsigned char* Ascll2FontData( unsigned char ascii, fontSize size)
{
    unsigned short ii = ((ascii - 0x20) * size.width * size.high)>>3;
    return (unsigned char*)( ascii8x16 + ii);
}


