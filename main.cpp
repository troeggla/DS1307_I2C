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

void sendBit(int bit) {
  if (bit == 0) {
    digitalWrite(SDA, LOW);
  } else {
    digitalWrite(SDA, HIGH);
  }

  digitalWrite(SCL, HIGH);
  delayMicroseconds(10);
  digitalWrite(SCL, LOW);
  delayMicroseconds(10);
}

void sendDeviceId(byte address) {
  for (int i=0; i<7; i++) {
    int bit = (address << i) & 0x40;
    sendBit(bit);
  }
}

bool readAck() {
  pinMode(SDA, INPUT);
  digitalWrite(SCL, HIGH);
  delayMicroseconds(10);

  int result = digitalRead(SDA);
  digitalWrite(SCL, LOW);
  delayMicroseconds(10);

  pinMode(SDA, OUTPUT);
  return result == 0;
}

void sendByte(byte data) {
  for (int i=0; i<8; i++) {
    int bit = (data << i) & 0x80;
    sendBit(bit);
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(SDA, OUTPUT);
  pinMode(SCL, OUTPUT);
}

void loop() {
}
