#include "zpmanager.h"

byte zeropage[256];

// (address >= 0x0000) && (address <= 0x00FF)
void ZP_read() {

  unsigned int address = (unsigned int) (GPIOJ->IDR & 0x0080) + (GPIOE->IDR & 0x0040) + ((GPIOI->IDR & 0x8000) >> 10) + ((GPIOG->IDR & 0x0400) >> 6) + ((GPIOH->IDR & 0x8000) >> 12) + ((GPIOK->IDR & 0x0001) << 2) + ((GPIOI->IDR & 0x0800) >> 10) + ((GPIOK->IDR & 0x0004) >> 2);
  
  GPIOJ->MODER = (0x00001555 | (GPIOJ->MODER & MODERPORTJMask));
  GPIOG->MODER = (0x04000000 | (GPIOG->MODER & MODERPORTGMask));

  R_memblock(address, 0x0000, zeropage);

}

// (address >= 0x0000) && (address <= 0x00FF)
void ZP_write() {

  unsigned int address = (unsigned int) (GPIOJ->IDR & 0x0080) + (GPIOE->IDR & 0x0040) + ((GPIOI->IDR & 0x8000) >> 10) + ((GPIOG->IDR & 0x0400) >> 6) + ((GPIOH->IDR & 0x8000) >> 12) + ((GPIOK->IDR & 0x0001) << 2) + ((GPIOI->IDR & 0x0800) >> 10) + ((GPIOK->IDR & 0x0004) >> 2);
  
  GPIOJ->MODER = (0x00000000 | (GPIOJ->MODER & MODERPORTJMask));
  GPIOG->MODER = (0x00000000 | (GPIOG->MODER & MODERPORTGMask));

  W_memblock(address, 0x0000, zeropage);

}

void ZP_setup() {

  pinMode(ZP_RD_IRQ, INPUT);
  pinMode(ZP_WR_IRQ, INPUT);

  memset(zeropage, 0x00, sizeof(byte)*256);
  
  attachInterrupt(digitalPinToInterrupt(ZP_RD_IRQ), ZP_read, RISING);
  attachInterrupt(digitalPinToInterrupt(ZP_WR_IRQ), ZP_write, RISING);

}
