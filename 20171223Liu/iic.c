
#include <msp430.h>
//#include "RX8025.h"	
#include "iic.h"	
	
//#include "main.h"


#define  uint unsigned int
#define  uchar unsigned char




 /*******************************************************************************
 * name: delay()
 * description: delay time.
 * input parameter: The times that need to delay.
 * output parameter: Null
 ********************************************************************************/
static void delay(uint tt)
{
  int i;
  for (i=0; i<tt; i++);
} 
/*******************************************************************************
 * name: I2C_START()  
 * description: 启动I2C总线
 * input parameter: NULL
 * output parameter: NULL
 ********************************************************************************/
//总线频率接近100k，8563最高400k
void I2C_START(void)
{
  //P2SEL |= 0x01;
  //P2SEL &= 0x01;
  SDA_out;
  delay(1);
  SCL_out;
  delay(1);
  
  SDA_high;
  delay(1);
  SCL_high; //initialize
  delay(1); 
  SDA_low;  //start
  delay(1); 
  SCL_low;
  delay(1);
}
/*******************************************************************************
 * name: I2C_STOP()  
 * description: 停止I2C总线
 * input parameter: NULL
 * output parameter: NULL
 ********************************************************************************/
void I2C_STOP(void)
{
  SDA_out;
  delay(1);
  SCL_out;
  delay(1);  
  SDA_low;
  delay(1);
  SCL_low;
  delay(1);
  SCL_high;  
  delay(1);
  SDA_high;  
  delay(1);  
  SCL_low;
  delay(1);
  SCL_high;
}
/*******************************************************************************
 * name: I2C_GetACK()  
 * description: 主机发送完数据后等待从应答。写数据时,应答的器件在第九个时钟周期
 * 将SDA线拉低，表示已收到一个8位数据,并表示可继续接收。主机在向从机写一字节后
 * 接收从应答，再进行后续操作(一般不考虑从应答位的具体值，仅在应答检测时用于判断
 * 从器件是否完成其内部写周期)
 * input parameter: NULL
 * output parameter: NULL
 ********************************************************************************/
int I2C_GetACK(void)
{
  int nTemp=0;
  SCL_out;
  delay(1);
  SCL_low;
  delay(1);
  SDA_in;
  SCL_high;
  delay(1);
  nTemp=(int)(P2IN & BIT5);
  SCL_low;
  delay(1);
  return (nTemp & BIT5);
}
/*******************************************************************************
 * name: I2C_SetACK()  
 * description: 主机接收完数据后发送主应答。读数据时,主机每接收从机一个字节数据
 * (不是最后一个)，返回主应答ACK(SDA=0)，是接收最后一个字节时返回无需应答NO_ACK（SDA＝1）
 * input parameter: NULL
 * output parameter: NULL
 ********************************************************************************/
void I2C_SetACK(void) //主应答ACK
{
  SDA_out;
  delay(1);
  SCL_out;
  
  SCL_low;
  delay(1);
  SDA_low;
  delay(1);
  SCL_high;
  delay(1);
  SCL_low;
  delay(1);
}
/*******************************************************************************
 * name: I2C_SetNAK()  
 * description: 主机接收最后一个字节时返回无需应答NO_ACK
 * input parameter: NULL
 * output parameter: NULL
 ********************************************************************************/
void I2C_SetNAK(void)
{
  SDA_out;
  delay(1);
  SCL_out;
  
  SCL_low;
  delay(1);
  SDA_high;
  delay(1);
  SCL_high;
  delay(1);
  SCL_low;
  delay(1);
}
/*******************************************************************************
 * name: I2C_WriteByte()  
 * description: 字节数据传送函数
 * input parameter: sdata
 * output parameter: NULL
 ********************************************************************************/
void I2C_WriteByte(unsigned char sdata)
{
  unsigned char i;
  SDA_out;
  SCL_out;
  for(i = 0; i < 8; i++)
  {
    SCL_low;    
    if(sdata & 0x80) //先发送最高位，最后发送最低位
        SDA_high;
    else
        SDA_low;
      delay(1);
      SCL_high;
      delay(1);
      sdata<<=1;
  }
  SCL_low;//发送完毕
}
/*******************************************************************************
 * name: I2C_ReadByte()  
 * description: 字节数读函数
 * input parameter: NULL
 * output parameter: rdata
 ********************************************************************************/
uchar I2C_ReadByte(void)
{
  unsigned char i;
  uchar rdata=0;
  SDA_in;
  SCL_out;
  
  SDA_high;
  for(i=0;i<8; i++)
  {
    rdata<<=1;
    SCL_low;
    delay(1);//等待从器件送出数据
    SCL_high;
    delay(1);
    if(P2IN & BIT5) 
      rdata |= 0x01;
   }
  SCL_low;//接收完毕
  return rdata;
}
