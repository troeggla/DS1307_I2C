#include "util.h"

void printRow(uint8_t data[], int offset, size_t rowLen) {
  char index[7];
  sprintf(index, "%04x  ", offset);
  Serial.print(index);

  for (int i=0; i<rowLen; i++) {
    char byteAtAddr[4];
    sprintf(byteAtAddr, "%02x ", data[offset + i]);
    Serial.print(byteAtAddr);
  }

  for (int i=0; i<8-rowLen; i++) {
    Serial.print("   ");
  }

  Serial.print(" |");

  for (int i=0; i<rowLen; i++) {
    if (data[offset + i] >= 32 && data[offset + i] <= 126) {
      Serial.print((char)data[offset + i]);
    } else {
      Serial.print(".");
    }
  }

  for (int i=0; i<8-rowLen; i++) {
    Serial.print(" ");
  }

  Serial.println("|");
}

void hexDump(uint8_t data[], size_t len) {
  int fullRowCount = len / 8;
  int lastRowLen = len % 8;

  for (int row=0; row<fullRowCount; row++) {
    int offset = row * 8;
    printRow(data, offset, 8);
  }

  if (lastRowLen > 0) {
    printRow(data, fullRowCount * 8, lastRowLen);
  }
}
