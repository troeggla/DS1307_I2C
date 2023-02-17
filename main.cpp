#include <Arduino.h>

#define SDA A4
#define SCL A5

byte deviceId = 0b1101000;

void sendStart() {
  digitalWrite(SCL, HIGH);

  digitalWrite(SDA, HIGH);
  digitalWrite(SDA, LOW);

  delay(10);
  digitalWrite(SCL, LOW);
}

void sendStop() {
  digitalWrite(SCL, HIGH);

  digitalWrite(SDA, LOW);
  delay(10);
  digitalWrite(SDA, HIGH);
}

void setup() {
  Serial.begin(115200);

  pinMode(SDA, OUTPUT);
  pinMode(SCL, OUTPUT);
}

void loop() {
}
