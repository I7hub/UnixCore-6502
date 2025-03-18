#include "keyboardmanager.h"

Keyboard keyb;

void keyboard_setup() {

    pinMode(PA_15, OUTPUT);
    keyb.begin();

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

    switch (_key.keys[0]) {
      
      case 0x2d:
        _key.keys[0] = 0x2e;
        break;
      case 0x2f:
        _key.keys[0] = 0x2a;
        break;
      case 0x34:
        _key.keys[0] = 0x28;
        break;
      case 0x33:
        _key.keys[0] = 0x31;
        break;
      case 0x65:
        _key.keys[0] = 0x35;
        break;
      default:
        break;

    }

    *c = keyb.getAscii(_key);

  } else {

    *c = 0;
    
  }

}