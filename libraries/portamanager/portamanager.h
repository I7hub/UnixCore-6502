#ifndef PORTAMANAGER_H
#define PORTAMANAGER_H

#define IDRPORTJMask 0x007F
#define IDRPORTGMask 0x2000

#define MODER_PORTA_IRQ 55

#define OUT_PORTA_IRQ 59
#define IN_PORTA_IRQ 63

#include "Arduino.h"

extern unsigned int DATADRA;

void MODER_PORTA();
void OUT_PORTA();
void IN_PORTA();
void SETUP_PORTA();

#endif
