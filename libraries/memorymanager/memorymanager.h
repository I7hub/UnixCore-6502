#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include "Arduino.h"

#define IDRPORTJMask 0x007F
#define IDRPORTGMask 0x2000

#define MODERPORTJMask 0xFFFFC000
#define MODERPORTGMask 0xF3FFFFFF

#define ODRPORTJMask 0xFF80
#define ODRPORTGMask 0xDFFF

void R_memblock(unsigned int address, unsigned int reference, byte memblock[]);
void W_memblock(unsigned int address, unsigned int reference, byte memblock[]);

#endif
