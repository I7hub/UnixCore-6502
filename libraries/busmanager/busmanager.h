#ifndef BUSMANAGER_H
#define BUSMANAGER_H

#include "Arduino.h"

#define IDRPORTJMask 0x007F
#define IDRPORTGMask 0x2000

#define BUS_IRQ 18

void BUS_write();
void BUS_setup();

#endif
