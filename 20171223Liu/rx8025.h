#ifndef _RX8025_H
#define _RX8025_H

#define  uint unsigned int
#define  uchar unsigned char

  void InitRX8025SA(void);
  void SetRX8025SACurrentTime(uchar  *TimeValue);
  void ReadRX8025SAClockData(uchar  *ClockData);
  
#endif