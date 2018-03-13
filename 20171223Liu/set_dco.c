#include <msp430.h>
#include "set_dco.h"

unsigned char CAL_DATA[8];                  // Temp. storage for constants
//volatile unsigned int i;
int j;
char *Flash_ptrA;                           // Segment A pointer

void Set_DCO(unsigned int Delta)            // Set DCO to selected frequency
{
  unsigned int Compare, Oldcapture = 0;

  BCSCTL1 |= DIVA_3;                        // ACLK = LFXT1CLK/8
  TACCTL2 = CM_1 + CCIS_1 + CAP;            // CAP, ACLK
  TACTL = TASSEL_2 + MC_2 + TACLR;          // SMCLK, cont-mode, clear

  while (1)
  {
    while (!(CCIFG & TACCTL2));             // Wait until capture occured
    TACCTL2 &= ~CCIFG;                      // Capture occured, clear flag
    Compare = TACCR2;                       // Get current captured SMCLK
    Compare = Compare - Oldcapture;         // SMCLK difference
    Oldcapture = TACCR2;                    // Save current captured SMCLK

    if (Delta == Compare)
      break;                                // If equal, leave "while(1)"
    else if (Delta < Compare)
    {
      DCOCTL--;                             // DCO is too fast, slow it down
      if (DCOCTL == 0xFF)                   // Did DCO roll under?
        if (BCSCTL1 & 0x0f)
          BCSCTL1--;                        // Select lower RSEL
    }
    else
    {
      DCOCTL++;                             // DCO is too slow, speed it up
      if (DCOCTL == 0x00)                   // Did DCO roll over?
        if ((BCSCTL1 & 0x0f) != 0x0f)
          BCSCTL1++;                        // Sel higher RSEL
    }
  }
  TACCTL2 = 0;                              // Stop TACCR2
  TACTL = 0;                                // Stop Timer_A
  BCSCTL1 &= ~DIVA_3;                       // ACLK = LFXT1CLK
}

void Reprograms_Dco(void)
{

  Set_DCO(DELTA_16MHZ);                     // Set DCO and obtain constants
  CAL_DATA[j++] = DCOCTL;
  CAL_DATA[j++] = BCSCTL1;

  Set_DCO(DELTA_12MHZ);                     // Set DCO and obtain constants
  CAL_DATA[j++] = DCOCTL;
  CAL_DATA[j++] = BCSCTL1;

  Set_DCO(DELTA_8MHZ);                      // Set DCO and obtain constants
  CAL_DATA[j++] = DCOCTL;
  CAL_DATA[j++] = BCSCTL1;

  Set_DCO(DELTA_1MHZ);                      // Set DCO and obtain constants
  CAL_DATA[j++] = DCOCTL;
  CAL_DATA[j++] = BCSCTL1;

  Flash_ptrA = (char *)0x10C0;              // Point to beginning of seg A
  FCTL2 = FWKEY + FSSEL0 + FN1;             // MCLK/3 for Flash Timing Generator
  FCTL1 = FWKEY + ERASE;                    // Set Erase bit
  FCTL3 = FWKEY + LOCKA;                    // Clear LOCK & LOCKA bits
  *Flash_ptrA = 0x00;                       // Dummy write to erase Flash seg A
  FCTL1 = FWKEY + WRT;                      // Set WRT bit for write operation
  Flash_ptrA = (char *)0x10F8;              // Point to beginning of cal consts
  for (j = 0; j < 8; j++)
    *Flash_ptrA++ = CAL_DATA[j];            // re-flash DCO calibration data
  FCTL1 = FWKEY;                            // Clear WRT bit
  FCTL3 = FWKEY + LOCKA + LOCK;             // Set LOCK & LOCKA bit
}