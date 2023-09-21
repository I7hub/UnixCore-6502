#include <RPC.h>
#define RESET_PIN 4
#define CLOCKSIG 5

void setup() {
  RPC.begin();
  pinMode(CLOCKSIG, OUTPUT);
  pinMode(RESET_PIN, OUTPUT);
  reset();
}

void reset() {
  digitalWrite(RESET_PIN, HIGH);
  delay(1000);
  digitalWrite(RESET_PIN, LOW);
  delay(1000);
  digitalWrite(RESET_PIN, HIGH);
}

void loop() {
  digitalWrite(CLOCKSIG, HIGH);
  delay(1);
  digitalWrite(CLOCKSIG, LOW);
  delay(1);
}