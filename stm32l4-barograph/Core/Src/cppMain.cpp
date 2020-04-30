#include <cstdio>
#include <cstring>
#include <tuple>
#include <cstdarg>
#include "i2c.h"
#include "main.h"
#include "adc.h"
#include "Adafruit_BMP085_Stm32.h"
#include "fonts.h"
#include "epd4in2.h"
#include "epdpaint.h"

Adafruit_BMP085 bmp{hi2c1};
volatile bool timeToGo = true;

Paint paint = Paint(new unsigned char[EPD_WIDTH * EPD_HEIGHT / 8], EPD_WIDTH, EPD_HEIGHT);

Epd epd;

#define WHITE 1
#define BLACK 0

/**
 * Draws a line of data
 *
 * @param x left position
 * @param y top position
 * @param font to be used
 * @param format 'printf()' style format
 * @param ... 'printf()' style arguments
 * @return [right, bottom] coordinates pair
 * @see printf()
 */
std::tuple<int, int> drawString(int x, int y, const sFONT &font, const char *format, ...) {
    char buff[100];
    va_list(args);
    va_start(args, format);
    vsnprintf(buff, sizeof(buff), format, args);
    va_end(args);

    unsigned int txtWidth = font.Width * strlen(buff);
    if (txtWidth > 400) txtWidth = 400;
    paint.DrawStringAt(x, y, buff, font, BLACK);
    return std::tuple<int, int>(x + txtWidth, y + font.Height);
}

constexpr static inline int right(const std::tuple<int, int> &coords) { return std::get<0>(coords); }

constexpr static inline int bottom(const std::tuple<int, int> &coords) { return std::get<1>(coords); }

void drawData(float pressure, float temperature) {

  if (epd.Init() != 0) {
    reportError("e-Paper init failed\r\n");
    return;
  }
  epd.ClearFrame(false);
  paint.Clear(WHITE);

    int y = bottom(drawString(0, 0, FontDoctorJekyllNF32, "%6.1fmmHg", pressure));
    int x = right(drawString(0, y, FontDoctorJekyllNF32, "%6.1f", temperature));
    x = right(drawString(x, y, FontDoctorJekyllNF20, "O"));
    y = 4 + bottom(drawString(x, y, FontDoctorJekyllNF32, "C"));

    paint.DrawLine(0, y - 2, 300, y - 2, BLACK);

/* This displays the data from the SRAM in e-Paper module */
  epd.SetPartialWindow(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());
  epd.DisplayFrame();
/* Deep sleep */
  epd.Sleep();

}

__attribute__((noreturn))
void cppMain() {
  if (!bmp.begin()) {
    reportError("BMP085 initialization error\r\n");
  }
  paint.SetRotate(ROTATE_270);
  HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
  while (HAL_ADC_GetState(&hadc1) == HAL_ADC_STATE_BUSY_INTERNAL);
  while (true) {
    while (!timeToGo) {
      __WFI();
    }
    timeToGo = false;
    HAL_ADC_Start(&hadc1);
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