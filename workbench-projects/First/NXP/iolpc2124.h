#ifndef __IOLPC2124_H
#define __IOLPC2124_H

// GPIO Registers
#define IO0PIN  (*((volatile unsigned long *) 0xE0028000))  // Pin Value Register
#define IO0SET  (*((volatile unsigned long *) 0xE0028004))  // Set Register
#define IO0DIR  (*((volatile unsigned long *) 0xE0028008))  // Direction Register
#define IO0CLR  (*((volatile unsigned long *) 0xE002800C))  // Clear Register

// Pin Connect Block
#define PINSEL0 (*((volatile unsigned long *) 0xE002C000))  // Pin Function Select Register 0

#endif 