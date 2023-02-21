#include "date.h"

int Date::getDate() {
  return date;
}

int Date::getMonth() {
  return month;
}

int Date::getYear() {
  return year;
}

String Date::getFormattedDate() {
  char formattedDate[11];

  sprintf(formattedDate, "%d-%02d-%02d", year, month, date);
  return String(formattedDate);
}
