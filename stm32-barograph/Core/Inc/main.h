/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void cppMain(void) __attribute__((noreturn));
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define DISP_BUSY_CN10_24_Pin GPIO_PIN_1
#define DISP_BUSY_CN10_24_GPIO_Port GPIOB
#define DISP_RST_CN10_18_Pin GPIO_PIN_11
#define DISP_RST_CN10_18_GPIO_Port GPIOB
#define DISP_CS_CN10_16_Pin GPIO_PIN_12
#define DISP_CS_CN10_16_GPIO_Port GPIOB
#define DISP_CLK_CN10_30_Pin GPIO_PIN_13
#define DISP_CLK_CN10_30_GPIO_Port GPIOB
#define DISP_DC_CN10_28_Pin GPIO_PIN_14
#define DISP_DC_CN10_28_GPIO_Port GPIOB
#define DISP_DIN_CN10_26_Pin GPIO_PIN_15
#define DISP_DIN_CN10_26_GPIO_Port GPIOB
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define I2C_SCL_CN8_3_Pin GPIO_PIN_8
#define I2C_SCL_CN8_3_GPIO_Port GPIOB
#define I2C_SDA_CN8_5_Pin GPIO_PIN_9
#define I2C_SDA_CN8_5_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
