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

int readByte() {
  pinMode(SDA, INPUT);

  int result = 0;

  for (int i=0; i<8; i++) {
    digitalWrite(SCL, HIGH);
    delayMicroseconds(10);

    int bit = digitalRead(SDA);
    result = (result << 1) + bit;

    digitalWrite(SCL, LOW);
    delayMicroseconds(10);
  }

  pinMode(SDA, OUTPUT);
  return result;
}

byte readByteAtAddress(byte deviceId, byte registerAddress) {
  sendStart();
  sendDeviceId(deviceId);
  sendBit(0b0);

  if (!readAck()) {
    return -1;
  }

  sendByte(registerAddress);

  if (!readAck()) {
    return -1;
  }

  sendStart();
  sendDeviceId(0b1101000);
  sendBit(0b1);

  if (!readAck()) {
    return -1;
  }

  int byte = readByte();

  sendBit(0b1);
  sendStop();

  return byte;
}

bool writeByteAtAddress(byte deviceId, byte registerAddress, byte data) {
  sendStart();
  sendDeviceId(deviceId);
  sendBit(0b0);

  if (!readAck()) {
    return false;
  }

  sendByte(registerAddress);

  if (!readAck()) {
    return false;
  }

  sendByte(data);

  if (!readAck()) {
    return false;
  }

  sendStop();
  return true;
}

void setup() {
  Serial.begin(115200);

  pinMode(SDA, OUTPUT);
  pinMode(SCL, OUTPUT);

  Serial.print("Byte at address 0x00: 0b");
  Serial.println(readByteAtAddress(deviceId, 0x00), BIN);

  Serial.print("Byte at address 0x07: 0b");
  Serial.println(readByteAtAddress(deviceId, 0x07), BIN);

  Serial.print("Writing data at 0x00: ");
  Serial.println((writeByteAtAddress(deviceId, 0x00, 0b01000000) == true) ? "true" : "false");
}

void loop() {
}
