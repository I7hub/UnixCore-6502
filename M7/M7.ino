/* PIN 22 = A15, PIN 24 = A14, PIN 26 = A13, PIN 28 = A12, PIN 30 = A11, PIN 32 = A10, PIN 34 = A9, PIN 36 = A8, PIN 38 = A7, PIN 40 = A6, PIN 42 = A5, PIN 44 = A4, PIN 46 = A3, PIN 48 = A2, PIN 50 = A1, PIN 52 = A0 */
/* PIN 23 = D0, PIN 25 = D1, PIN 27 = D2, PIN 29 = D3, PIN 31 = D4, PIN 33 = D5, PIN 35 = D6, PIN 37 = D7 */
/* PIN 21 = PA0, PIN 20 = PA1, PIN 19 = PA2, PIN 18 = PA3, PIN 17 = PA4, PIN 16 = PA5, PIN 15 = PA6, PIN 14 = PA7 */
/* PIN 6 = PB0, PIN 7 = PB1, PIN 8 = PB2, PIN 9 = PB3, PIN 10 = PB4, PIN 11 = PB5, PIN 12 = PB6, PIN 13 = PB7 */
/* PIN 43 = ZP_RD_IRQ, PIN 45 = ZP_WR_IRQ, PIN 47 = STK_RD_IRQ, PIN 49 = STK_WR_IRQ, PIN 51 = FRAM_RD_IRQ, PIN 53 = FRAM_WR_IRQ, PIN 59 = OUT_PORTA_IRQ, PIN 61 = OUT_PORTB_IRQ, PIN 63 = IN_PORTA_IRQ, PIN 70 = IN_PORTB_IRQ, PIN 72 = LROM_RD_IRQ, PIN 74 = HROM_RD_IRQ */

#include <RPC.h>

#include "zpmanager.h"
#include "stkmanager.h"
#include "frammanager.h"
#include "portamanager.h"
#include "portbmanager.h"
#include "rommanager.h"

#define AddressPins 16
#define DataPins 8

const char ADDR_PIN[] = {22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52};
const char DATA_PIN[] = {23, 25, 27, 29, 31, 33, 35, 37};

void setup() {

  RPC.begin();

  for(int i = 0; i < AddressPins; i++) {
    pinMode(ADDR_PIN[i],INPUT);
  }

  for(int i = 0; i < DataPins; i++) {
    pinMode(DATA_PIN[i],INPUT);
  }

  RCC->AHB4ENR |= 0x0000000e;

  ZP_setup();
  STK_setup();
  FRAM_setup();

  SETUP_PORTA();
  SETUP_PORTB();

  ROM_setup();

}

void loop() {}
