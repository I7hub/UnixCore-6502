#ifndef PORTBMANAGER_H
#define PORTBMANAGER_H

#define IDRPORTJMask 0x007F
#define IDRPORTGMask 0x2000

#define MODER_PORTB_IRQ 57

#define OUT_PORTB_IRQ 61
#define IN_PORTB_IRQ 70

#include "Arduino.h"

extern unsigned int DATADRB;

void MODER_PORTB();
void OUT_PORTB();
void IN_PORTB();
void SETUP_PORTB();

#endif
