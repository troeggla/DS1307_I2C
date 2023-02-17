#ifndef DS1307_H
#define DS1307_H

#include <Arduino.h>
#include "../i2c_device/i2c_device.h"

#define DEVICE_ID 0b1101000

typedef enum class OscillatorFreq {
  FREQ_1HZ,
  FREQ_4KHZ,
  FREQ_8KHZ,
  FREQ_32KHZ,
} OscillatorFreq;

class DS1307 {
  I2CDevice device;

  int convertFromBcd(uint8_t bcd);
  uint8_t convertToBcd(int value);

public:
  DS1307(uint8_t sdaPin, uint8_t sclPin) : device(DEVICE_ID, sdaPin, sclPin) {}

  bool enableClock(bool enable);
  bool enableOscillator(bool enable);
  bool enableOscillator(bool enable, OscillatorFreq freq);

  bool setDateTime(int year, int month, int date, int hour, int minute, int second);
  String getFormattedDateTime();
};

#endif
