#define CPU_CLOCK 8000000

#define delay_us(us) __delay_cycles(CPU_CLOCK/1000000*(us))

#define delay_ms(ms) __delay_cycles(CPU_CLOCK/1000*(ms))