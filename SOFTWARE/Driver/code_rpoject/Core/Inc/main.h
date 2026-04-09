/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32g4xx_hal.h"

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

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_GREEN_Pin GPIO_PIN_14
#define LED_GREEN_GPIO_Port GPIOC
#define LED_ORANGE_Pin GPIO_PIN_15
#define LED_ORANGE_GPIO_Port GPIOC
#define ST_LINK_RX_Pin GPIO_PIN_0
#define ST_LINK_RX_GPIO_Port GPIOC
#define ST_LINK_TX_Pin GPIO_PIN_1
#define ST_LINK_TX_GPIO_Port GPIOC
#define ADDR_BIT1_Pin GPIO_PIN_2
#define ADDR_BIT1_GPIO_Port GPIOC
#define ADDR_BIT0_Pin GPIO_PIN_3
#define ADDR_BIT0_GPIO_Port GPIOC
#define VBUS_monitoring_Pin GPIO_PIN_0
#define VBUS_monitoring_GPIO_Port GPIOA
#define INL1_Pin GPIO_PIN_8
#define INL1_GPIO_Port GPIOE
#define INH1_Pin GPIO_PIN_9
#define INH1_GPIO_Port GPIOE
#define INl2_Pin GPIO_PIN_10
#define INl2_GPIO_Port GPIOE
#define INH2_Pin GPIO_PIN_11
#define INH2_GPIO_Port GPIOE
#define INL3_Pin GPIO_PIN_12
#define INL3_GPIO_Port GPIOE
#define INH3_Pin GPIO_PIN_13
#define INH3_GPIO_Port GPIOE
#define ENC_CS_Z_Pin GPIO_PIN_2
#define ENC_CS_Z_GPIO_Port GPIOD
#define ENC_CS_Z_EXTI_IRQn EXTI2_IRQn
#define TEST_ADC_Pin GPIO_PIN_6
#define TEST_ADC_GPIO_Port GPIOB
#define LIMIT_SWITCH2_Pin GPIO_PIN_7
#define LIMIT_SWITCH2_GPIO_Port GPIOB
#define LIMIT_SWITCH1_Pin GPIO_PIN_8
#define LIMIT_SWITCH1_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
