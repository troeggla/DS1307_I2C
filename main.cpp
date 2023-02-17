#include <Arduino.h>
#include "i2c_device/i2c_device.h"

byte deviceId = 0b1101000;
I2CDevice device(deviceId, A4, A5);

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

void printTime() {
  byte result[8];
  bool success = device.readBytesStartingAt(0x00, 8, result);

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

  uint8_t* readByte = (uint8_t*)malloc(sizeof(uint8_t));

  Serial.print("Byte at address 0x00: 0b");
  device.readByteAtAddress(0x00, readByte);
  Serial.println(*readByte, BIN);

  Serial.print("Byte at address 0x07: 0b");
  device.readByteAtAddress(0x07, readByte);
  Serial.println(*readByte, BIN);

  free(readByte);

  Serial.print("Writing data at 0x00: ");
  Serial.println((device.writeByteAtAddress(0x00, 0b00000000) == true) ? "true" : "false");

  byte data[7] = { 0, convertToBcd(59), convertToBcd(23), 0, convertToBcd(28), convertToBcd(2), convertToBcd(23) };

  Serial.print("Writing block data at 0x00: ");
  Serial.println((device.writeBytesStartingAt(0x00, 7, data) == true) ? "true" : "false");

  char txt[] = "Hello World";
  Serial.print("Writing data at 0x10: ");
  Serial.println((device.writeBytesStartingAt(0x10, strlen(txt), (byte*)txt) == true) ? "true" : "false");

  byte buf[12];
  device.readBytesStartingAt(0x10, 11, buf);
  buf[11] = '\0';

  Serial.print("Data read from 0x10: ");
  Serial.println(String((char*)buf));

  Serial.print("Enabling oscillator: ");
  Serial.println((device.writeByteAtAddress(0x07, 0b00010000) == true) ? "true" : "false");

  Serial.println();

  pinMode(2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), printTime, RISING);
}

void loop() {
}
