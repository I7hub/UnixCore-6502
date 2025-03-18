#ifndef PORTAMANAGER_H
#define PORTAMANAGER_H

#define IDRPORTJMask 0x007F
#define IDRPORTGMask 0x2000

#define OUT_PORTA_IRQ 59
#define IN_PORTA_IRQ 63

#define DATADRA 0x00

#include "Arduino.h"

void OUT_PORTA();
void IN_PORTA();
void SETUP_PORTA();

#endif
