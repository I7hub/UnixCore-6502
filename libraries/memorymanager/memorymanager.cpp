#include "memorymanager.h"

void R_memblock(unsigned int address, unsigned int reference, byte memblock[]) {

  unsigned int offset = address - reference;

  GPIOJ->ODR = (unsigned short) (((memblock[offset] & 0x00FE) >> 1) | (GPIOJ->ODR & ODRPORTJMask));
  GPIOG->ODR = (unsigned short) (((memblock[offset] & 0x0001) << 13) | (GPIOG->ODR & ODRPORTGMask));

}

void W_memblock(unsigned int address, unsigned int reference, byte memblock[]) {

  unsigned int offset = address - reference;

  memblock[offset] = (byte) ((GPIOJ->IDR & IDRPORTJMask) << 1) + ((GPIOG->IDR & IDRPORTGMask) >> 13);
  
}