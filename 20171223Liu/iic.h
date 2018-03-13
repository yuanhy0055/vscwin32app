
#ifndef _I2C_H_
#define _I2C_H_
	

       //�˿����������ƽ�ߵͿ���
        #define SCL_high (P2OUT |= BIT4)
        #define SCL_low  (P2OUT &= ~BIT4)
        #define SDA_high (P2OUT |= BIT5)
        #define SDA_low  (P2OUT &= ~BIT5)
 
       //�˿ڷ������ 
        #define SCL_in  (P2DIR &= ~BIT4) //SCL���
        #define SCL_out (P2DIR |= BIT4)  //SCL����
        #define SDA_in  (P2DIR &= ~BIT5) //SDA����
        #define SDA_out (P2DIR |= BIT5)  //SDA���

	void I2C_START(void);
	void I2C_STOP(void);
	int I2C_GetACK(void);
	void I2C_SetACK(void);
	void I2C_SetNAK(void);
	void I2C_WriteByte(unsigned char sdata);
	unsigned char  I2C_ReadByte(void);
        
#endif
