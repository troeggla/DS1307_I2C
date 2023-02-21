#include "date_time.h"

Time DateTime::getTime() {
  return time;
}

Date DateTime::getDate() {
  return date;
}

String DateTime::getFormattedDateTime() {
  return date.getFormattedDate() + " " + time.getFormattedTime();
}
