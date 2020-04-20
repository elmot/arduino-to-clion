#include <Adafruit_BMP085.h>
Adafruit_BMP085 bmp;

void setup() {
  // put your setup code here, to run once:
 Serial.begin(115200);

  if (!bmp.begin()) {
        Serial.println("Could not find a valid BMP085 sensor, check wiring!");
        while (1) {}
  }
}

void loop() {
  float pressure = 0.0075f * bmp.readPressure();

  String s = "P=";
  s += String(pressure, 1);
  s += "mmHg";

  Serial.println(s);

  delay(5000);
}
