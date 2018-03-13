
#include <msp430.h>
#include <stdbool.h>

#include "UART.h"
#include "delay.h"
#include "windows.h"

// *****************************************************************************
// *****************************************************************************
// Section: Data
// *****************************************************************************
// *****************************************************************************
UartRxBuf Uart0Receive ={
  UART0, false, 0, 0, UartReceiveBuf_Size, {'\0'}
};
UartRxBuf Uart1Receive ={
  UART1, false, 0, 0, UartReceiveBuf_Size, {'\0'}
};
UartTxBuf Uart0Transmitter ={
  UART0, false, 0, 0, UartSendBuf_Size, {'\0'}
};
UartTxBuf Uart1Transmitter ={
  UART1, true, 0, 0, UartSendBuf_Size, {'\0'}
};

unsigned char recv_count;
unsigned char recv_buf[16]={0};
unsigned char recv1_count;
unsigned char recv1_buf[16]={0};

extern unsigned char Rchaxun[4];

extern unsigned char model;




const unsigned char hextoascii[] = {
    '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'
};

//CRC
const unsigned char auchCRCHi[] = { 
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40 
};
//CRC校验
const unsigned char auchCRCLo[] = { 
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 
    0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 
    0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 
    0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 
    0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4, 
    0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3, 
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 
    0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 
    0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 
    0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 
    0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED, 
    0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26, 
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 
    0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 
    0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 
    0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 
    0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 
    0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5, 
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 
    0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 
    0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 
    0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 
    0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B, 
    0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C, 
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 
    0x43, 0x83, 0x41, 0x81, 0x80, 0x40 
};


void init_UART0(void)
{
    unsigned short BaudRate = 8000000/BaudRate_UART0;//CLK/BaudRate

    UCA0CTL1 &= ~UCSWRST;//
    UCA0CTL0 &= ~UC7BIT; //
    UCA0CTL0 &= ~UCSPB;  //
    UCA0CTL0 &= ~UCPEN;  //
    UCA0CTL0 &= ~(UCMODE1 | UCMODE0); //
    UCA0CTL0 &= ~UCSYNC; //
    UCA0CTL1 |= UCSSEL1;   //
    /* 8M / 9600, BR = 832 = 0x0340, UCBRS = 1, UCBRF = 0 */
    //UCA0BR0 = 0x40;    //0x340   低位写0x40
    //UCA0BR1 = 0x03;    //高位写0x03
    /* 1M / 9600, BR = 104 = 0x0068, UCBRS = 1, UCBRF = 0 */
    UCA0BR0 = *((unsigned char*)&BaudRate+0);//0x68;
    UCA0BR1 = *((unsigned char*)&BaudRate+1);//0x00;
    UCA0MCTL &= ~(UCBRF0 | UCBRF1 | UCBRF2 | UCBRF3);//UCBRF = 0
    UCA0MCTL &= ~(UCBRS1 | UCBRS2);//UCBRS = 1
    UCA0MCTL |= UCBRS0;

    //P3.4 -- TX, P3.5 -- RX
    P3SEL |= BIT4 | BIT5;//
    P3DIR |= BIT4;       //
    P3DIR &= ~BIT5;      //
    P3OUT |= BIT5;       //
    P3REN |= BIT5;       //

    UC0IE |= UCA0RXIE | UCA0TXIE;
}

void init_UART1(void)
{
    unsigned short BaudRate = 8000000/BaudRate_UART1;//CLK/BaudRate
    
    UCA1CTL1 &= ~UCSWRST;//
    UCA1CTL0 &= ~UC7BIT; //
    UCA1CTL0 &= ~UCSPB;  //
    UCA1CTL0 &= ~UCPEN;  //
    UCA1CTL0 &= ~(UCMODE0 | UCMODE0); //
    UCA1CTL0 &= ~UCSYNC; //
    UCA1CTL1 |= UCSSEL1;   //
    /* 1M / 9600, BR = 104 = 0x0068, UCBRS = 1, UCBRF = 0 */
    UCA1BR0 = *((unsigned char*)&BaudRate+0);//0x68;
    UCA1BR1 = *((unsigned char*)&BaudRate+1);//0x00;
    UCA1MCTL &= ~(UCBRF0 | UCBRF1 | UCBRF2 | UCBRF3);//UCBRF = 0
    UCA1MCTL &= ~(UCBRS1 | UCBRS2);//UCBRS = 1
    UCA1MCTL |= UCBRS0;
  
    //P3.6 -- TX, P3.7 -- RX
    P3SEL |= BIT6 | BIT7;//
    P3DIR |= BIT6;       //
    P3DIR &= ~BIT7;      //
    P3OUT |= BIT7;       //
    P3REN |= BIT7;       //
  

    UC1IE |= UCA1RXIE | UCA1TXIE;
}

// *****************************************************************************
// UART0 RX interrupt handler
// *****************************************************************************
#pragma vector = USCIAB0RX_VECTOR
__interrupt void UART0_RXISR(void)
{
  switch (model)
  {
   case 0:        //测量     
      unsigned char temp = UCA0RXBUF;
      UartRxBuf* UartReceive = &Uart0Receive;

      if(!(*UartReceive).Finish)
      {
        if(temp=='R')
        {
            (*UartReceive).Length = 0;
            (*UartReceive).iterator = 0;
        }
        else if((*UartReceive).iterator>=(*UartReceive).Size)
        {
            (*UartReceive).Length = 0;
            (*UartReceive).iterator = 0;
            (*UartReceive).Finish = true;
            return;
        }
        else if((*UartReceive).iterator==3)
        {
            if(temp=='A')
            {
                (*UartReceive).data[(*UartReceive).iterator++] = temp;
                (*UartReceive).Length++;
                (*UartReceive).iterator = 0;
                (*UartReceive).Finish = true;
                return;
            }
        }
        else if(temp=='\n')
        {
            if((*UartReceive).iterator>1)
            {
                if((*UartReceive).data[(*UartReceive).iterator-1]=='\r')
                {
                    (*UartReceive).data[(*UartReceive).iterator++] = temp;
                    (*UartReceive).Length++;
                    (*UartReceive).iterator = 0;
                    (*UartReceive).Finish = true;
                    return;
                }
            }
        }
        (*UartReceive).data[(*UartReceive).iterator++] = temp;
        (*UartReceive).Length++;
      }
    
     break;
    
   case 1 :        //通讯
     
     break;
     
   case 2:         //刻度
     UCA1TXBUF = UCA0RXBUF;
     break;
     
   default:
     break;

  }

   UC0IFG &= ~UCA0RXIFG;  
}

// *****************************************************************************
// UART0 TX interrupt handler
// *****************************************************************************
#pragma vector = USCIAB0TX_VECTOR
__interrupt void UART0_TXISR(void)
{
   switch ( model )
   { 
    case 0:   
      UartTxBuf* UartTransmitter = &Uart0Transmitter;

      if((*UartTransmitter).iterator < (*UartTransmitter).Length)
      {
        if((*UartTransmitter).Length <= (*UartTransmitter).Size)
        {
            (*UartTransmitter).Finish = false;
            UCA0TXBUF = (*UartTransmitter).data[(*UartTransmitter).iterator++];
        }
      }
      else
      {
        (*UartTransmitter).Length = 0;
        (*UartTransmitter).iterator = 0;
        (*UartTransmitter).Finish = true;
       }
      break;     
    case 1:
      break;
    case 2:
      break;
    default:
      break;
   }     

    UC0IFG &= ~UCA0TXIFG;
}

// *****************************************************************************
// UART1 RX interrupt handler
// *****************************************************************************
#pragma vector = USCIAB1RX_VECTOR
__interrupt void UART1_RXISR(void)
{
  switch (model)
  {
   case 0:                 //测量
     break;
   case 1:                 //通讯
     break;
   case 2:                 //刻度
      UCA0TXBUF = UCA1RXBUF ;
     break;
   default:
     break;      
  }
  
   //UCA1TXBUF = UCA1RXBUF;
   UC1IFG &= ~UCA1RXIFG;
}

// *****************************************************************************
// UART1 TX interrupt handler
// *****************************************************************************
#pragma vector = USCIAB1TX_VECTOR
__interrupt void UART1_TXISR(void)
{
  
    
    UC1IFG &= ~UCA1TXIFG;
}





void WriteStringToUART(UART_MODULE id, const char* string)
{
    UartTxBuf *UartTransmitter;
    
    switch(id)
    {
        case UART0:
            UartTransmitter = &Uart0Transmitter;
            break;
        case UART1:
            UartTransmitter = &Uart1Transmitter;
            break;
        default:
            return;
    }

    //while(!(*UartTransmitter).Finish) _NOP();
    volatile unsigned int ii = 20000;
    do ii--;
    while( ii && !(*UartTransmitter).Finish );
    if( !ii ) return;
    
    (*UartTransmitter).Length = 0;
    while(*string)
    {
        if((*UartTransmitter).Length < (*UartTransmitter).Size)
            (*UartTransmitter).data[(*UartTransmitter).Length++] = *(string++);
        else 
        {
            (*UartTransmitter).data[(*UartTransmitter).Length] = '\0';
            break;
        }
    }
    (*UartTransmitter).iterator = 0;
    //INTSetFlag
    switch(id)
    {
        case UART0:
            UC0IFG |= UCA0TXIFG;
            break;
        case UART1:
            UC1IFG |= UCA1TXIFG;
            break;
        default:
            return;
    }
}

void WriteHexToUART(UART_MODULE id, const char* hex, int size)
{
    UartTxBuf *UartTransmitter;

    switch(id)
    {
        case UART0:
            UartTransmitter = &Uart0Transmitter;
            break;
        case UART1:
            UartTransmitter = &Uart1Transmitter;
            break;
        default:
            return;
    }

    //while(!(*UartTransmitter).Finish) _NOP();
    volatile unsigned int ii = 20000;
    do ii--;
    while( ii && !(*UartTransmitter).Finish );
    if( !ii ) return;
    
    (*UartTransmitter).Length = size;
    (*UartTransmitter).iterator = 0;
    while(size--)
    {
        if((*UartTransmitter).iterator < (*UartTransmitter).Size)
            (*UartTransmitter).data[(*UartTransmitter).iterator++] = *(hex++);
        else 
        {
            (*UartTransmitter).data[(*UartTransmitter).iterator] = '\0';
            break;
        }
    }
    (*UartTransmitter).iterator = 0;
    //INTSetFlag
    switch(id)
    {
        case UART0:
            UC0IFG |= UCA0TXIFG;
            break;
        case UART1:
            UC1IFG |= UCA1TXIFG;
            break;
        default:
            return;
    }
}










unsigned char HexToAscii(unsigned char* input, unsigned short size, unsigned char* output)
{
    unsigned char ii=0;

    for(ii=0;ii<size;ii++)
    {
       *(output+ii*2+0) = hextoascii[((unsigned short)*(input+size-1-ii))>>4];
       *(output+ii*2+1) = hextoascii[((unsigned short)*(input+size-1-ii))&0x000F];
    }
    *(output+size*2) = '\0';
    
    return (0);
}


unsigned char AsciiToHex(unsigned char* input, unsigned short size, unsigned char* output)
{
    unsigned char st=0;
    unsigned char ii=0;
    unsigned char jj=0;

    //if((size<=0)||(size>16))
    {
        //st = 1;
        //for(jj=0;jj<(size+1)/2;jj++) *(output+jj) = 0;
    }
   // else
    {
        for(ii=0;ii<size;ii++)
        {
            if((*(input+ii)>='0')&&(*(input+ii)<='9')) 
            {
                if(ii & 0x01)
                {
                    *(output+((size-1-ii)>>1)) += *(input+ii)-'0';
                }
                else
                {
                    *(output+((size-1-ii)>>1)) = (*(input+ii)-'0') * 16;
                } 
            }
            else if((*(input+ii)>='A')&&(*(input+ii)<='F')) 
            {
                if(ii & 0x01)
                {
                    *(output+((size-1-ii)>>1)) += *(input+ii)-'A'+10;
                }
                else
                {
                    *(output+((size-1-ii)>>1)) = (*(input+ii)-'A'+10) * 16;
                }
            }
            else if((*(input+ii)>='a')&&(*(input+ii)<='f')) 
            {
                if(ii & 0x01)
                {
                    *(output+((size-1-ii)>>1)) += *(input+ii)-'a'+10;
                }
                else
                {
                    *(output+((size-1-ii)>>1)) = (*(input+ii)-'a'+10) * 16;
                }
            }
            else 
            {
                st = 2;
                for(jj=0;jj<(size+1)>>1;jj++) *(output+jj) = 0;
                break;
            }
        }
    }
    return (st);
}


unsigned short CRC_16(unsigned char* puchMsg, unsigned short usDataLen)
{

    unsigned char uchCRCHi = 0xFF ; //
    unsigned char uchCRCLo = 0xFF ; //
    unsigned uIndex ;               //

    while (usDataLen--) //
    {
        uIndex = uchCRCHi ^ (*(puchMsg++)); //
        uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex];
        uchCRCLo = auchCRCLo[uIndex] ; 
    } 
    return (uchCRCHi << 8 | uchCRCLo) ; 
}


unsigned char LRC_8(unsigned char* puchMsg, unsigned short usDataLen)
{ 
    unsigned char uchLRC = 0 ;

    while(usDataLen--)       
    {
        uchLRC += *(puchMsg++);
    }
    return (-uchLRC);
}
/**
  * @brief  USART1 发送一个字节
  * @param  None
  * @retval None
*/
void Uart1_Send_Byte(unsigned char byte)
{
   //while (!(IFG2 & UTXIFG1));             // TX缓存空闲？
   UCA1TXBUF = byte;
}

/**
  * @brief  USART1 发送字符串
  * @param  None
  * @retval None
*/
unsigned char Uart1_Sent_nStr(unsigned char *str, unsigned int n)
{

   unsigned int i;

   for(i = 0; i < n; i++)
   {
       Uart1_Send_Byte(str[i]);
       delay_ms(3);
       //WDTCTL = WDT_ARST_1000; //1s看门狗功能启动
   }
   return 1;
}

/**
  * @brief  USART0 发送一个字节
  * @param  None
  * @retval None
*/
void Uart0_Send_Byte(unsigned char byte)
{
   //while (!(IFG2 & UTXIFG1));             // TX缓存空闲？
   UCA0TXBUF = byte;
}

/**
  * @brief  USART0 发送字符串
  * @param  None
  * @retval None
*/
unsigned char Uart0_Sent_nStr(unsigned char *str, unsigned int n)
{

   unsigned int i;

   for(i = 0; i < n; i++)
   {
       Uart0_Send_Byte(str[i]);
       delay_ms(3);
       //WDTCTL = WDT_ARST_1000; //1s看门狗功能启动
   }
   return 1;
}