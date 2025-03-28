#include "busmanager.h"

void BUS_write() {

  byte data = (byte) ((GPIOJ->IDR & IDRPORTJMask) << 1) + ((GPIOG->IDR & IDRPORTGMask) >> 13);
  
  GPIOD->ODR = (unsigned short) (((data & 0x01) << 13) | (GPIOD->ODR & 0xDFFF));
  GPIOB->ODR = (unsigned short) (((data & 0x02) << 3) | (GPIOB->ODR & 0xFFEF));
  GPIOB->ODR = (unsigned short) (((data & 0x04) << 6) | (GPIOB->ODR & 0xFEFF));
  GPIOB->ODR = (unsigned short) (((data & 0x08) << 6) | (GPIOB->ODR & 0xFDFF));
  GPIOK->ODR = (unsigned short) (((data & 0x10) >> 3) | (GPIOK->ODR & 0xFFFD));
  GPIOJ->ODR = (unsigned short) (((data & 0x20) << 5) | (GPIOJ->ODR & 0xFBFF));
  GPIOJ->ODR = (unsigned short) (((data & 0x40) << 5) | (GPIOJ->ODR & 0xF7FF));
  GPIOH->ODR = (unsigned short) (((data & 0x80) >> 1) | (GPIOH->ODR & 0xFFBF));
  
}

void BUS_setup() {

  pinMode(BUS_IRQ, INPUT);

  GPIOD->MODER = (0x04000000 | (GPIOD->MODER & ~0x0C000000));
  GPIOB->MODER = (0x00000100 | (GPIOB->MODER & ~0x00000300));
  GPIOB->MODER = (0x00010000 | (GPIOB->MODER & ~0x00030000));
  GPIOB->MODER = (0x00040000 | (GPIOB->MODER & ~0x000C0000));
  GPIOK->MODER = (0x00000004 | (GPIOK->MODER & ~0x0000000C));
  GPIOJ->MODER = (0x00100000 | (GPIOJ->MODER & ~0x00300000));
  GPIOJ->MODER = (0x00400000 | (GPIOJ->MODER & ~0x00C00000));
  GPIOH->MODER = (0x00001000 | (GPIOH->MODER & ~0x00003000));

  attachInterrupt(digitalPinToInterrupt(BUS_IRQ), BUS_write, RISING);

}
