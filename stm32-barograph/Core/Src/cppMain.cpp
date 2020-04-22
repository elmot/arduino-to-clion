//
// Created by Ilia.Motornyi on 22/04/2020.
//
#include <rtc.h>
#include "main.h"
//#include <iostream>
#include <cstdio>
#include <adc.h>

__attribute__((noreturn)) void cppMain() {
    HAL_SuspendTick();
    HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
    while(HAL_ADC_GetState(&hadc1) == HAL_ADC_STATE_BUSY_INTERNAL);
    HAL_ADC_Start(&hadc1);
    while (true) {
        __WFI();
    }
}

const uint16_t VREFINT_CAL = *(uint16_t*)((uintptr_t)0x1FFFF7BA);
void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc) {
    RTC_TimeTypeDef time;
    RTC_DateTypeDef date;
    HAL_RTC_GetDate(hrtc, &date, RTC_FORMAT_BIN);
    HAL_RTC_GetTime(hrtc, &time, RTC_FORMAT_BIN);

    printf("Date: %04d-%02d-%02d %02d:%02d:%02d.%03ld\r\n", date.Year + 2000, date.Month, date.Date,
           time.Hours, time.Minutes, time.Seconds, 1000 * time.SubSeconds / (time.SecondFraction + 1));
    uint32_t state = HAL_ADC_GetState(&hadc1);
    uint16_t v = HAL_ADC_GetValue(&hadc1);
    int voltage = 3300L * VREFINT_CAL / v;
    printf("ADC State: %x; value: %d; voltage: %d mV", state, v, voltage);
}