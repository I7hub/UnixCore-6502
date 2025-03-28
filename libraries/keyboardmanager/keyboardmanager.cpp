#include "keyboardmanager.h"

Keyboard keyb;
HID_KEYBD_Info_TypeDef _key;
byte modFlags;
byte keyFlow;

void KEYBD_handle() {

  if (keyb.available() && digitalRead(IRQB)) {

    _key = keyb.read();
    modFlags = ((_key.lalt | _key.ralt) << 1) | _key.lgui | 0x80;
    keyFlow = (modFlags == 0x80) ? _key.keys[0] : modFlags;
    GPIOA->ODR = (_key.keys[0]) ? (unsigned short) GPIOA->ODR & 0xFF7F : GPIOA->ODR;
  
  }

}

void KEYBD_read() {

  GPIOJ->ODR = (unsigned short) (((keyFlow & 0x00FE) >> 1) | (GPIOJ->ODR & ODRPORTJMask));
  GPIOG->ODR = (unsigned short) (((keyFlow & 0x0001) << 13) | (GPIOG->ODR & ODRPORTGMask));

  GPIOA->ODR = (modFlags == 0x80) ? (unsigned short) GPIOA->ODR | 0x0080 : GPIOA->ODR;
  keyFlow = (modFlags == 0x80) ? 0x00 : _key.keys[0];
  modFlags = 0x80; 

}

void KEYBD_setup() {

  pinMode(KEYBD_IRQ, INPUT);

  pinMode(IRQB, OUTPUT);
  digitalWrite(IRQB, HIGH);

  pinMode(PA_15, OUTPUT);
  keyb.begin();
  
  attachInterrupt(digitalPinToInterrupt(KEYBD_IRQ), KEYBD_read, RISING);

}
