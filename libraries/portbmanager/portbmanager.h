#ifndef PORTBMANAGER_H
#define PORTBMANAGER_H

#define IDRPORTJMask 0x007F
#define IDRPORTGMask 0x2000

#define OUT_PORTB_IRQ 61
#define IN_PORTB_IRQ 70

#define DATADRB 0x00

#include "Arduino.h"

void OUT_PORTB();
void IN_PORTB();
void SETUP_PORTB();

#endif
