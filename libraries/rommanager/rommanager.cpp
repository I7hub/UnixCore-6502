#include "rommanager.h"

byte lowrom[16384] = {};
byte highrom[16384];

// (address >= 0x8000) && (address <= 0xBFFF)
void LROM_read() {

  unsigned int address = (unsigned int) 0x8000 + ((GPIOJ->IDR & 0x4000) >> 1) + ((GPIOJ->IDR & 0x8000) >> 3) + ((GPIOK->IDR & 0x0008) << 8) + ((GPIOK->IDR & 0x0010) << 6) + ((GPIOK->IDR & 0x0020) << 4) + ((GPIOK->IDR & 0x0040) << 2) + (GPIOJ->IDR & 0x0080) + (GPIOE->IDR & 0x0040) + ((GPIOI->IDR & 0x8000) >> 10) + ((GPIOG->IDR & 0x0400) >> 6) + ((GPIOH->IDR & 0x8000) >> 12) + ((GPIOK->IDR & 0x0001) << 2) + ((GPIOI->IDR & 0x0800) >> 10) + ((GPIOK->IDR & 0x0004) >> 2);
  
  GPIOJ->MODER = (0x00001555 | (GPIOJ->MODER & MODERPORTJMask));
  GPIOG->MODER = (0x04000000 | (GPIOG->MODER & MODERPORTGMask));

  R_memblock(address, 0x8000, lowrom);

}

// (address >= 0xC000) && (address <= 0xFFFF)
void HROM_read() {

  unsigned int address = (unsigned int) 0xC000 + ((GPIOJ->IDR & 0x4000) >> 1) + ((GPIOJ->IDR & 0x8000) >> 3) + ((GPIOK->IDR & 0x0008) << 8) + ((GPIOK->IDR & 0x0010) << 6) + ((GPIOK->IDR & 0x0020) << 4) + ((GPIOK->IDR & 0x0040) << 2) + (GPIOJ->IDR & 0x0080) + (GPIOE->IDR & 0x0040) + ((GPIOI->IDR & 0x8000) >> 10) + ((GPIOG->IDR & 0x0400) >> 6) + ((GPIOH->IDR & 0x8000) >> 12) + ((GPIOK->IDR & 0x0001) << 2) + ((GPIOI->IDR & 0x0800) >> 10) + ((GPIOK->IDR & 0x0004) >> 2);
  
  GPIOJ->MODER = (0x00001555 | (GPIOJ->MODER & MODERPORTJMask));
  GPIOG->MODER = (0x04000000 | (GPIOG->MODER & MODERPORTGMask));

  R_memblock(address, 0xC000, highrom);

}

void ROM_setup() {

  pinMode(LROM_RD_IRQ, INPUT);
  pinMode(HROM_RD_IRQ, INPUT);

  if ( SIZE < 16384 ) {

    memset(&lowrom[SIZE], 0x00, sizeof(byte)*(16384-SIZE));

  }
  
  memset(highrom, 0x00, sizeof(byte)*16384);

  highrom[16381] = 0x80;

  attachInterrupt(digitalPinToInterrupt(LROM_RD_IRQ), LROM_read, RISING);
  attachInterrupt(digitalPinToInterrupt(HROM_RD_IRQ), HROM_read, RISING);

}
