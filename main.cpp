#include <Arduino.h>

#define SDA A4
#define SCL A5

byte deviceId = 0b1101000;

void setup() {
  Serial.begin(115200);

  pinMode(SDA, OUTPUT);
  pinMode(SCL, OUTPUT);
}

void loop() {
}
