#ifndef I2C_DEVICE_H
#define I2C_DEVICE_H

#include <Arduino.h>

class I2CDevice {
  uint8_t deviceAddress;

  uint8_t sdaPin;
  uint8_t sclPin;

  void sendStart();
  void sendStop();

  void sendBit(uint8_t bit);
  void sendByte(uint8_t byte);
  void sendDeviceId();

  bool readAck();
  uint8_t readByte();

public:
  I2CDevice(uint8_t deviceAddress, uint8_t sdaPin, uint8_t sclPin);
  I2CDevice(uint8_t deviceAddress);

  bool writeByteAtAddress(uint8_t registerAddress, uint8_t byte);
  bool writeBytesStartingAt(uint8_t registerAddress, size_t numBytes, uint8_t* data);

  bool readByteAtAddress(uint8_t registerAddress, uint8_t *result);
  bool readBytesStartingAt(uint8_t registerAddress, size_t numBytes, uint8_t* data);
};

#endif
