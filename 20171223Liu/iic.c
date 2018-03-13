
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
 * description: ����I2C����
 * input parameter: NULL
 * output parameter: NULL
 ********************************************************************************/
//����Ƶ�ʽӽ�100k��8563���400k
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
 * description: ֹͣI2C����
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
 * description: �������������ݺ�ȴ���Ӧ��д����ʱ,Ӧ��������ڵھŸ�ʱ������
 * ��SDA�����ͣ���ʾ���յ�һ��8λ����,����ʾ�ɼ������ա���������ӻ�дһ�ֽں�
 * ���մ�Ӧ���ٽ��к�������(һ�㲻���Ǵ�Ӧ��λ�ľ���ֵ������Ӧ����ʱ�����ж�
 * �������Ƿ�������ڲ�д����)
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
 * description: �������������ݺ�����Ӧ�𡣶�����ʱ,����ÿ���մӻ�һ���ֽ�����
 * (�������һ��)��������Ӧ��ACK(SDA=0)���ǽ������һ���ֽ�ʱ��������Ӧ��NO_ACK��SDA��1��
 * input parameter: NULL
 * output parameter: NULL
 ********************************************************************************/
void I2C_SetACK(void) //��Ӧ��ACK
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
 * description: �����������һ���ֽ�ʱ��������Ӧ��NO_ACK
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
 * description: �ֽ����ݴ��ͺ���
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
    if(sdata & 0x80) //�ȷ������λ����������λ
        SDA_high;
    else
        SDA_low;
      delay(1);
      SCL_high;
      delay(1);
      sdata<<=1;
  }
  SCL_low;//�������
}
/*******************************************************************************
 * name: I2C_ReadByte()  
 * description: �ֽ���������
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
    delay(1);//�ȴ��������ͳ�����
    SCL_high;
    delay(1);
    if(P2IN & BIT5) 
      rdata |= 0x01;
   }
  SCL_low;//�������
  return rdata;
}
