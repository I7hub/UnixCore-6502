#ifndef ZPMANAGER_H
#define ZPMANAGER_H

#include "memorymanager.h"
#include "Arduino.h"

#define ZP_RD_IRQ 2
#define ZP_WR_IRQ 3

extern byte zeropage[256];

void ZP_read();
void ZP_write();
void ZP_setup();

#endif
