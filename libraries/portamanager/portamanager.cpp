#include "portamanager.h"

void OUT_PORTA() {

  byte data = (byte) ((GPIOJ->IDR & IDRPORTJMask) << 1) + ((GPIOG->IDR & IDRPORTGMask) >> 13);

  GPIOH->ODR = bitRead(DATADRA, 0) ? (unsigned short) (((data & 0x01) << 4) | (GPIOH->ODR & 0xFFEF)) : GPIOH->ODR;
  GPIOB->ODR = bitRead(DATADRA, 1) ? (unsigned short) (((data & 0x02) << 10) | (GPIOB->ODR & 0xF7FF)) : GPIOB->ODR;
  GPIOD->ODR = bitRead(DATADRA, 2) ? (unsigned short) (((data & 0x04) << 4) | (GPIOD->ODR & 0xFFBF)) : GPIOD->ODR;
  GPIOD->ODR = bitRead(DATADRA, 3) ? (unsigned short) (((data & 0x08) << 2) | (GPIOD->ODR & 0xFFDF)) : GPIOD->ODR;      
  GPIOI->ODR = bitRead(DATADRA, 4) ? (unsigned short) (((data & 0x10) << 5) | (GPIOI->ODR & 0xFDFF)) : GPIOI->ODR;      
  GPIOH->ODR = bitRead(DATADRA, 5) ? (unsigned short) (((data & 0x20) << 8) | (GPIOH->ODR & 0xDFFF)) : GPIOH->ODR;      
  GPIOC->ODR = bitRead(DATADRA, 6) ? (unsigned short) (((data & 0x40) << 1) | (GPIOC->ODR & 0xFF7F)) : GPIOC->ODR;      
  GPIOG->ODR = bitRead(DATADRA, 7) ? (unsigned short) (((data & 0x80) << 7) | (GPIOG->ODR & 0xBFFF)) : GPIOG->ODR;
  
}

void IN_PORTA() {}

void SETUP_PORTA() {

  pinMode(OUT_PORTA_IRQ,INPUT);
  pinMode(IN_PORTA_IRQ,INPUT);

  GPIOH->MODER = bitRead(DATADRA, 0) ? (0x00000100 | (GPIOH->MODER & ~0x00000300)) : (0x00000000 | (GPIOH->MODER & ~0x00000300));
  GPIOB->MODER = bitRead(DATADRA, 1) ? (0x00400000 | (GPIOB->MODER & ~0x00C00000)) : (0x00000000 | (GPIOB->MODER & ~0x00C00000));
  GPIOD->MODER = bitRead(DATADRA, 2) ? (0x00001000 | (GPIOD->MODER & ~0x00003000)) : (0x00000000 | (GPIOD->MODER & ~0x00003000));      
  GPIOD->MODER = bitRead(DATADRA, 3) ? (0x00000400 | (GPIOD->MODER & ~0x00000C00)) : (0x00000000 | (GPIOD->MODER & ~0x00000C00));      
  GPIOI->MODER = bitRead(DATADRA, 4) ? (0x00040000 | (GPIOI->MODER & ~0x000C0000)) : (0x00000000 | (GPIOI->MODER & ~0x000C0000));      
  GPIOH->MODER = bitRead(DATADRA, 5) ? (0x04000000 | (GPIOH->MODER & ~0x0C000000)) : (0x00000000 | (GPIOH->MODER & ~0x0C000000));      
  GPIOC->MODER = bitRead(DATADRA, 6) ? (0x00004000 | (GPIOC->MODER & ~0x0000C000)) : (0x00000000 | (GPIOC->MODER & ~0x0000C000));
  GPIOG->MODER = bitRead(DATADRA, 7) ? (0x10000000 | (GPIOG->MODER & ~0x30000000)) : (0x00000000 | (GPIOG->MODER & ~0x30000000));
  
  attachInterrupt(digitalPinToInterrupt(OUT_PORTA_IRQ), OUT_PORTA, RISING);
  attachInterrupt(digitalPinToInterrupt(IN_PORTA_IRQ), IN_PORTA, RISING);

}
