#ifndef ROMMANAGER_H
#define ROMMANAGER_H

#include "memorymanager.h"
#include "Arduino.h"

#define SIZE 19

#define LROM_RD_IRQ 72
#define HROM_RD_IRQ 74

extern byte lowrom[16384];
extern byte highrom[16384];

void LROM_read();
void HROM_read();
void ROM_setup();

#endif
