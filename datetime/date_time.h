#ifndef DATETIME_H
#define DATETIME_H

#include <Arduino.h>
#include "time.h"
#include "date.h"

class DateTime {
  Time time;
  Date date;

public:
  DateTime(Time time, Date date) : time(time), date(date) {}

  Time getTime();
  Date getDate();
  String getFormattedDateTime();
};

#endif
