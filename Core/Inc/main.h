/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "stm32f0xx_hal.h"

#include "stm32f0xx_ll_rtc.h"
#include "stm32f0xx_ll_usart.h"
#include "stm32f0xx_ll_rcc.h"
#include "stm32f0xx_ll_system.h"
#include "stm32f0xx_ll_gpio.h"
#include "stm32f0xx_ll_exti.h"
#include "stm32f0xx_ll_bus.h"
#include "stm32f0xx_ll_cortex.h"
#include "stm32f0xx_ll_utils.h"
#include "stm32f0xx_ll_pwr.h"
#include "stm32f0xx_ll_dma.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Misc_Function.h"
#include "App_RAM.h"
#include "GPRS.h"
#include "string.h"
#include "FOTA.h"
#include "stdio.h"
#include "stdlib.h"
#include "ctype.h"
#include "SMS.h"
 
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
#define GPRS_ON   GPIOB -> BRR = LL_GPIO_PIN_8
#define GPRS_OFF  GPIOB -> BSRR  = LL_GPIO_PIN_8
  
  
#define STATUS_LED_ON          STATUS_LED_GPIO_Port->BRR  = STATUS_LED_Pin           // STATUS LED ON
#define STATUS_LED_OFF         STATUS_LED_GPIO_Port->BSRR = STATUS_LED_Pin           // STATUS LED OFF
#define STATUS_LED_TOGGLE      STATUS_LED_GPIO_Port->ODR ^= STATUS_LED_Pin           // STATUS LED TOGGLE
  
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/*----- FLASH PROGRAMMING FOR WRITTING CONFIG TABLE --------------------------*/
#define ApplicationAddress      ((uint32_t)0x08005000)
  
  
  
#define EndAddr                 ((uint32_t)0x08040000)                               // MICRO FLASH LAST ADDRESS BUT ONE PAGE 
//#define FLASH_PAGE_SIZE         ((uint16_t)0x800)                                    // MICRO FLASH PAGE SIZE

/*----- SECONDARY SPI FLASH WRITTING DATA TABLE ------------------------------*/
#define SPIFLASH_FOTA           0xF00000                                        // FOR STORING ONLY PROGRAM DATA
#define SPIFLASH_DATA           0xFB0000                                        // FOR STORING ALL HEX 

//#define FOTA_PKT                8192                                            // FOTA Packet Download size

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE BEGIN EFP */
#define IS_AF(c)  ((c >= 'A') && (c <= 'F'))
#define IS_af(c)  ((c >= 'a') && (c <= 'f'))
#define IS_09(c)  ((c >= '0') && (c <= '9'))
#define ISVALIDHEX(c)  IS_AF(c) || IS_af(c) || IS_09(c)

#define CONVERTHEX_alpha(c)  (IS_AF(c) ? (c - 'A'+10) : (c - 'a'+10))
#define CONVERTHEX(c)   (IS_09(c) ? (c - '0') : CONVERTHEX_alpha(c))
/* USER CODE END Private defines */
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
#define IS_AF(c)  ((c >= 'A') && (c <= 'F'))
#define IS_af(c)  ((c >= 'a') && (c <= 'f'))
#define IS_09(c)  ((c >= '0') && (c <= '9'))
#define ISVALIDHEX(c)  IS_AF(c) || IS_af(c) || IS_09(c)

#define CONVERTHEX_alpha(c)  (IS_AF(c) ? (c - 'A'+10) : (c - 'a'+10))
#define CONVERTHEX(c)   (IS_09(c) ? (c - '0') : CONVERTHEX_alpha(c))
/* USER CODE END Private defines */
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define GPRS_TX_Pin LL_GPIO_PIN_9
#define GPRS_TX_GPIO_Port GPIOA
#define GPRS_RX_Pin LL_GPIO_PIN_10
#define GPRS_RX_GPIO_Port GPIOA
#define GPRS_LED_Pin LL_GPIO_PIN_11
#define GPRS_LED_GPIO_Port GPIOC
#define GPS_LED_Pin LL_GPIO_PIN_12
#define GPS_LED_GPIO_Port GPIOC
#define STATUS_LED_Pin LL_GPIO_PIN_2
#define STATUS_LED_GPIO_Port GPIOD
#define GPRS_Pin LL_GPIO_PIN_8
#define GPRS_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
