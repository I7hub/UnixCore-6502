/* PIN 22 = A15, PIN 24 = A14, PIN 26 = A13, PIN 28 = A12, PIN 30 = A11, PIN 32 = A10, PIN 34 = A9, PIN 36 = A8, PIN 38 = A7, PIN 40 = A6, PIN 42 = A5, PIN 44 = A4, PIN 46 = A3, PIN 48 = A2, PIN 50 = A1, PIN 52 = A0 */
/* PIN 23 = D0, PIN 25 = D1, PIN 27 = D2, PIN 29 = D3, PIN 31 = D4, PIN 33 = D5, PIN 35 = D6, PIN 37 = D7 */
/* PIN 6 = BUS0, PIN 7 = BUS1, PIN 8 = BUS2, PIN 9 = BUS3, PIN 10 = BUS4, PIN 11 = BUS5, PIN 12 = BUS6, PIN 13 = BUS7 */
/* PIN 2 = ZP_RD_IRQ, PIN 3 = ZP_WR_IRQ, PIN 14 = STK_RD_IRQ, PIN 15 = STK_WR_IRQ, PIN 16 = FRAM_RD_IRQ, PIN 17 = FRAM_WR_IRQ, PIN 18 = BUS_IRQ, PIN 19 = KEYBD_IRQ, PIN 20 = LROM_RD_IRQ, PIN 21 = HROM_RD_IRQ */

#include "zpmanager.h"
#include "stkmanager.h"
#include "frammanager.h"
#include "rommanager.h"

#include "keyboardmanager.h"
#include "busmanager.h"

#define AddressPins 16
#define DataPins 8

#define ResetPin 4

const char ADDR_PIN[] = {22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52};
const char DATA_PIN[] = {23, 25, 27, 29, 31, 33, 35, 37};

void setup() {

  Serial.begin(9600);

  for(int i = 0; i < AddressPins; i++) {
    pinMode(ADDR_PIN[i], INPUT);
  }

  for(int i = 0; i < DataPins; i++) {
    pinMode(DATA_PIN[i], INPUT);
  }

  ZP_setup();
  STK_setup();
  FRAM_setup();
  ROM_setup();

  KEYBD_setup();
  BUS_setup();
  
  pinMode(ResetPin, OUTPUT);
  reset();

}

void loop() {

  KEYBD_handle();

}

void reset() {

  digitalWrite(ResetPin, HIGH);
  delay(1000);
  digitalWrite(ResetPin, LOW);
  delay(1000);
  digitalWrite(ResetPin, HIGH);

}
