//
// Created by Ilia.Motornyi on 22/04/2020.
//
#include <cstdio>
#include <cstring>
#include <tuple>
#include <cstdarg>
#include "rtc.h"
#include "i2c.h"
#include "main.h"
#include "adc.h"
#include "Adafruit_BMP085_Stm32.h"
#include "fonts.h"
#include "FontDoctorJekyllNF32.h"
#include "epd4in2.h"
#include "epdpaint.h"

Adafruit_BMP085 bmp{hi2c1};
const uint16_t *const VREFINT_CAL = (const uint16_t *) (uintptr_t) 0x1FFFF7BA;
volatile bool timeToGo = true;

Paint paint = Paint(new unsigned char[400 * 300 / 8], 400, 300);

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

constexpr char months[][12] = {"Jan", "Feb", "Mar", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

void drawData(float pressure, float temperature, RTC_TimeTypeDef &time, RTC_DateTypeDef &date) {

    if (epd.Init() != 0) {
        reportError("e-Paper init failed\r\n");
        return;
    }
    epd.ClearFrame(false);
    paint.Clear(WHITE);
    int y = 4 + bottom(drawString(0, 0, Font24, "%02d-%s-%04d %02d:%02d", date.Date, months[date.Month - 1],
                                  2000 + date.Year, time.Hours, time.Minutes));

    paint.DrawLine(0, y - 2, 300, y - 2, BLACK);

    y = 4 + bottom(drawString(0, y, FontDoctorJekyllNF32, "%6.1fmmHg", pressure));
    int x = right(drawString(0, y, FontDoctorJekyllNF32, "%6.1f", temperature));
    x = right(drawString(x, y, Font20, "O"));
    y = 4 + bottom(drawString(x, y, FontDoctorJekyllNF32, "C"));

    paint.DrawLine(0, y - 2, 300, y - 2, BLACK);

    /* This displays the data from the SRAM in e-Paper module */
    epd.SetPartialWindow(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());
    epd.DisplayFrame();
/* Deep sleep */
    epd.Sleep();

    HAL_Delay(15000);
}


__attribute__((noreturn)) void cppMain() {
    if (!bmp.begin()) {
        reportError("BMP085 initialization error\r\n");
    }
    paint.SetRotate(ROTATE_270);
    HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
    while (HAL_ADC_GetState(&hadc1) == HAL_ADC_STATE_BUSY_INTERNAL);
    HAL_ADC_Start(&hadc1);
    while (true) {
        while (!timeToGo) {
            __WFI();
        }
        timeToGo = false;
        RTC_TimeTypeDef time;
        RTC_DateTypeDef date;
        HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
        HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);

        printf("Date: %04d-%02d-%02d %02d:%02d:%02d.%03ld\r\n", date.Year + 2000, date.Month, date.Date,
               time.Hours, time.Minutes, time.Seconds, 1000 * time.SubSeconds / (time.SecondFraction + 1));
        uint16_t v = HAL_ADC_GetValue(&hadc1);
        int voltage = 3300L * *VREFINT_CAL / v;
        float temperature = bmp.readTemperature();
        float pressure = 0.0075f * bmp.readPressure();
        printf("VCC: %d mV; P: %.1f mmHg; T: %.2f C; \n\r", voltage, pressure, temperature);
        drawData(pressure, temperature, time, date);
    }
}


void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *aHrtc) {
    UNUSED(aHrtc);
    timeToGo = true;
}