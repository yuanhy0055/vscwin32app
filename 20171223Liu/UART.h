#ifndef __UART_H
#define __UART_H



#define BaudRate_UART0 9600
#define BaudRate_UART1 9600

#define UartReceiveBuf_Size 90

#define UartSendBuf_Size 100
//#define SP3222_SHDN   0x40    //P4.6  High is normal control, Low is shutdown
//#define RS485_EN      0x80    //P2.7  High is Enable the Out to AB buffer







typedef enum UART_MODULE_
{
  UART0 = 1,
  UART1 = 2,
}UART_MODULE;
typedef enum BaudRateType_ {
    BAUD50,                //POSIX ONLY
    BAUD75,                //POSIX ONLY
    BAUD110,
    BAUD134,               //POSIX ONLY
    BAUD150,               //POSIX ONLY
    BAUD200,               //POSIX ONLY
    BAUD300,
    BAUD600,
    BAUD1200,
    BAUD1800,              //POSIX ONLY
    BAUD2400,
    BAUD4800,
    BAUD9600,
    BAUD14400,             //WINDOWS ONLY
    BAUD19200,
    BAUD38400,
    BAUD56000,             //WINDOWS ONLY
    BAUD57600,
    BAUD76800,             //POSIX ONLY
    BAUD115200,
    BAUD128000,            //WINDOWS ONLY
    BAUD256000             //WINDOWS ONLY
}BaudRateType;



typedef struct UartReceiveOrBuf { 
    unsigned char  ID;      //
    unsigned char  Finish;  //
    unsigned short iterator;//
    unsigned short Length;  //
    unsigned short Size ;   //
    unsigned char  data[UartReceiveBuf_Size+1];//
} UartRxBuf ;


typedef struct UartTransmitterBuf { 
    unsigned char  ID;      //
    unsigned char  Finish;  //
    unsigned short iterator;//
    unsigned short Length;  //
    unsigned short Size ;   //
    unsigned char  data[UartSendBuf_Size+1];//
} UartTxBuf ;


extern UartRxBuf Uart0Receive;
extern UartRxBuf Uart1Receive;
extern UartTxBuf Uart0Transmitter;
extern UartTxBuf Uart1Transmitter;



extern const unsigned char hextoascii[];
//CRC 
extern const unsigned char auchCRCHi[];
//CRC
extern const unsigned char auchCRCLo[];

//unsigned char recv_buf[16];


extern void init_UART( UART_MODULE id );
extern void init_UART0(void);
extern void init_UART1(void);

extern void Uart1_Send_Byte(unsigned char byte);
extern unsigned char Uart1_Sent_nStr(unsigned char *str, unsigned int n);
extern void Uart0_Send_Byte(unsigned char byte);
extern unsigned char Uart0_Sent_nStr(unsigned char *str, unsigned int n);

extern void WriteStringToUART(UART_MODULE id, const char* string);
extern void WriteHexToUART(UART_MODULE id, const char* hex, int size);
extern unsigned char HexToAscii(unsigned char* input, unsigned short size, unsigned char* output);
extern unsigned char AsciiToHex(unsigned char* input, unsigned short size, unsigned char* output);
extern unsigned short CRC_16(unsigned char* puchMsg, unsigned short usDataLen);
extern unsigned char LRC_8(unsigned char* puchMsg, unsigned short usDataLen);
extern void test_UART0(void);
extern void test_UART1(void);




#endif

