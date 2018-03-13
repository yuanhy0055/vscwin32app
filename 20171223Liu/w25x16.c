/*****************************************************
******************************************************
����˵����SPI��д����
Ӳ�����ӣ�W25X16�洢оƬ
����˵����    CS-P5.0; DIO-P5.1; DO-P5.2 CLK-P5.3

ԭ������˵����CS-P2.0; CLK-P2.1; DIO-P2.2; DO-P2.3
******************************************************
*****************************************************/


#include <msp430.h>
#include "w25x16.h"



void SPI_IO_Init(void) //���ų�ʼ��
{
  P5DIR |= BIT0+BIT1+BIT3;
  P5DIR &=~ BIT2;   //DO  ����
  CS_1;
  CLK_0;
  delay_ms(10);
}


void Spi_WriteByte(uchar data)      //д��һ���ֽ�
{
  uchar i;
  //CLK_0; //ģʽ0
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


uchar Spi_ReadByte(void)      //�½��ض�ȡһ���ֽ�
{
  uchar i,temp=0;
  CLK_1; //ģʽ

  for(i=0;i<8;i++)
  {
    delay_us(1);
    CLK_0;   
    delay_us(1);
    temp<<=1;
    if(DO_IN)      //��ȡ���λ����������ĩβ��ͨ������λ��������ֽ�
     temp |= 0x01;
    CLK_1;   

  }
  
  return temp;
}


void Write_Enable(void)     //оƬдʹ��
{
  CS_0;
  delay_us(1);
  Spi_WriteByte(WRITE_ENABLE);
  CS_1;
}



void Write_Disable(void)    //оƬ��ֹд��
{
  CS_0;
  delay_us(1);
  Spi_WriteByte(WRITE_DISABLE);
  CS_1;
}


uchar Read_StatReg(void)    //��ȡоƬ״̬�Ĵ���
{
  uchar temp;
  CS_0;
  delay_us(1);
  Spi_WriteByte(READ_STATUSREG);
  temp = Spi_ReadByte();
  CS_1;

  return temp;

}


void Write_StatReg(uchar com)     //д״̬�Ĵ���
{
  CS_0;
  delay_us(1);
  Spi_WriteByte(WRITE_STATUSREG);
  Spi_WriteByte(com);
  CS_1;
}



void Erase_Page(long address)   //��������  4K
{
  uchar H,M,L;
  H = address>>16;
  M = address>>8;
  L = address&0xff;
  Write_Enable(); //��ִ��дʹ��
  CS_0;
  delay_us(1);
  Spi_WriteByte(CLEAR_PAGE);
  Spi_WriteByte(H);
  Spi_WriteByte(M);
  Spi_WriteByte(L);
  CS_1;
}


void Write_Byte(long address,uchar byte)    //�������ַд��һ���ֽ�
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

//�������ַ��ʼд��һ�����ݰ�����󳤶Ȳ�����256���ֽڣ�
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



uchar Read_Byte(long address) //�������ַ����һ���ֽ�
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



void Read_Data(long address,uchar Date_Buf[],uint size)   //�������ַ��ʼ��������
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

