/*--------------------------------------------------------------
 File:      interrupts.h
 Purpose:   Header file for interrupt management
 Compiler:  IAR EW 5.5
 System:    Philips LPC2148 MCU with ARM7TDMI-s core
----------------------------------------------------------------*/

//If this file is not included by another program, define it here (avoids multiple definitions)  
#ifndef   __INTERRUPTS_H
#define   __INTERRUPTS_H

#include <intrinsics.h>
//Macros
#define INT_NUMBERS   32   //Total Interrupt Sources for ARM7
#define VIC_CHANNELS  16   //IRQ Priority Leves

//Function Prototypes
void VIC_init(void);

void install_IRQ(unsigned int IntNumber,  void (*ISR)(void), unsigned int channel);

void install_FIQ(unsigned int IntNumber,  void (*ISR)(void));

#endif //__INTERRUPTS_H
