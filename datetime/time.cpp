#include "time.h"

int Time::getHours() {
  return hours;
}

int Time::getMinutes() {
  return minutes;
}

int Time::getSeconds() {
  return seconds;
}

int Time::get12HourClockHour() {
  if (hours == 0) {
    return 12;
  } else if (hours > 12) {
    return hours - 12;
  }

  return hours;
}

String Time::getAMPM() {
  if (hours >= 12) {
    return "PM";
  }

  return "AM";
}
