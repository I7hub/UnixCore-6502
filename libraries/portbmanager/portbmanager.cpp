#include "portbmanager.h"

unsigned int DATADRB = 0x00;

void MODER_PORTB() {

  byte data = (byte) ((GPIOJ->IDR & IDRPORTJMask) << 1) + ((GPIOG->IDR & IDRPORTGMask) >> 13);

  GPIOD->ODR = bitRead(DATADRB, 0) ? (unsigned short) (0x0000 | (GPIOD->ODR & 0xDFFF)) : GPIOD->ODR;
  GPIOB->ODR = bitRead(DATADRB, 1) ? (unsigned short) (0x0000 | (GPIOB->ODR & 0xFFEF)) : GPIOB->ODR;
  GPIOB->ODR = bitRead(DATADRB, 2) ? (unsigned short) (0x0000 | (GPIOB->ODR & 0xFEFF)) : GPIOB->ODR;
  GPIOB->ODR = bitRead(DATADRB, 3) ? (unsigned short) (0x0000 | (GPIOB->ODR & 0xFDFF)) : GPIOB->ODR;
  GPIOK->ODR = bitRead(DATADRB, 4) ? (unsigned short) (0x0000 | (GPIOK->ODR & 0xFFFD)) : GPIOK->ODR;
  GPIOJ->ODR = bitRead(DATADRB, 5) ? (unsigned short) (0x0000 | (GPIOJ->ODR & 0xFBFF)) : GPIOJ->ODR;
  GPIOJ->ODR = bitRead(DATADRB, 6) ? (unsigned short) (0x0000 | (GPIOJ->ODR & 0xF7FF)) : GPIOJ->ODR;
  GPIOH->ODR = bitRead(DATADRB, 7) ? (unsigned short) (0x0000 | (GPIOH->ODR & 0xFFBF)) : GPIOH->ODR;
  DATADRB = data;
  GPIOD->MODER = bitRead(data, 0) ? (0x04000000 | (GPIOD->MODER & ~0x0C000000)) : (0x00000000 | (GPIOD->MODER & ~0x0C000000));
  GPIOB->MODER = bitRead(data, 1) ? (0x00000100 | (GPIOB->MODER & ~0x00000300)) : (0x00000000 | (GPIOB->MODER & ~0x00000300));
  GPIOB->MODER = bitRead(data, 2) ? (0x00010000 | (GPIOB->MODER & ~0x00030000)) : (0x00000000 | (GPIOB->MODER & ~0x00030000));
  GPIOB->MODER = bitRead(data, 3) ? (0x00040000 | (GPIOB->MODER & ~0x000C0000)) : (0x00000000 | (GPIOB->MODER & ~0x000C0000));
  GPIOK->MODER = bitRead(data, 4) ? (0x00000004 | (GPIOK->MODER & ~0x0000000C)) : (0x00000000 | (GPIOK->MODER & ~0x0000000C));
  GPIOJ->MODER = bitRead(data, 5) ? (0x00100000 | (GPIOJ->MODER & ~0x00300000)) : (0x00000000 | (GPIOJ->MODER & ~0x00300000));
  GPIOJ->MODER = bitRead(data, 6) ? (0x00400000 | (GPIOJ->MODER & ~0x00C00000)) : (0x00000000 | (GPIOJ->MODER & ~0x00C00000));
  GPIOH->MODER = bitRead(data, 7) ? (0x00001000 | (GPIOH->MODER & ~0x00003000)) : (0x00000000 | (GPIOH->MODER & ~0x00003000));

}

void OUT_PORTB() {

  byte data = (byte) ((GPIOJ->IDR & IDRPORTJMask) << 1) + ((GPIOG->IDR & IDRPORTGMask) >> 13);
  
  GPIOD->ODR = bitRead(DATADRB, 0) ? (unsigned short) (((data & 0x01) << 13) | (GPIOD->ODR & 0xDFFF)) : GPIOD->ODR;
  GPIOB->ODR = bitRead(DATADRB, 1) ? (unsigned short) (((data & 0x02) << 3) | (GPIOB->ODR & 0xFFEF)) : GPIOB->ODR;
  GPIOB->ODR = bitRead(DATADRB, 2) ? (unsigned short) (((data & 0x04) << 6) | (GPIOB->ODR & 0xFEFF)) : GPIOB->ODR;
  GPIOB->ODR = bitRead(DATADRB, 3) ? (unsigned short) (((data & 0x08) << 6) | (GPIOB->ODR & 0xFDFF)) : GPIOB->ODR;
  GPIOK->ODR = bitRead(DATADRB, 4) ? (unsigned short) (((data & 0x10) >> 3) | (GPIOK->ODR & 0xFFFD)) : GPIOK->ODR;
  GPIOJ->ODR = bitRead(DATADRB, 5) ? (unsigned short) (((data & 0x20) << 5) | (GPIOJ->ODR & 0xFBFF)) : GPIOJ->ODR;
  GPIOJ->ODR = bitRead(DATADRB, 6) ? (unsigned short) (((data & 0x40) << 5) | (GPIOJ->ODR & 0xF7FF)) : GPIOJ->ODR;
  GPIOH->ODR = bitRead(DATADRB, 7) ? (unsigned short) (((data & 0x80) >> 1) | (GPIOH->ODR & 0xFFBF)) : GPIOH->ODR;
  
}

void IN_PORTB() {}

void SETUP_PORTB() {

  pinMode(MODER_PORTB_IRQ,INPUT);

  pinMode(OUT_PORTB_IRQ,INPUT);
  pinMode(IN_PORTB_IRQ,INPUT);

  GPIOD->MODER = (0x00000000 | (GPIOD->MODER & ~0x0C000000));
  GPIOB->MODER = (0x00000000 | (GPIOB->MODER & ~0x00000300));
  GPIOB->MODER = (0x00000000 | (GPIOB->MODER & ~0x00030000));
  GPIOB->MODER = (0x00000000 | (GPIOB->MODER & ~0x000C0000));
  GPIOK->MODER = (0x00000000 | (GPIOK->MODER & ~0x0000000C));
  GPIOJ->MODER = (0x00000000 | (GPIOJ->MODER & ~0x00300000));
  GPIOJ->MODER = (0x00000000 | (GPIOJ->MODER & ~0x00C00000));
  GPIOH->MODER = (0x00000000 | (GPIOH->MODER & ~0x00003000));
  
  attachInterrupt(digitalPinToInterrupt(MODER_PORTB_IRQ), MODER_PORTB, RISING);

  attachInterrupt(digitalPinToInterrupt(OUT_PORTB_IRQ), OUT_PORTB, RISING);
  attachInterrupt(digitalPinToInterrupt(IN_PORTB_IRQ), IN_PORTB, RISING);

}
