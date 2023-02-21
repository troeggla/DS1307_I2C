#ifndef DATE_H
#define DATE_H

#include <Arduino.h>

class Date {
  int date;
  int month;
  int year;

public:
  Date(int date, int month, int year) : date(date), month(month), year(year) {}

  int getDate();
  int getMonth();
  int getYear();

  String getFormattedDate();
};

#endif
