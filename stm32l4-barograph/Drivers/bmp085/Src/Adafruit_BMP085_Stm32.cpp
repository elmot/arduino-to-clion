/*************************************************** 
  This is a library for the Adafruit BMP085/BMP180 Barometric Pressure + Temp sensor

  Designed specifically to work with the Adafruit BMP085 or BMP180 Breakout 
  ----> http://www.adafruit.com/products/391
  ----> http://www.adafruit.com/products/1603

  These displays use I2C to communicate, 2 pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution

  Modified to be compatible with STM32 HAL by
  Ilia "Elmot" Motornyi for JetBrains
 ****************************************************/

#include <cmath>
#include "Adafruit_BMP085_Stm32.h"

static void delay(int mSec) {
    HAL_Delay(mSec);
}

bool Adafruit_BMP085::begin(uint8_t mode) {
    if (mode > BMP085_ULTRAHIGHRES)
        mode = BMP085_ULTRAHIGHRES;
    oversampling = mode;

    if (read8(0xD0) != 0x55) return false;

    /* read calibration data */
    ac1 = read16(BMP085_CAL_AC1);
    ac2 = read16(BMP085_CAL_AC2);
    ac3 = read16(BMP085_CAL_AC3);
    ac4 = read16(BMP085_CAL_AC4);
    ac5 = read16(BMP085_CAL_AC5);
    ac6 = read16(BMP085_CAL_AC6);

    b1 = read16(BMP085_CAL_B1);
    b2 = read16(BMP085_CAL_B2);

    mb = read16(BMP085_CAL_MB);
    mc = read16(BMP085_CAL_MC);
    md = read16(BMP085_CAL_MD);

    return true;
}

int32_t Adafruit_BMP085::computeB5(int32_t UT) const {
    int32_t X1 = (UT - (int32_t) ac6) * ((int32_t) ac5) /32768;
    int32_t X2 = ((int32_t) mc << 11) / (X1 + (int32_t) md);
    return X1 + X2;
}

uint16_t Adafruit_BMP085::readRawTemperature() {
    write8(BMP085_CONTROL, BMP085_READTEMPCMD);
    delay(5);
    return read16(BMP085_TEMPDATA);
}

uint32_t Adafruit_BMP085::readRawPressure() {
    uint32_t raw;

    write8(BMP085_CONTROL, BMP085_READPRESSURECMD + (oversampling << 6u));

    if (oversampling == BMP085_ULTRALOWPOWER)
        delay(5);
    else if (oversampling == BMP085_STANDARD)
        delay(8);
    else if (oversampling == BMP085_HIGHRES)
        delay(14);
    else
        delay(26);

    raw = read16(BMP085_PRESSUREDATA);

    raw <<= 8u;
    raw |= read8(BMP085_PRESSUREDATA + 2);
    raw >>= (8u - oversampling);

    return raw;
}

int32_t Adafruit_BMP085::readPressure() {
    int32_t UT, UP, B3, B5, B6, X1, X2, X3, p;
    uint32_t B4, B7;

    UT = readRawTemperature();
    UP = readRawPressure();

    B5 = computeB5(UT);

    // do pressure calcs
    B6 = B5 - 4000;
    X1 = ((int32_t) b2 * ((B6 * B6) >> 12)) >> 11;
    X2 = ((int32_t) ac2 * B6) >> 11;
    X3 = X1 + X2;
    B3 = ((((int32_t) ac1 * 4 + X3) << oversampling) + 2) / 4;

    X1 = ((int32_t) ac3 * B6) >> 13;
    X2 = ((int32_t) b1 * ((B6 * B6) >> 12)) >> 16;
    X3 = ((X1 + X2) + 2) >> 2;
    B4 = ((uint32_t) ac4 * (uint32_t) (X3 + 32768)) >> 15;
    B7 = ((uint32_t) UP - B3) * (uint32_t) (50000UL >> oversampling);

    if (B7 < 0x80000000) {
        p = (B7 * 2) / B4;
    } else {
        p = (B7 / B4) * 2;
    }
    X1 = (p >> 8) * (p >> 8);
    X1 = (X1 * 3038) >> 16;
    X2 = (-7357 * p) >> 16;

    p = p + ((X1 + X2 + (int32_t) 3791) >> 4);
    return p;
}

[[maybe_unused]] int32_t Adafruit_BMP085::readSealevelPressure(float altitude_meters) {
    float pressure = readPressure();
    return (int32_t) (pressure / pow(1.0 - altitude_meters / 44330, 5.255));
}

float Adafruit_BMP085::readTemperature() {
    int32_t UT, B5;     // following ds convention
    float temp;

    UT = readRawTemperature();

    B5 = computeB5(UT);
    temp = (B5 + 8) >> 4;
    temp /= 10;

    return temp;
}

[[maybe_unused]] float Adafruit_BMP085::readAltitude(float sealevelPressure) {
    float altitude;

    float pressure = readPressure();

    altitude = 44330.0 * (1.0 - pow(pressure / sealevelPressure, 0.1903));

    return altitude;
}


/*********************************************************************/

uint8_t Adafruit_BMP085::read8(uint8_t a) {
    uint8_t ret;
    HAL_StatusTypeDef status = HAL_I2C_Mem_Read(&handle, BMP085_I2CADDR * 2, a, 1, &ret, 1, 100);
    halError(__FUNCTION__, status);
    return ret;
}


uint16_t Adafruit_BMP085::read16(uint8_t a) {
    uint8_t ret[2];
    HAL_StatusTypeDef status = HAL_I2C_Mem_Read(&handle, BMP085_I2CADDR * 2 + 1, a, 1,  ret, 2, 100);
    halError(__FUNCTION__, status);
    return ret[1] |(uint16_t)( ret[0] << 8u);
}

void Adafruit_BMP085::write8(uint8_t a, uint8_t d) {
    halError(__FUNCTION__, HAL_I2C_Mem_Write(&handle, BMP085_I2CADDR * 2 + 1, a, 1, &d, 1, 100));
}
