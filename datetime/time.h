#ifndef TIME_H
#define TIME_H

#include <Arduino.h>

class Time {
  int hours;
  int minutes;
  int seconds;

public:
  Time(int hours, int minutes, int seconds) : hours(hours), minutes(minutes), seconds(seconds) {}

  int getHours();
  int getMinutes();
  int getSeconds();

  int get12HourClockHour();
  String getAMPM();

  String getFormattedTime();
  String getFormatted12HourClockTime();
};

#endif
