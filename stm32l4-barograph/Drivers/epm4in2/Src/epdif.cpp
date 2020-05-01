/**
 *  @filename   :   epdif.cpp
 *  @brief      :   Implements EPD interface functions
 *                  Users have to implement all the functions in epdif.cpp
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare     August 10 2017
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documnetation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to  whom the Software is
 * furished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "epdif.h"
#include "spi.h"

EpdIf::EpdIf() = default;

void EpdIf::DigitalWriteDC(bool value) {
    HAL_GPIO_WritePin(DISP_DC_GPIO_Port, DISP_DC_Pin, value ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void EpdIf::DigitalWriteCS(bool value) {
    HAL_GPIO_WritePin(DISP_CS_GPIO_Port, DISP_CS_Pin, value ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void EpdIf::DigitalWriteRST(bool value) {
    HAL_GPIO_WritePin(DISP_RST_GPIO_Port, DISP_RST_Pin, value ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

int EpdIf::DigitalReadBusy() {
    return HAL_GPIO_ReadPin(DISP_BUSY_GPIO_Port, DISP_BUSY_Pin);
}

void EpdIf::DelayMs(unsigned int delaytime) {
    HAL_Delay(delaytime);
}

void EpdIf::SpiTransfer(unsigned char data) {
    DigitalWriteCS(false);
    halError(__FUNCTION__, HAL_SPI_Transmit(&hspi1, &data, 1, 100));
    DigitalWriteCS(true);
}

int EpdIf::IfInit() {
    return 0;
}
