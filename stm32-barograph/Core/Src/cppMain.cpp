//
// Created by Ilia.Motornyi on 22/04/2020.
//
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
const uint16_t *const VREFINT_CAL = (const uint16_t *) (uintptr_t) 0x1FFFF7BA;
volatile bool timeToGo = true;

unsigned char image[400 * 300 / 8];
Epd epd;

#define WHITE 1
#define BLACK 0

void drawString(const char *s, int x, int y) {
    /**
          Due to RAM not enough in Arduino UNO, a frame buffer is not allowed.
          In this case, a smaller image buffer is allocated and you have to
          update a partial display several times.
          1 byte = 8 pixels, therefore you have to set 8*N pixels at a time.
          Note that the picture on the display is 270 degree rotated
          and paint size and coorinates calculations are a bit tricky
      */
    unsigned int txtWidth = Font24.Width * strlen(s);
    if (txtWidth > 300) txtWidth = 300;

    Paint paint = Paint(image, (int) txtWidth, 24);
    paint.Clear(WHITE);
    paint.DrawStringAt(0, 0, s, &Font24, BLACK);
    epd.SetPartialWindow(paint.GetImage(), x, y, paint.GetWidth(), paint.GetHeight());
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

    HAL_Delay(15000);
}


__attribute__((noreturn)) void cppMain() {
    if (!bmp.begin()) {
        reportError("BMP085 initialization error\r\n");
    }
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
        drawData(pressure, temperature);
    }
}


void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *aHrtc) {
    UNUSED(aHrtc);
    timeToGo = true;
}