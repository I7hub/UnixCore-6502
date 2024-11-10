#ifndef KEYBOARDMANAGER_H
#define KEYBOARDMANAGER_H

#include "USBHostGiga.h"

extern Keyboard keyb;

void keyboard_setup();
void keyboard_handler(char *c, boolean *ctrl);

#endif
