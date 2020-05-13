#include <cstdio>
#include <cstring>
#include <tuple>
#include <cstdarg>
#include <algorithm>
#include "rtc.h"
#include "i2c.h"
#include "main.h"
#include "adc.h"
#include "Adafruit_BMP085_Stm32.h"
#include "fonts.h"
#include "epd4in2.h"
#include "epdpaint.h"

using namespace std;

Adafruit_BMP085 bmp{hi2c1};
volatile bool timeToGo = true;

Paint paint = Paint(new unsigned char[EPD_WIDTH * EPD_HEIGHT / 8], EPD_WIDTH, EPD_HEIGHT);

Epd epd;

#define WHITE 1
#define BLACK 0
const int LOW_BATT_MVOLTS = 3000;
const int EMPTY_BATT_MVOLTS = 2900;
const int chartPoints = 48;
const int LAST_TIMESTAMP_REGISTER = 30;

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

void drawData(float pressure, float temperature, int mVolts) {

    if (epd.Init() != 0) {
        reportError("e-Paper init failed\r\n");
        return;
    }
    epd.ClearFrame(false);
    paint.Clear(WHITE);

    int y = bottom(drawString(0, 0, FontDoctorJekyllNF32, "%6.1fmmHg", pressure));
    int x = right(drawString(0, y, FontDoctorJekyllNF32, "%6.1f", temperature));
    x = right(drawString(x, y, FontDoctorJekyllNF20, "O"));

    if (mVolts <= EMPTY_BATT_MVOLTS) {
        paint.DrawCharAt((300 - FontPictogramNF32.Width) / 2, 200, BATT_EMPTY,
                         FontPictogramNF32, BLACK);
    } else if (mVolts <= LOW_BATT_MVOLTS) {
        paint.DrawCharAt(300 - FontPictogramNF32.Width, y, BATT_LOW,
                         FontPictogramNF32, BLACK);
    }

    drawString(x, y, FontDoctorJekyllNF32, "C");

}

void plotChart(array<uint16_t, chartPoints + 1> &chartData) {

    auto dataStart = chartData.end();
    while (dataStart != chartData.begin() && *(dataStart - 1) != 0) {
        dataStart--;
    }

    int max = *max_element(dataStart, chartData.end());
    int min = *min_element(dataStart, chartData.end());

    min = 100 * (( min - 30) / 100);
    max = 100 * (( max + 130) / 100);
    const int top = 80;
    const int left = FontDoctorJekyllNF20.Width * 3;
    paint.DrawHorizontalLine(left, top, 300, BLACK);
    paint.DrawVerticalLine(left, top, 400 - top, BLACK);
    drawString(0, top, FontDoctorJekyllNF20, "%3d", max / 10);
    drawString(0, 400 - FontDoctorJekyllNF20.Height, FontDoctorJekyllNF20, "%3d", min / 10);
    for (int p = min + 100; p <= max; p += 100) {
        int y = top + (max - p) * (399 - top) / (max - min);
        for (int x = left; x < 300; x += 3) {
            paint.DrawPixel(x, y, BLACK);
        }
    }
    const auto dx = (300.0f - (float) left) / chartPoints;
    auto x1 = (float) left;
    uint16_t v1 = *dataStart;
    for (auto point = dataStart + 1; point < chartData.end(); point++) {
        uint16_t v2 = *point;
        uint16_t x2 = x1 + dx;
        if (v1 > 0 && v2 > 0) {
            int y1 = top + (max - v1) * (399 - top) / (max - min);
            int y2 = top + (max - v2) * (399 - top) / (max - min);
            paint.DrawLine((int) x1, y1, (int) x2, y2, BLACK);
        }
        x1 = x2;
        v1 = v2;
    }
  int point1 = *(chartData.end() - 3);
  int point2 = *(chartData.end() - 2);
  if ((point1 != 0) && (point2 != 0)) {
    char pictogram = 0;
    int delta = point2 - point1;
    if (delta >= 10) {
      pictogram = PRESSURE_UP;
    } else if (delta >= 3) {
      pictogram = PRESSURE_LITE_UP;
    } else if (delta <= -10) {
      pictogram = PRESSURE_DOWN;
    } else if (delta <= -3) {
      pictogram = PRESSURE_LITE_DOWN;
    }
    if (pictogram != 0) {
      paint.DrawCharAt((300 - FontPictogramNF32.Width) / 2,
                       400 - FontPictogramNF32.Height,
                       pictogram, FontPictogramNF32, BLACK);
    }
  }
}

void readUpdateHistory(array<uint16_t, chartPoints + 1> &chartData, RTC_TimeTypeDef &time, RTC_DateTypeDef &date,
                       float pressure) {

//    int hourNumber = time.Hours; todo uncomment
    int hourNumber = time.Seconds /20;
    int deltaHours = hourNumber - (int)HAL_RTCEx_BKUPRead(&hrtc, LAST_TIMESTAMP_REGISTER);
    for (uint32_t i = 0; i < chartPoints / 2; i++) {
        uint32_t r = HAL_RTCEx_BKUPRead(&hrtc, i);
        chartData[i * 2] = r & 0xFFFFu;
        chartData[i * 2 + 1] = r >> 16u;
    }
    chartData.back() = (uint16_t) (pressure * 10);
    if (deltaHours != 0) {
        for (int i = 0; i < chartPoints; i += 2) {
            uint32_t r = chartData[i + 1] | (uint32_t) (chartData[i + 2] << 16u);
            HAL_RTCEx_BKUPWrite(&hrtc, i / 2, r);
        }
        HAL_RTCEx_BKUPWrite(&hrtc, LAST_TIMESTAMP_REGISTER, hourNumber);
    }
}

__attribute__((noreturn))
void cppMain() {
    if (!bmp.begin()) {
        reportError("BMP085 initialization error\r\n");
    }
    paint.SetRotate(ROTATE_270);
    HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
    while (true) {
        while (!timeToGo) {
            __WFI();
        }
        timeToGo = false;
        HAL_ADC_Start(&hadc1);
        RTC_TimeTypeDef time;
        RTC_DateTypeDef date;
        __HAL_RTC_WRITEPROTECTION_DISABLE(&hrtc);
        HAL_RTC_WaitForSynchro(&hrtc);
        __HAL_RTC_WRITEPROTECTION_ENABLE(&hrtc);
        HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);//order of calls is important!
        HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);//See MCU reference manual

        HAL_ADC_PollForConversion(&hadc1,100);
        uint16_t v = HAL_ADC_GetValue(&hadc1);
#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-signed-bitwise"
        int voltage = __HAL_ADC_CALC_VREFANALOG_VOLTAGE(v, LL_ADC_RESOLUTION_12B);
#pragma clang diagnostic pop
        float temperature = bmp.readTemperature();
        float pressure = 0.0075f * bmp.readPressure();

        printf("VCC: %d mV; P: %.1f mmHg; T: %.2f C; \n\r", voltage, pressure, temperature);
        drawData(pressure, temperature, voltage);

        static array<uint16_t, chartPoints + 1> chartData{};
        readUpdateHistory(chartData, time, date, pressure);

        plotChart(chartData);
        /* This displays the data from the SRAM in e-Paper module */
        epd.DisplayFrame(paint.GetImage());
        /* Deep sleep */
        epd.Sleep();
    }
}

void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *aHrtc) {
    UNUSED(aHrtc);
    timeToGo = true;
}