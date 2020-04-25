//
// Created by Ilia.Motornyi on 22/04/2020.
//
#include <cstdio>
#include <cstring>
#include <tuple>
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

std::tuple<int, int> drawString(const char *s, int x, int y, const sFONT &font) {
    /**
          Due to RAM not enough in Arduino UNO, a frame buffer is not allowed.
          In this case, a smaller image buffer is allocated and you have to
          update a partial display several times.
          1 byte = 8 pixels, therefore you have to set 8*N pixels at a time.
          Note that the picture on the display is 270 degree rotated
          and paint size and coorinates calculations are a bit tricky
      */
    unsigned int txtWidth = font.Width * strlen(s);
    if (txtWidth > 400) txtWidth = 400;

    paint.DrawStringAt(x, y, s, font, BLACK);
    return std::tuple<int, int>(txtWidth, font.Height);
}

constexpr char weekdays[][7] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};

void drawData(float pressure, float temperature,
              RTC_TimeTypeDef &time, RTC_DateTypeDef &date) {
    char buff[100];
    snprintf(buff, sizeof(buff), "%5.1fmmHg", pressure);

    if (epd.Init() != 0) {
        reportError("e-Paper init failed\r\n");
        return;
    }
    epd.ClearFrame(false);
    paint.Clear(WHITE);
    int str2Top = std::get<1>(drawString(buff, 0, 0, FontDoctorJekyllNF32)) + 4;
    snprintf(buff, sizeof(buff), "%5.1f", temperature);
    auto[left, str2height] = drawString(buff, 0, str2Top, FontDoctorJekyllNF32);
    left += std::get<0>(drawString("O", left, str2Top, Font24));
    drawString("C", left, str2Top, FontDoctorJekyllNF32);
    snprintf(buff, sizeof(buff), "%02d:%02d", time.Hours, time.Minutes);
    auto str3Height = std::get<1>(drawString(buff, 0, str2Top + str2height, FontDoctorJekyllNF32));
    snprintf(buff, sizeof(buff), "%02d`-%02d-%04d, %s", date.Date, date.Month, 2000 + date.Year,
             weekdays[date.WeekDay - 1]);
    drawString(buff, 0, str2Top + str2height + str3Height, Font24);
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