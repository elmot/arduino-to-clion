//
// Created by Ilia.Motornyi on 22/04/2020.
//
#include <cstdio>
#include <cstring>
#include <tuple>
#include <cstdarg>
#include <algorithm>
#include <FontDoctorJekyllNF24.h>
#include "rtc.h"
#include "i2c.h"
#include "main.h"
#include "adc.h"
#include "Adafruit_BMP085_Stm32.h"
#include "fonts.h"
#include "FontDoctorJekyllNF32.h"
#include "epd4in2.h"
#include "epdpaint.h"
#include "barograph.hpp"

using namespace std;

Adafruit_BMP085 bmp{hi2c1};
const uint16_t *const VREFINT_CAL = (const uint16_t *) (uintptr_t) 0x1FFFF7BA;

Paint paint = Paint(new unsigned char[400 * 300 / 8], 400, 300);

Epd epd;

tuple<int, int> drawString(int x, int y, const sFONT &font, const char *format, ...) {
    char buff[100];
    va_list(args);
    va_start(args, format);
    vsnprintf(buff, sizeof(buff), format, args);
    va_end(args);

    unsigned int txtWidth = font.Width * strlen(buff);
    if (txtWidth > 400) txtWidth = 400;
    paint.DrawStringAt(x, y, buff, font, BLACK);
    return tuple<int, int>(x + txtWidth, y + font.Height);
}

constexpr char months[][12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
extern const sFONT FontPictogramNF32;
const char BATT_LOW = ' ';
const char BATT_EMPTY = '!';
const char PRESSURE_UP = '"';
const char PRESSURE_LITE_UP = '#';
[[maybe_unused]] const char PRESSURE_STILL = '$';
const char PRESSURE_LITE_DOWN = '%';
const char PRESSURE_DOWN = '&';
const int LOW_BATT_MVOLTS = 2600;
const int EMPTY_BATT_MVOLTS = 2300;

bool displayInit() {
    if (epd.Init() != 0) {
        reportError("e-Paper init failed\r\n");
        return false;
    } else {
        epd.ClearFrame(false);
        paint.Clear(WHITE);
    }
    return true;
}

void drawData(float pressure, float temperature, RTC_TimeTypeDef &time, RTC_DateTypeDef &date, uint16_t mVolts) {
    if (!displayInit()) {
        return;
    }

    int y = 4 +
            bottom(drawString(0, 0, FontDoctorJekyllNF24, "%02d-%s-%04d %02d:%02d", date.Date, months[date.Month - 1],
                              2000 + date.Year, time.Hours, time.Minutes));

    paint.DrawLine(0, y - 2, 300, y - 2, BLACK);

    y = 4 + bottom(drawString(0, y, FontDoctorJekyllNF32, "%6.1fmmHg", pressure));
    int x = right(drawString(0, y, FontDoctorJekyllNF32, "%6.1f", temperature));
    x = right(drawString(x, y, Font20, "O"));

    if (mVolts <= EMPTY_BATT_MVOLTS) {
        paint.DrawCharAt((300 - FontPictogramNF32.Width) / 2,
                         (400 - FontPictogramNF32.Height) / 2, BATT_EMPTY, FontPictogramNF32, BLACK);
    } else if (mVolts <= LOW_BATT_MVOLTS) {
        paint.DrawCharAt(300 - FontPictogramNF32.Width, y, BATT_LOW, FontPictogramNF32, BLACK);
    }
    y = 4 + bottom(drawString(x, y, FontDoctorJekyllNF32, "C"));
    paint.DrawLine(0, y - 2, 300, y - 2, BLACK);
}

void runCorrection();

void plotChart(array<uint16_t, chartPoints + 1> &chartData) {

    auto dataStart = chartData.end();
    while (dataStart != chartData.begin() && *(dataStart - 1) != 0) {
        dataStart--;
    }

    int max = *max_element(dataStart, chartData.end());
    int min = *min_element(dataStart, chartData.end());

    min = 100 * ((min - 30) / 100);
    max = 100 * ((max + 130) / 100);
    const int top = 100;
    const int left = Font20.Width * 3;
    paint.DrawVerticalLine(left, top, 400 - top, BLACK);
    drawString(0, top, Font20, "%3d", max / 10);
    drawString(0, 400 - Font20.Height + 5, Font20, "%3d", min / 10);
    for (int p = min; p <= max; p += 100) {
        int y = top + (max - p) * (400 - top) / (max - min);
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
            int y1 = top + (max - v1) * (400 - top) / (max - min);
            int y2 = top + (max - v2) * (400 - top) / (max - min);
            paint.DrawLine((int) x1, y1, (int) x2, y2, BLACK);
        }
        x1 = x2;
        v1 = v2;
    }
    int nextToLastValue = *(chartData.end() - 1);
    if (nextToLastValue != 0) {
        char pictogram = 0;
        int delta = (int) chartData.back() - nextToLastValue;
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

    int hourNumber = time.Hours;
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
    while (HAL_ADC_GetState(&hadc1) == HAL_ADC_STATE_BUSY_INTERNAL);
    HAL_ADC_Start(&hadc1);
    if (!displayInit()) {
        reportError("Display Init error");
    }
    runCorrection();
    // After sleep mode RTC has to be synchronized
    HAL_Delay(2);
    RTC_TimeTypeDef time;
    RTC_DateTypeDef date;
    HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
    HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);

    uint16_t v = HAL_ADC_GetValue(&hadc1);
    int voltage = 3300L * *VREFINT_CAL / v;
    float temperature = bmp.readTemperature();
    float pressure = 0.0075f * bmp.readSealevelPressure((float) getAltitude());

    drawData(pressure, temperature, time, date, voltage);
    array<uint16_t, chartPoints + 1> chartData{};
    readUpdateHistory(chartData, time, date, pressure);

    plotChart(chartData);
    epd.DisplayFrame(paint.GetImage());
    //Enter standby mode
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
    HAL_PWR_EnterSTANDBYMode();
    while (true);
}
