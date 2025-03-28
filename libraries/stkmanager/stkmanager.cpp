#include "stkmanager.h"

byte stack[256];

// (address >= 0x0100) && (address <= 0x01FF)
void STK_read() {

  unsigned int address = (unsigned int) 0x0100 + (GPIOJ->IDR & 0x0080) + (GPIOE->IDR & 0x0040) + ((GPIOI->IDR & 0x8000) >> 10) + ((GPIOG->IDR & 0x0400) >> 6) + ((GPIOH->IDR & 0x8000) >> 12) + ((GPIOK->IDR & 0x0001) << 2) + ((GPIOI->IDR & 0x0800) >> 10) + ((GPIOK->IDR & 0x0004) >> 2);
  
  GPIOJ->MODER = (0x00001555 | (GPIOJ->MODER & MODERPORTJMask));
  GPIOG->MODER = (0x04000000 | (GPIOG->MODER & MODERPORTGMask));

  R_memblock(address, 0x0100, stack);

}

// (address >= 0x0100) && (address <= 0x01FF)
void STK_write() {

  unsigned int address = (unsigned int) 0x0100 + (GPIOJ->IDR & 0x0080) + (GPIOE->IDR & 0x0040) + ((GPIOI->IDR & 0x8000) >> 10) + ((GPIOG->IDR & 0x0400) >> 6) + ((GPIOH->IDR & 0x8000) >> 12) + ((GPIOK->IDR & 0x0001) << 2) + ((GPIOI->IDR & 0x0800) >> 10) + ((GPIOK->IDR & 0x0004) >> 2);
  
  GPIOJ->MODER = (0x00000000 | (GPIOJ->MODER & MODERPORTJMask));
  GPIOG->MODER = (0x00000000 | (GPIOG->MODER & MODERPORTGMask));

  W_memblock(address, 0x0100, stack);

}

void STK_setup() {

  pinMode(STK_RD_IRQ, INPUT);
  pinMode(STK_WR_IRQ, INPUT);

  memset(stack, 0x00, sizeof(byte)*256);
  
  attachInterrupt(digitalPinToInterrupt(STK_RD_IRQ), STK_read, RISING);
  attachInterrupt(digitalPinToInterrupt(STK_WR_IRQ), STK_write, RISING);

}
