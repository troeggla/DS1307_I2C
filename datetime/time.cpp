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

String Time::getFormattedTime() {
  char time[9];

  sprintf(time, "%02d:%02d:%02d", hours, minutes, seconds);
  return String(time);
}


String Time::getFormatted12HourClockTime() {
  char time[12];

  sprintf(time, "%02d:%02d:%02d %s", get12HourClockHour(), minutes, seconds, getAMPM());
  return String(time);
}
