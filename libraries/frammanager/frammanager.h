#ifndef FRAMMANAGER_H
#define FRAMMANAGER_H

#include "memorymanager.h"
#include "Arduino.h"

#define FRAM_RD_IRQ 16
#define FRAM_WR_IRQ 17

extern byte freeram[24064];

void FRAM_read();
void FRAM_write();
void FRAM_setup();

#endif
