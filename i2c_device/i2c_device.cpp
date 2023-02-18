#include "i2c_device.h"

I2CDevice::I2CDevice(uint8_t deviceAddress, uint8_t sdaPin, uint8_t sclPin) : deviceAddress(deviceAddress), sdaPin(sdaPin), sclPin(sclPin) {
  pinMode(sdaPin, OUTPUT);
  pinMode(sclPin, OUTPUT);
}

I2CDevice::I2CDevice(uint8_t deviceAddress) : I2CDevice(deviceAddress, SDA, SCL) {
}

void I2CDevice::sendStart() {
  digitalWrite(sclPin, HIGH);

  digitalWrite(sdaPin, HIGH);
  digitalWrite(sdaPin, LOW);

  delayMicroseconds(5);
  digitalWrite(sclPin, LOW);
}

void I2CDevice::sendStop() {
  digitalWrite(sclPin, HIGH);

  digitalWrite(sdaPin, LOW);
  digitalWrite(sdaPin, HIGH);

  delayMicroseconds(5);
  digitalWrite(sclPin, LOW);
  delayMicroseconds(5);
}

void I2CDevice::sendBit(uint8_t bit) {
  if (bit == 0) {
    digitalWrite(sdaPin, LOW);
  } else {
    digitalWrite(sdaPin, HIGH);
  }

  digitalWrite(sclPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(sclPin, LOW);
  delayMicroseconds(5);
}

void I2CDevice::sendNBits(uint8_t data, unsigned int numBits) {
  int mask = 1 << (numBits - 1);

  for (int i=0; i<numBits; i++) {
    int bit = (data << i) & mask;
    sendBit(bit);
  }
}

void I2CDevice::sendDeviceId() {
  sendNBits(deviceAddress, 7);
}

void I2CDevice::sendByte(uint8_t data) {
  sendNBits(data, 8);
}

bool I2CDevice::readAck() {
  pinMode(sdaPin, INPUT);
  digitalWrite(sclPin, HIGH);
  delayMicroseconds(5);

  int result = digitalRead(sdaPin);
  digitalWrite(sclPin, LOW);
  delayMicroseconds(5);

  pinMode(sdaPin, OUTPUT);
  return result == 0;
}

uint8_t I2CDevice::readByte() {
  pinMode(sdaPin, INPUT);

  int result = 0;

  for (int i=0; i<8; i++) {
    digitalWrite(sclPin, HIGH);
    delayMicroseconds(5);

    int bit = digitalRead(sdaPin);
    result = (result << 1) + bit;

    digitalWrite(sclPin, LOW);
    delayMicroseconds(5);
  }

  pinMode(sdaPin, OUTPUT);
  return result;
}

bool I2CDevice::writeByteAtAddress(uint8_t registerAddress, uint8_t data) {
  sendStart();
  sendDeviceId();
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

bool I2CDevice::writeBytesStartingAt(uint8_t registerAddress, size_t numBytes, uint8_t* data) {
  sendStart();
  sendDeviceId();
  sendBit(0b0);

  if (!readAck()) {
    return false;
  }

  sendByte(registerAddress);

  if (!readAck()) {
    return false;
  }

  for (uint8_t i=0; i<numBytes; i++) {
    sendByte(data[i]);

    if (!readAck()) {
      return false;
    }
  }

  sendStop();
  return true;
}

bool I2CDevice::readByteAtAddress(uint8_t registerAddress, uint8_t* result) {
  sendStart();
  sendDeviceId();
  sendBit(0b0);

  if (!readAck()) {
    return false;
  }

  sendByte(registerAddress);

  if (!readAck()) {
    return false;
  }

  sendStart();
  sendDeviceId();
  sendBit(0b1);

  if (!readAck()) {
    return false;
  }

  uint8_t byteResult = readByte();
  *result = byteResult;

  sendBit(0b1);
  sendStop();

  return true;
}

bool I2CDevice::readBytesStartingAt(uint8_t registerAddress, size_t numBytes, uint8_t* result) {
  sendStart();
  sendDeviceId();
  sendBit(0b0);

  if (!readAck()) {
    return false;
  }

  sendByte(registerAddress);

  if (!readAck()) {
    return false;
  }

  sendStart();
  sendDeviceId();
  sendBit(0b1);

  if (!readAck()) {
    return false;
  }

  for (uint8_t i=0; i<numBytes; i++) {
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
