//
// Created by Ilia.Motornyi on 25/04/2020.
//
#include <array>
#include <string>
#include <tuple>
#include <epdpaint.h>
#include <epd4in2.h>
#include "main.h"
#include "FontDoctorJekyllNF32.h"
#include "barograph.hpp"

extern RTC_HandleTypeDef hrtc;
extern Paint paint;
using namespace std;
const sFONT &font = FontDoctorJekyllNF32;
constexpr int ALT_BKP_REGISTER = 15;

typedef const struct {
    int min;
    int max;
    int digits;
    int screenLength = digits;
    const string name;
    int &value;
    int screenTop;
    int screenLeft;
    bool &fieldChanged;
} FieldMetaData;

int year;
int day;
int month;
int hour;
int minute;
int alt;
bool dateChanged;
bool timeChanged;
bool altChanged;
static const int MIN_YEAR = 20;
static const int dataLine1Top = 102;
static const int dataLine2Top = 182;
static const int dataLine3Top = 294;

extern Epd epd;
static const array<FieldMetaData, 6>
    fields{{
               {.min = 1, .max = 31, .digits =2, .name="Day", .value = day,
                   .screenTop = dataLine1Top, .screenLeft = 1, .fieldChanged = dateChanged},

               {.min = 1, .max = 11, .digits = 2, .screenLength = 3, .name="Month", .value = month,
                   .screenTop = dataLine1Top, .screenLeft = 3 * font.Width, .fieldChanged = dateChanged},

               {.min = MIN_YEAR, .max = 99, .digits =2, .name="Year", .value = year,
                   .screenTop = dataLine1Top, .screenLeft = 7 * font.Width, .fieldChanged = dateChanged},

               {.min = 0, .max = 24, .digits =2, .name="Hour", .value = hour,
                   .screenTop = dataLine2Top, .screenLeft = 1, .fieldChanged = timeChanged},

               {.min = 0, .max = 59, .digits =2, .name="Minute", .value = minute,
                   .screenTop =dataLine2Top, .screenLeft = 3 * font.Width, .fieldChanged = timeChanged},

               {.min = 0, .max = 9999, .digits = 4, .name="Altitude", .value = alt,
                   .screenTop = dataLine3Top, .screenLeft =1, .fieldChanged = altChanged}}};

struct Button {
    enum State {
        NOT_RELEASED, RELEASED, PRESSED, LONG_PRESSED, LONG_RELEASED
    };
private:
    volatile State state = NOT_RELEASED;
    volatile uint32_t lastPressed = 0;
    volatile uint32_t lastRelease = 0;
    volatile int clickCount = 0;

    [[nodiscard]] static bool isPressed() { return (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_RESET); }

public:
    void init() noexcept {
        changeContext();
    }

    void _pressed() {
        uint32_t tick = HAL_GetTick();
        if ((tick - lastRelease) <= 2) return;//debouncing
        lastPressed = tick;
        state = PRESSED;
    }

    void _released() {
        uint32_t tick = HAL_GetTick();
        if ((tick - lastPressed) <= 2) return;//debouncing
        if (state == PRESSED) {
            clickCount++;
        }
        lastRelease = tick;
        state = RELEASED;
    }

    [[nodiscard]] int getClickCount(int max) const {
        return clickCount > max ? max : clickCount;
    }

    State getState() {
        uint32_t time = HAL_GetTick();
        __disable_irq();
        switch (state) {
            case PRESSED:
                if ((time - lastPressed) >= 4000)
                    state = LONG_PRESSED;
            case RELEASED:
                if ((time - lastRelease) >= 4000)
                    state = LONG_RELEASED;
            case LONG_PRESSED:
            case LONG_RELEASED:
            case NOT_RELEASED:
                break;

        }
        __enable_irq();
        return state;
    }

    void changeContext() {
        state = isPressed() ? NOT_RELEASED : RELEASED;
        clickCount = 0;
    }
} Button;

void HAL_GPIO_EXTI_Callback([[maybe_unused]] uint16_t GPIO_Pin) {
    if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_RESET) {
        Button._pressed();
    } else {
        Button._released();
    }
}

void changeField(FieldMetaData &field) {
    constexpr array<int, 5> dividers{1, 10, 100, 1000, 10000};
    constexpr int top = 150;

    int newValue = 0;
    for (int currentDigitIdx = 0; currentDigitIdx < field.digits; ++currentDigitIdx) {
        paint.Clear(WHITE);
        int y = bottom(drawString((300 - 6 * FontDoctorJekyllNF32.Width) / 2, 0, FontDoctorJekyllNF32, "Change"));
        int nameX = (int) (300 - FontDoctorJekyllNF32.Width * field.name.length()) / 2;
        y = bottom(drawString(nameX, y + 8, FontDoctorJekyllNF32, field.name.c_str()));
        drawString(0, y + 8, Font20, "Tap btn 0...9 times");
        for (int i = 0; i < field.digits; i++) {
            int divider = dividers[field.digits - 1 - i];
            unsigned char c = (newValue / divider) % 10 + '0';
            if (c > '9') c = '9';
            int x = i * FontDoctorJekyllNF32.Width;
            paint.DrawCharAt(x, top, c, FontDoctorJekyllNF32, BLACK);
            if (i == currentDigitIdx) {
                paint.DrawRectangle(x + 1, top, x + FontDoctorJekyllNF32.Width, top + FontDoctorJekyllNF32.Height,
                                    BLACK);
            }
        }
        Button.changeContext();
        epd.DisplayFrame(paint.GetImage(), true);
        while (Button.getState() != Button::LONG_RELEASED) {
            __WFI();
        }
        int multiplier = dividers[field.digits - 1 - currentDigitIdx];
        int clickCount = Button.getClickCount(9);
        newValue = (newValue % multiplier) + clickCount * multiplier +
                   (newValue / multiplier / 10) * multiplier * 10;
    }
    if (newValue > field.max) newValue = field.max;
    else if (newValue < field.min) newValue = field.min;
    field.value = newValue;
    field.fieldChanged = true;
}

void runCorrection() {
    Button.init();
    if(Button.getState() == Button::RELEASED) {
        return;
    }
    RTC_DateTypeDef date;
    RTC_TimeTypeDef time;
    HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
    HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);

    year = (date.Year < MIN_YEAR) ? year = MIN_YEAR : date.Year;
    month = date.Month;
    day = date.Date;

    hour = time.Hours;
    minute = time.Minutes;
    alt = getAltitude();

    timeChanged = dateChanged = altChanged = false;
    for (FieldMetaData *field = fields.begin(); field != fields.end(); ++field) {
        paint.Clear(WHITE);
        int y = bottom(drawString((300 - 6 * FontDoctorJekyllNF32.Width) / 2, 0, FontDoctorJekyllNF32, "Config"));
        y = bottom(drawString(0, y + 14, Font20, "Tap button to change"));
        drawString(0, y + 4, Font20, "or wait to skip field");
        drawString(0, dataLine1Top, font, "%02d-%3s-%02d", day, months[month - 1], year);
        drawString(0, dataLine2Top - font.Height - 4, font, "Time:");
        drawString(0, dataLine2Top, font, "%02d:%02d", hour, minute);
        drawString(0, dataLine3Top - font.Height - 4, font, "Altitude");
        drawString(0, dataLine3Top, font, "%04d", alt);
        paint.DrawRectangle(field->screenLeft, field->screenTop,
                            field->screenLeft + field->screenLength * font.Width,
                            field->screenTop + font.Height, BLACK
        );
        Button.changeContext();
        epd.DisplayFrame(paint.GetImage(), true);
        while (true) {
            Button::State state = Button.getState();
            if (state == Button::LONG_RELEASED) {
                break;
            }
            if ((state == Button::PRESSED) ||
                (state == Button::NOT_RELEASED) ||
                (Button.getClickCount(1) > 0) > 0) {
                changeField(*field);
                --field;
                break;
            }
        }
    }

    if (dateChanged) {
        date.Date = day;
        date.Month = month;
        date.Year = year;
        date.WeekDay = 1;
        HAL_PWR_EnableBkUpAccess();
        HAL_RTC_SetDate(&hrtc, &date, RTC_FORMAT_BIN);
        HAL_PWR_DisableBkUpAccess();
    }
    if (timeChanged) {
        time.Hours = hour;
        time.Minutes = minute;
        time.Seconds = 0;
        time.SubSeconds = 0;
        time.SecondFraction = 0;
        time.StoreOperation = RTC_STOREOPERATION_SET;
        time.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
        HAL_PWR_EnableBkUpAccess();
        HAL_RTC_SetTime(&hrtc, &time, RTC_FORMAT_BIN);
        HAL_PWR_DisableBkUpAccess();
    }
    if (altChanged) {
        HAL_PWR_EnableBkUpAccess();
        HAL_RTCEx_BKUPWrite(&hrtc, ALT_BKP_REGISTER, alt);
        HAL_PWR_DisableBkUpAccess();
    }
}

int getAltitude() {
    return HAL_RTCEx_BKUPRead(&hrtc, ALT_BKP_REGISTER);
}