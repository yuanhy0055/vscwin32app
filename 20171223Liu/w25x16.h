
//现在引脚说明：CS-P5.0; SI-P5.1; SO-P5.2 CLK-P5.3

//原有引脚说明：CS-P2.0; CLK-P2.1; DIO-P2.2; DO-P2.3



#ifndef _w25x16_H
#define _w25x16_H

#include <msp430.h>
#include "delay.h"

#define uchar unsigned char
#define uint  unsigned int

#define CPU_F ((double)8000000)

//#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
//#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))

#define CS_1 P5OUT |= BIT0
#define CS_0 P5OUT &= ~BIT0

#define CLK_1 P5OUT |= BIT3
#define CLK_0 P5OUT &= ~BIT3

#define DIO_1 P5OUT |= BIT1
#define DIO_0 P5OUT &= ~BIT1

#define DO_IN P5IN & BIT2 //输入引脚



//***********W25X16 命令指令表定义**********//

#define   WRITE_ENABLE     0X06
#define   WRITE_DISABLE    0X04
#define   READ_STATUSREG   0X05
#define   WRITE_STATUSREG    0X01
#define   READ_DATE     0X03
#define   READ_FAST     0X0B
#define   READ_DOUBLEFAST    0X3B
#define   WRITE_PAGE    0X02
#define   CLEAR_BLOCK     0XD8
#define   CLEAR_PAGE    0X20
#define   CLEAR_SHIP     0XC7
#define   POWER_OFF    0XB9
#define   POWER_ON     0XAB
#define   SHIP_ID    0X90
#define   JEDEC_ID   0X9F



//*******函数声明**********//

extern void SPI_IO_Init(void);
extern void Write_Enable(void);
extern void Write_Disable(void);
extern uchar Read_StatReg(void);
extern void Write_StatReg(uchar com);
extern void Erase_Page(long address);
extern uchar Read_Byte(long address);
extern void Read_Data(long address,uchar Date_Buf[],uint size);
extern void Write_Byte(long address,uchar byte);
extern void Write_Date(long address,uchar Date_Buf[],uchar size);


#endif