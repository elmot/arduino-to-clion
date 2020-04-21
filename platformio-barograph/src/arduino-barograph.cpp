#include <Arduino.h>
#include <Adafruit_BMP085.h>
#include "epd4in2.h"
#include "epdpaint.h"

Adafruit_BMP085 bmp;

#define BLACK     0
#define WHITE     1

unsigned char image[900];
Epd epd;
int count = 1;

void setup() {
  // put your setup code here, to run once:
 Serial.begin(115200);

  if (!bmp.begin()) {
        Serial.println("Could not find a valid BMP085 sensor, check wiring!");
        while (1) {}
  }
  /* This clears the SRAM of the e-paper display */
}

void loop() {
  float pressure = 0.0075f * bmp.readPressure();

  String s = "N=";
  s += count++;
  s += "; P=";
  s += String(pressure, 1);
  s += "mmHg";

  Serial.println(s);

  char buf[50];
  s.toCharArray(buf, 24);
  /**
      Due to RAM not enough in Arduino UNO, a frame buffer is not allowed.
      In this case, a smaller image buffer is allocated and you have to
      update a partial display several times.
      1 byte = 8 pixels, therefore you have to set 8*N pixels at a time.
      Note that the picture on the display is 270 degree rotated 
      and paint size and coorinates calculations are a bit tricky
  */
  int txtWidth = Font24.Width * s.length();
  if (txtWidth > 300) txtWidth = 300;

  Paint paint = Paint(image, txtWidth, 24);
  paint.Clear(WHITE);
  paint.DrawStringAt(0, 0,  buf, &Font24, BLACK);
  if (epd.Init() != 0) {
    Serial.print("e-Paper init failed");
    return;
  }
  epd.ClearFrame();
  epd.SetPartialWindow(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());

  /* This displays the data from the SRAM in e-Paper module */
  epd.DisplayFrame();
  /* Deep sleep */
  epd.Sleep();

  delay(15000);
}
