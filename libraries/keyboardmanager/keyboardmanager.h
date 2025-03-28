#ifndef KEYBOARDMANAGER_H
#define KEYBOARDMANAGER_H

#include "USBHostGiga.h"
#include "Arduino.h"

#define ODRPORTJMask 0xFF80
#define ODRPORTGMask 0xDFFF

#define KEYBD_IRQ 19
#define IRQB 5

extern Keyboard keyb;
extern HID_KEYBD_Info_TypeDef _key;
extern byte modFlags;
extern byte keyFlow;

void KEYBD_handle();
void KEYBD_read();
void KEYBD_setup();

#endif
