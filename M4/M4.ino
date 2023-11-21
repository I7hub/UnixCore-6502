#include <RPC.h>
#include "USBHostGiga.h"

#define PinRESET 4
#define ClockSigOUT 5

Keyboard keyb;

void setup() {
  RPC.begin();
  pinMode(PA_15, OUTPUT);
  keyb.begin();
  pinMode(ClockSigOUT, OUTPUT);
  pinMode(PinRESET, OUTPUT);
  reset();
}

void reset() {
  digitalWrite(PinRESET, HIGH);
  delay(1000);
  digitalWrite(PinRESET, LOW);
  delay(1000);
  digitalWrite(PinRESET, HIGH);
}

void keyboard_handler(char *c, boolean *ctrl) {

  if (keyb.available()) {

    auto _key = keyb.read();
    
    _key.lshift = _key.lgui ? 1 : _key.lshift;
    _key.lgui = _key.lgui ? 0 : _key.lgui;

    _key.lctrl = _key.lalt ? 1 : _key.lctrl;
    _key.lalt = _key.lalt ? 0 : _key.lalt;

    _key.rctrl = _key.ralt ? 1 : _key.rctrl;
    _key.ralt = _key.ralt ? 0 : _key.ralt;

    *ctrl = (_key.lctrl | _key.rctrl) ? 1 : 0;

    _key.keys[0] = (_key.keys[0] == 0x2d) ? 0x2e : _key.keys[0];
    _key.keys[0] = (_key.keys[0] == 0x2f) ? 0x2a : _key.keys[0];
    _key.keys[0] = (_key.keys[0] == 0x34) ? 0x28 : _key.keys[0];
    _key.keys[0] = (_key.keys[0] == 0x33) ? 0x31 : _key.keys[0];
    _key.keys[0] = (_key.keys[0] == 0x65) ? 0x35 : _key.keys[0];

    *c = keyb.getAscii(_key);

  } else {

    *c = 0;
    
  }

}

void loop() {
  digitalWrite(ClockSigOUT, HIGH);
  delay(1000);
  digitalWrite(ClockSigOUT, LOW);
  delay(1000);
}
