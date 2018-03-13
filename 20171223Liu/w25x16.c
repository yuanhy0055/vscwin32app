/*****************************************************
******************************************************
程序说明：SPI读写程序
硬件连接：W25X16存储芯片
引脚说明：    CS-P5.0; DIO-P5.1; DO-P5.2 CLK-P5.3

原有引脚说明：CS-P2.0; CLK-P2.1; DIO-P2.2; DO-P2.3
******************************************************
*****************************************************/


#include <msp430.h>
#include "w25x16.h"



void SPI_IO_Init(void) //引脚初始化
{
  P5DIR |= BIT0+BIT1+BIT3;
  P5DIR &=~ BIT2;   //DO  输入
  CS_1;
  CLK_0;
  delay_ms(10);
}


void Spi_WriteByte(uchar data)      //写入一个字节
{
  uchar i;
  //CLK_0; //模式0
  //delay_us(1);
  for(i=0;i<8;i++)
   {
    CLK_0;    //CLK_0;
    if(0x80&data)
      DIO_1;
    else
      DIO_0;
    delay_us(1);
    CLK_1;   // CLK_1;
    delay_us(1);
    data<<=1;
   }
 // CLK_0;
  //delay_us(1);
}


uchar Spi_ReadByte(void)      //下降沿读取一个字节
{
  uchar i,temp=0;
  CLK_1; //模式

  for(i=0;i<8;i++)
  {
    delay_us(1);
    CLK_0;   
    delay_us(1);
    temp<<=1;
    if(DO_IN)      //读取最高位，保存至最末尾，通过左移位完成整个字节
     temp |= 0x01;
    CLK_1;   

  }
  
  return temp;
}


void Write_Enable(void)     //芯片写使能
{
  CS_0;
  delay_us(1);
  Spi_WriteByte(WRITE_ENABLE);
  CS_1;
}



void Write_Disable(void)    //芯片禁止写入
{
  CS_0;
  delay_us(1);
  Spi_WriteByte(WRITE_DISABLE);
  CS_1;
}


uchar Read_StatReg(void)    //读取芯片状态寄存器
{
  uchar temp;
  CS_0;
  delay_us(1);
  Spi_WriteByte(READ_STATUSREG);
  temp = Spi_ReadByte();
  CS_1;

  return temp;

}


void Write_StatReg(uchar com)     //写状态寄存器
{
  CS_0;
  delay_us(1);
  Spi_WriteByte(WRITE_STATUSREG);
  Spi_WriteByte(com);
  CS_1;
}



void Erase_Page(long address)   //扇区擦除  4K
{
  uchar H,M,L;
  H = address>>16;
  M = address>>8;
  L = address&0xff;
  Write_Enable(); //先执行写使能
  CS_0;
  delay_us(1);
  Spi_WriteByte(CLEAR_PAGE);
  Spi_WriteByte(H);
  Spi_WriteByte(M);
  Spi_WriteByte(L);
  CS_1;
}


void Write_Byte(long address,uchar byte)    //在任意地址写入一个字节
{
  uchar H,M,L;
  CLK_1;
  H = address>>16;
  M = address>>8;
  L = address&0xff;
  
  CS_0;
  delay_us(1);
  Spi_WriteByte(WRITE_PAGE);
  Spi_WriteByte(H);
  Spi_WriteByte(M);
  Spi_WriteByte(L);
  Spi_WriteByte(byte);
  CS_1;
  delay_us(1);
  CLK_1;
}

//在任意地址开始写入一个数据包（最大长度不超过256个字节）
void Write_Date(long address,uchar Date_Buf[],uchar size)
{
  uchar i;
  uchar H,M,L;
  H = address>>16;
  M = address>>8;
  L = address&0xff;
  CS_0;
  delay_us(1);
  Spi_WriteByte(WRITE_PAGE);
  Spi_WriteByte(H);
  Spi_WriteByte(M);
  Spi_WriteByte(L);
  
  for(i=0;i<size;i++)
  {
    Spi_WriteByte(Date_Buf[i]);
  }
  
  CS_1;
}



uchar Read_Byte(long address) //在任意地址读出一个字节
{
  uchar temp;
  uchar H,M,L;
  CLK_1;
  H = address>>16;
  M = address>>8;
  L = address&0xff;
  CS_0;
  delay_us(1);
  Spi_WriteByte(READ_DATE);
  Spi_WriteByte(H);
  Spi_WriteByte(M);
  Spi_WriteByte(L);
  temp = Spi_ReadByte();
  CS_1;
  delay_us(1);
  CLK_1;
  return temp;

}



void Read_Data(long address,uchar Date_Buf[],uint size)   //从任意地址开始读出数据
{
  uchar i;
  uchar H,M,L;

  H = address>>16;
  M = address>>8;
  L = address&0xff;
  CS_0;
  delay_us(1);
  Spi_WriteByte(READ_DATE);
  Spi_WriteByte(H);
  Spi_WriteByte(M);
  Spi_WriteByte(L);
  
  for(i=0;i<size;i++)
  {
    Date_Buf[i] = Spi_ReadByte();
  }

  CS_1;

}

