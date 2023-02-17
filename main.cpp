#include <Arduino.h>

#define SDA A4
#define SCL A5

byte deviceId = 0b1101000;

int convertFromBcd(int bcd) {
  int tens = ((bcd & 0b11110000) >> 4) * 10;
  int ones = bcd & 0b00001111;

  return tens + ones;
}

byte convertToBcd(int num) {
  int ones = num % 10;
  int tens = num / 10;

  return (tens << 4) + ones;
}

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

bool writeBytesStartingAt(byte deviceId, byte registerAddress, int numBytes, byte* data) {
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

  for (int i=0; i<numBytes; i++) {
    sendByte(data[i]);

    if (!readAck()) {
      return false;
    }
  }

  sendStop();
  return true;
}

bool readBytesStartingAt(byte deviceId, byte registerAddress, int numBytes, byte* result) {
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

  sendStart();
  sendDeviceId(0b1101000);
  sendBit(0b1);

  if (!readAck()) {
    return false;
  }

  for (int i=0; i<numBytes; i++) {
    byte data = readByte();
    result[i] = data;

    if (i != numBytes - 1) {
      sendBit(0b0);
    }
  }

  sendBit(0b1);
  sendStop();

  return true;
}

void printTime() {
  byte result[8];
  bool success = readBytesStartingAt(deviceId, 0x00, 8, result);

  if (success) {
    int seconds = convertFromBcd(result[0]);
    int minutes = convertFromBcd(result[1]);
    int hours = convertFromBcd(result[2]);
    // int day = convertFromBcd(result[3]);
    int date = convertFromBcd(result[4]);
    int month = convertFromBcd(result[5]);
    int year = convertFromBcd(result[6]);

    char datetime[20];
    sprintf(datetime, "20%02d-%02d-%02d %02d:%02d:%02d", year, month, date, hours, minutes, seconds);

    Serial.print("Datetime: ");
    Serial.println(datetime);
  }
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

  byte data[7] = { 0, convertToBcd(59), convertToBcd(23), 0, convertToBcd(28), convertToBcd(2), convertToBcd(23) };

  Serial.print("Writing block data at 0x00: ");
  Serial.println((writeBytesStartingAt(deviceId, 0x00, 7, data) == true) ? "true" : "false");

  char txt[] = "Hello World";
  Serial.print("Writing data at 0x10: ");
  Serial.println((writeBytesStartingAt(deviceId, 0x10, strlen(txt), (byte*)txt) == true) ? "true" : "false");

  byte buf[12];
  readBytesStartingAt(deviceId, 0x10, 11, buf);
  buf[11] = '\0';

  Serial.print("Data read from 0x10: ");
  Serial.println(String((char*)buf));

  Serial.print("Enabling oscillator: ");
  Serial.println((writeByteAtAddress(deviceId, 0x07, 0b00010000) == true) ? "true" : "false");

  Serial.println();
}

void loop() {
}
