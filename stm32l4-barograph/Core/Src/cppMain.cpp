//
// Created by Ilia.Motornyi on 30/04/2020.
//
#include "main.h"
#include <cstdio>
#include "i2c.h"
#include "Adafruit_BMP085_Stm32.h"

Adafruit_BMP085 bmp{hi2c1};

__attribute__((noreturn))
void cppMain() {
  if (!bmp.begin()) {
    reportError("BMP085 initialization error\r\n");
  }
  while (true) {
    float temperature = bmp.readTemperature();
    float pressure = 0.0075f * bmp.readPressure();
    printf("P: %.1f mmHg; T: %.2f C; \n\r", pressure, temperature);
  }
}