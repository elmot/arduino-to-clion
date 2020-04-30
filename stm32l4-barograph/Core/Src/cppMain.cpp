#include <cstdio>
#include "rtc.h"
#include "i2c.h"
#include "main.h"
#include "adc.h"
#include "Adafruit_BMP085_Stm32.h"

Adafruit_BMP085 bmp{hi2c1};
volatile bool timeToGo = true;

__attribute__((noreturn)) void cppMain() {
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
    int voltage = __LL_ADC_CALC_VREFANALOG_VOLTAGE(v,LL_ADC_RESOLUTION_12B);
    float temperature = bmp.readTemperature();
    float pressure = 0.0075f * bmp.readPressure();
    printf("VCC: %d mV; P: %.1f mmHg; T: %.2f C; \n\r", voltage, pressure, temperature);
  }
}

void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *aHrtc) {
  UNUSED(aHrtc);
  timeToGo = true;
}