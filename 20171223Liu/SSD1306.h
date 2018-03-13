
//
#ifndef __SSD1306_H
#define __SSD1306_H

// *****************************************************************************
// *****************************************************************************
// Section: Configuration bits
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
// *****************************************************************************
// Section: System Macros
// *****************************************************************************
// *****************************************************************************
// 80XX-Series Parallel
//Power <--> P4.6
#define SSD_PW_DIR(SsdPwDir) P4DIR = SsdPwDir ? P4DIR | 0x40 : P4DIR & ~0x40
#define SSD_PW_O(SsdPwOut) P4OUT = SsdPwOut ? P4OUT | 0x40 : P4OUT & ~0x40
#define SSD_PW_I() (P4IN & 0x40)
// RES# <--> P3.2
#define SSD_RES_DIR(SsdResDir) P3DIR = SsdResDir ? P3DIR | 0x04 : P3DIR & ~0x04
#define SSD_RES_O(SsdResOut) P3OUT = SsdResOut ? P3OUT | 0x04 : P3OUT & ~0x04
#define SSD_RES_I() (P3IN & 0x04)
// CS# <--> P3.3
#define SSD_CS_DIR(SsdCsDir) P3DIR = SsdCsDir ? P3DIR | 0x08 : P3DIR & ~0x08
#define SSD_CS_O(SsdCsOut) P3OUT = SsdCsOut ? P3OUT | 0x08 : P3OUT & ~0x08
#define SSD_CS_I() (P3IN & 0x08)
// D/C# <--> P3.1
#define SSD_DC_DIR(SsdDcDir) P3DIR = SsdDcDir ? P3DIR | 0x02 : P3DIR & ~0x02
#define SSD_DC_O(SsdDcOut) P3OUT = SsdDcOut ? P3OUT | 0x02 : P3OUT & ~0x02
#define SSD_DC_I() (P3IN & 0x02)
// WR# <--> P3.0
#define SSD_WR_DIR(SsdWrDir) P3DIR = SsdWrDir ? P3DIR | 0x01 : P3DIR & ~0x01
#define SSD_WR_O(SsdWrOut) P3OUT = SsdWrOut ? P3OUT | 0x01 : P3OUT & ~0x01
#define SSD_WR_I() (P3IN & 0x01)
// RD# <--> P2.7
#define SSD_RD_DIR(SsdRdDir) P2DIR = SsdRdDir ? P2DIR | 0x80 : P2DIR & ~0x80
#define SSD_RD_O(SsdRdOut) P2OUT = SsdRdOut ? P2OUT | 0x80 : P2OUT & ~0x80
#define SSD_RD_I() (P2IN & 0x80)
// D0~D1 <--> P1.0~P1.7
#define SSD_D_DIR(SsdDataDir) P1DIR = SsdDataDir ? 0xFF : 0
#define SSD_D_O(SsdDataOut) P1OUT = SsdDataOut
#define SSD_D_I() (P1IN)

// *****************************************************************************
// *****************************************************************************
// Section: Data types
// *****************************************************************************
// *****************************************************************************
typedef enum DCM_ {
    SSDcommand,
    SSDdata,
}DCM;

typedef union fontSize_ {
    unsigned short i;
    struct {
        unsigned char high;
        unsigned char width;
    };
}fontSize;

typedef union position_ {
    unsigned short i;
    struct {
        unsigned char x;  //ап
        unsigned char y;  //рЁ
    };
}position;




//extern
extern void InitOled( void );
extern void SSD_Write( const unsigned char* data, unsigned short len, DCM DC );
extern void SSD_Read( unsigned char* data, unsigned short len, DCM DC );
extern void SSD_DataBlockWrite( const unsigned char* data, unsigned short len, unsigned short page, unsigned short column );
extern void SSD_DataBlockRead( unsigned char* data, unsigned short len, unsigned short page, unsigned short column );

extern void OledClearScreen( void );
extern void OledDisplayStr(const char* data, fontSize size, position pos );
extern unsigned char* Ascll2FontData( unsigned char ascii, fontSize size);

//extern void SSD_byte_Write( const unsigned char* data , unsigned short page, unsigned short column,unsigned char  flag);
//extern void OLED_ShowCHinese(const unsigned char* data,unsigned short page,unsigned short column,unsigned short no,unsigned char  flag);

#endif
