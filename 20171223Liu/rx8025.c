#include "rx8025.h"	
#include "iic.h"	
//#include "RC522.h"	
//#include "main.h"	
/*******************************************************************************/


#define  uint unsigned int
#define  uchar unsigned char





/********************************************************
// 说明：RX-8025SA驱动程序
// SCL=P1.0
// SDA=P1.1
********************************************************/
/*******************************************************************************
 * name: AdjustRX8025SA()  
 * description: 调整时钟。timetype是需要修改的时间类型，value是新设置的时间值(BCD格式)。
 *    timetyp=0x00    秒
 *    timetyp=0x01    分
 *    timetyp=0x02    小时
 *    timetyp=0x03    星期
 *    timetyp=0x04    日
 *    timetyp=0x05    月(世纪)
 *    timetyp=0x06    年
 * input parameter: timetype, value
 * output parameter: NULL
 ********************************************************************************/
void AdjustRX8025SA(uchar timetype,uchar value)
{
  I2C_START();             //启动总线
  I2C_WriteByte(0x64);     //发送器件写地址,64H为写RX8025SA的地址
  I2C_GetACK();            //主机发送完数据后等待从应答
  I2C_WriteByte(timetype); //需要修改的时间类型
  I2C_GetACK();            //主机发送完数据后等待从应答
  I2C_WriteByte(value);
  I2C_GetACK();            //主机发送完数据后等待从应答
  I2C_STOP();              //停止总线
}
/*****************************************************************
函数名称: void InitRX8025SA(void)
函数功能: 设置RX8025SA的寄存器
函数参数: 无
函数说明:
*****************************************************************/
void InitRX8025SA(void)
{
  AdjustRX8025SA(0xe0,0x20); //设置成24小时制
}

/*****************************************************************
函数名称: void GetRX8025SA(uchar firsttype,uchar count,uchar *buff)
函数功能: 读取时钟芯片RX8025SA的时间,设置要读的第一个时间类型firsttype，
          并设置读取的字节数，则会一次把时间读取到buff中
函数参数: 
函数输出:
函数说明:
*****************************************************************/

void GetRX8025SA(uchar firsttype, uchar count, uchar *buff)
{
  uchar i;
  I2C_START();              // 启动总线
  I2C_WriteByte(0x64);      // 发送器件写地址,64H为写RX8025SA的地址
  I2C_GetACK();             // 主机发送完数据后等待从应答
  I2C_WriteByte(firsttype);
  I2C_GetACK();             // 主机发送完数据后等待从应答
  I2C_START();              // 启动总线
  I2C_WriteByte(0x65);      // 发送器件读地址,65H为读RX8025SA的地址
  I2C_GetACK();             // 主机发送完数据后等待从应答
  for(i = 0; i < count; i++)
  {
    *buff = I2C_ReadByte();
    if(i != count-1)        // 除最后一个字节外，其他都要从主机发应答。
     {
      I2C_SetACK();         // 主机接收完数据后发送主应答
     }
    buff++;
  }
  I2C_SetNAK();             //主机接收最后一个字节时返回无需应答NO_ACK
  I2C_STOP();               // 停止总线
}

/*****************************************************************
函数名称: void ReadRX8025SAClockData(uchar *ClockData)
函数功能: 读取时钟芯片RX8025SA的时间数据
函数参数: 
函数输出:
函数说明:
*****************************************************************/

void ReadRX8025SAClockData(uchar  *ClockData)
{
  uchar ClockBuff[8];
  GetRX8025SA(0x00,7,ClockBuff); // 读取时间 
  ClockData[5] = ClockBuff[0]&0x7F;   // 秒
  ClockData[4] = ClockBuff[1]&0x7F;   // 分
  ClockData[3] = ClockBuff[2]&0x3F;   // 时
  ClockData[2] = ClockBuff[4]&0x3F;   // 日
  ClockData[1] = ClockBuff[5]&0x1F;   // 月
  ClockData[0] = ClockBuff[6];        // 年
}

/*****************************************************************
函数名称: void writeRX8025SARegister(uchar address, uchar  value)
函数功能: 设置RX8025SA的寄存器
函数参数: 无
函数说明:
*****************************************************************/

void writeRX8025SARegister(uchar address, uchar value)
{
  I2C_START();                     // 启动总线
  I2C_WriteByte(0x64);             // 发送器件写地址,64H为写RX8025SA的地址
  I2C_GetACK();                    // 主机发送完数据后等待从应答
  I2C_WriteByte(address);          // 发送器件写地址,01H为控制/状态寄存器2；
  I2C_GetACK();                    // 主机发送完数据后等待从应答
  I2C_WriteByte(value);            //设置控制/状态寄存器2: 0000 1010
  I2C_GetACK();                    // 主机发送完数据后等待从应答
  I2C_STOP();                      // 停止总线
}


/*****************************************************************
函数名称: void SetRX8025SACurrentTime(uchar *TimeValue)
函数功能: 设置RX8025SA的当前时间
函数参数: 无
函数说明:
*****************************************************************/
void SetRX8025SACurrentTime(uchar  *TimeValue)
{ 
  AdjustRX8025SA(0x60,TimeValue[0]);  //年
  AdjustRX8025SA(0x50,TimeValue[1]);
  AdjustRX8025SA(0x40,TimeValue[2]);
  AdjustRX8025SA(0x20,TimeValue[3]);
  AdjustRX8025SA(0x10,TimeValue[4]);
  AdjustRX8025SA(0x00,TimeValue[5]);  //秒
}





