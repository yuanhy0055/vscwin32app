#ifndef _set_dco_h_
#define _set_dco_h_
#define DELTA_1MHZ    244                   // 244 x 4096Hz = 999.4Hz
#define DELTA_8MHZ    1953                  // 1953 x 4096Hz = 7.99MHz
#define DELTA_12MHZ   2930                  // 2930 x 4096Hz = 12.00MHz
#define DELTA_16MHZ   3906                  // 3906 x 4096Hz = 15.99MHz

extern unsigned char CAL_DATA[8];                  // Temp. storage for constants
//extern volatile unsigned int i;
extern int j;
extern char *Flash_ptrA;                           // Segment A pointer

void Set_DCO(unsigned int Delta);            // Set DCO to selected frequency
void Reprograms_Dco(void);

#endif