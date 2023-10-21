#include <RPC.h>

#define PinRESET 4
#define ClockSigOUT 5

void setup() {
  RPC.begin();
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

void loop() {
  digitalWrite(ClockSigOUT, HIGH);
  delay(1000);
  digitalWrite(ClockSigOUT, LOW);
  delay(1000);
}
