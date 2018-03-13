#include "rx8025.h"	
#include "iic.h"	
//#include "RC522.h"	
//#include "main.h"	
/*******************************************************************************/


#define  uint unsigned int
#define  uchar unsigned char





/********************************************************
// ˵����RX-8025SA��������
// SCL=P1.0
// SDA=P1.1
********************************************************/
/*******************************************************************************
 * name: AdjustRX8025SA()  
 * description: ����ʱ�ӡ�timetype����Ҫ�޸ĵ�ʱ�����ͣ�value�������õ�ʱ��ֵ(BCD��ʽ)��
 * �t   timetyp=0x00    ��
 *    �ttimetyp=0x01    ��
 *    �ttimetyp=0x02    Сʱ
 *    �ttimetyp=0x03    ����
 *    �ttimetyp=0x04    ��
 *    �ttimetyp=0x05    ��(����)
 *    �ttimetyp=0x06    ��
 * input parameter: timetype, value
 * output parameter: NULL
 ********************************************************************************/
void AdjustRX8025SA(uchar timetype,uchar value)
{
  I2C_START();             //��������
  I2C_WriteByte(0x64);     //��������д��ַ,64HΪдRX8025SA�ĵ�ַ
  I2C_GetACK();            //�������������ݺ�ȴ���Ӧ��
  I2C_WriteByte(timetype); //��Ҫ�޸ĵ�ʱ������
  I2C_GetACK();            //�������������ݺ�ȴ���Ӧ��
  I2C_WriteByte(value);
  I2C_GetACK();            //�������������ݺ�ȴ���Ӧ��
  I2C_STOP();              //ֹͣ����
}
/*****************************************************************
��������: void InitRX8025SA(void)
��������: ����RX8025SA�ļĴ���
��������: ��
����˵��:
*****************************************************************/
void InitRX8025SA(void)
{
  AdjustRX8025SA(0xe0,0x20); //���ó�24Сʱ��
}

/*****************************************************************
��������: void GetRX8025SA(uchar firsttype,uchar count,uchar *buff)
��������: ��ȡʱ��оƬRX8025SA��ʱ��,����Ҫ���ĵ�һ��ʱ������firsttype��
          �����ö�ȡ���ֽ��������һ�ΰ�ʱ���ȡ��buff��
��������: 
�������:
����˵��:
*****************************************************************/

void GetRX8025SA(uchar firsttype, uchar count, uchar *buff)
{
  uchar i;
  I2C_START();              // ��������
  I2C_WriteByte(0x64);      // ��������д��ַ,64HΪдRX8025SA�ĵ�ַ
  I2C_GetACK();             // �������������ݺ�ȴ���Ӧ��
  I2C_WriteByte(firsttype);
  I2C_GetACK();             // �������������ݺ�ȴ���Ӧ��
  I2C_START();              // ��������
  I2C_WriteByte(0x65);      // ������������ַ,65HΪ��RX8025SA�ĵ�ַ
  I2C_GetACK();             // �������������ݺ�ȴ���Ӧ��
  for(i = 0; i < count; i++)
  {
    *buff = I2C_ReadByte();
    if(i != count-1)        // �����һ���ֽ��⣬������Ҫ��������Ӧ��
     {
      I2C_SetACK();         // �������������ݺ�����Ӧ��
     }
    buff++;
  }
  I2C_SetNAK();             //�����������һ���ֽ�ʱ��������Ӧ��NO_ACK
  I2C_STOP();               // ֹͣ����
}

/*****************************************************************
��������: void ReadRX8025SAClockData(uchar *ClockData)
��������: ��ȡʱ��оƬRX8025SA��ʱ������
��������: 
�������:
����˵��:
*****************************************************************/

void ReadRX8025SAClockData(uchar  *ClockData)
{
  uchar ClockBuff[8];
  GetRX8025SA(0x00,7,ClockBuff); // ��ȡʱ�� 
  ClockData[5] = ClockBuff[0]&0x7F;   // ��
  ClockData[4] = ClockBuff[1]&0x7F;   // ��
  ClockData[3] = ClockBuff[2]&0x3F;   // ʱ
  ClockData[2] = ClockBuff[4]&0x3F;   // ��
  ClockData[1] = ClockBuff[5]&0x1F;   // ��
  ClockData[0] = ClockBuff[6];        // ��
}

/*****************************************************************
��������: void writeRX8025SARegister(uchar address, uchar  value)
��������: ����RX8025SA�ļĴ���
��������: ��
����˵��:
*****************************************************************/

void writeRX8025SARegister(uchar address, uchar value)
{
  I2C_START();                     // ��������
  I2C_WriteByte(0x64);             // ��������д��ַ,64HΪдRX8025SA�ĵ�ַ
  I2C_GetACK();                    // �������������ݺ�ȴ���Ӧ��
  I2C_WriteByte(address);          // ��������д��ַ,01HΪ����/״̬�Ĵ���2��
  I2C_GetACK();                    // �������������ݺ�ȴ���Ӧ��
  I2C_WriteByte(value);            //���ÿ���/״̬�Ĵ���2: 0000 1010
  I2C_GetACK();                    // �������������ݺ�ȴ���Ӧ��
  I2C_STOP();                      // ֹͣ����
}


/*****************************************************************
��������: void SetRX8025SACurrentTime(uchar *TimeValue)
��������: ����RX8025SA�ĵ�ǰʱ��
��������: ��
����˵��:
*****************************************************************/
void SetRX8025SACurrentTime(uchar  *TimeValue)
{ 
  AdjustRX8025SA(0x60,TimeValue[0]);  //��
  AdjustRX8025SA(0x50,TimeValue[1]);
  AdjustRX8025SA(0x40,TimeValue[2]);
  AdjustRX8025SA(0x20,TimeValue[3]);
  AdjustRX8025SA(0x10,TimeValue[4]);
  AdjustRX8025SA(0x00,TimeValue[5]);  //��
}





