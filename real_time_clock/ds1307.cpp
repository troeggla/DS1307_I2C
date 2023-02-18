#include "ds1307.h"

int DS1307::convertFromBcd(uint8_t bcd) {
  int tens = ((bcd & 0b11110000) >> 4) * 10;
  int ones = bcd & 0b00001111;

  return tens + ones;
}

uint8_t DS1307::convertToBcd(int value) {
  uint8_t ones = value % 10;
  uint8_t tens = value / 10;

  return (tens << 4) + ones;
}

bool DS1307::enableClock(bool enable) {
  uint8_t currentValue = 0;
  bool success = device.readByteAtAddress(0x00, &currentValue);

  if (!success) {
    return false;
  }

  uint8_t enableClockByte = (enable == true) ? (
    currentValue & 0b01111111
  ) : (
    currentValue | 0b10000000
  );

  return device.writeByteAtAddress(0x00, enableClockByte);
}

bool DS1307::enableOscillator(bool enable, OscillatorFreq freq) {
  uint8_t enableOsc = 0x0;

  if (enable == true) {
    enableOsc = 0b00010000;

    switch (freq) {
      case OscillatorFreq::FREQ_4KHZ:
        enableOsc = enableOsc | 0b1;
        break;
      case OscillatorFreq::FREQ_8KHZ:
        enableOsc = enableOsc | 0b10;
        break;
      case OscillatorFreq::FREQ_32KHZ:
        enableOsc = enableOsc | 0b11;
        break;
      default:
        break;
    }
  }

  return device.writeByteAtAddress(0x07, enableOsc);
}

bool DS1307::enableOscillator(bool enable) {
  return enableOscillator(enable, OscillatorFreq::FREQ_1HZ);
}

bool DS1307::setDateTime(int year, int month, int date, int hours, int minutes, int seconds) {
  uint8_t currentSecondValue = 0;
  bool success = device.readByteAtAddress(0x00, &currentSecondValue);

  if (!success) {
    return false;
  }

  uint8_t newSecondValue = (currentSecondValue & 0b10000000) | convertToBcd(seconds);

  uint8_t data[7] = {
    newSecondValue, convertToBcd(minutes), convertToBcd(hours),
    0,
    convertToBcd(date), convertToBcd(month), convertToBcd(year % 100)
  };

  return device.writeBytesStartingAt(0x00, 7, data);
}

String DS1307::getFormattedDateTime() {
  uint8_t result[8];
  bool success = device.readBytesStartingAt(0x00, 8, result);

  if (success) {
    int seconds = convertFromBcd(result[0]);
    int minutes = convertFromBcd(result[1]);
    int hours = convertFromBcd(result[2]);
    int date = convertFromBcd(result[4]);
    int month = convertFromBcd(result[5]);
    int year = convertFromBcd(result[6]);

    char datetime[20];
    sprintf(datetime, "20%02d-%02d-%02d %02d:%02d:%02d", year, month, date, hours, minutes, seconds);

    return String(datetime);
  }

  return "";
}
