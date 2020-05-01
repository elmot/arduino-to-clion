#include <cstdio>
#include <cstring>
#include "rtc.h"
#include "i2c.h"
#include "main.h"
#include "adc.h"
#include "Adafruit_BMP085_Stm32.h"
#include "fonts.h"
#include "epd4in2.h"
#include "epdpaint.h"

Adafruit_BMP085 bmp{hi2c1};
volatile bool timeToGo = true;

unsigned char image[EPD_WIDTH * EPD_HEIGHT / 8];
Epd epd;

#define WHITE 1
#define BLACK 0

void drawString(const char *s, int x, int y) {
  unsigned int txtWidth = Font24.Width * strlen(s);
  if (txtWidth > 300)
    txtWidth = 300;

  Paint paint = Paint(image, (int)txtWidth, 24);
  paint.Clear(WHITE);
  paint.DrawStringAt(0, 0, s, Font24, BLACK);
  epd.SetPartialWindow(paint.GetImage(), x, y, paint.GetWidth(),
                       paint.GetHeight());
}

void drawData(float pressure, float temperature) {
  char buff[100];
  snprintf(buff, sizeof(buff), "P=%.2f mmHg", pressure);

  if (epd.Init() != 0) {
    reportError("e-Paper init failed\r\n");
    return;
  }
  epd.ClearFrame(false);

  drawString(buff, 0, 0);
  snprintf(buff, sizeof(buff), "T=%.2f C", temperature);
  drawString(buff, 0, 28);

/* This displays the data from the SRAM in e-Paper module */
  epd.DisplayFrame();
/* Deep sleep */
  epd.Sleep();

}

__attribute__((noreturn))
void cppMain() {
  if (!bmp.begin()) {
    reportError("BMP085 initialization error\r\n");
  }
  HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
  while (HAL_ADC_GetState(&hadc1) == HAL_ADC_STATE_BUSY_INTERNAL);
  while (true) {
    while (!timeToGo) {
      __WFI();
    }
    timeToGo = false;
    HAL_ADC_Start(&hadc1);
    RTC_TimeTypeDef time;
    RTC_DateTypeDef date;
    HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
    HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);

    printf("Date: %04d-%02d-%02d %02d:%02d:%02d.%03ld\r\n", date.Year + 2000, date.Month, date.Date,
           time.Hours, time.Minutes, time.Seconds, 1000 * time.SubSeconds / (time.SecondFraction + 1));
    while (HAL_ADC_GetState(&hadc1) == HAL_ADC_STATE_BUSY_INTERNAL);
    uint16_t v = HAL_ADC_GetValue(&hadc1);
#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-signed-bitwise"
    int voltage = __HAL_ADC_CALC_VREFANALOG_VOLTAGE(v,LL_ADC_RESOLUTION_12B);
#pragma clang diagnostic pop
    float temperature = bmp.readTemperature();
    float pressure = 0.0075f * bmp.readPressure();
    printf("VCC: %d mV; P: %.1f mmHg; T: %.2f C; \n\r", voltage, pressure, temperature);
    drawData(pressure, temperature);
  }
}

void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *aHrtc) {
  UNUSED(aHrtc);
  timeToGo = true;
}