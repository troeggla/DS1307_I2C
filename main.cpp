#include <Arduino.h>
#include "real_time_clock/ds1307.h"

DS1307 clock(A4, A5);

void printTime() {
  Serial.println(clock.getFormattedDateTime());
}

void setup() {
  Serial.begin(115200);

  clock.enableClock(false);
  clock.setDateTime(2023, 02, 28, 23, 59, 30);
  clock.enableOscillator(true, OscillatorFreq::FREQ_1HZ);
  clock.enableClock(true);

  pinMode(2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), printTime, RISING);
}

void loop() {
}
