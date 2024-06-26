/**
  ******************************************************************************
  * @file    SPI_FLASH.h
  * @brief   This file provides code for the configuration
  *          of the SPI instances.
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
**/
#ifndef __SPI_FLASH_H__
#define __SPI_FLASH_H__


#include "main.h"

extern SPI_HandleTypeDef hspi1;
/* Private typedef -----------------------------------------------------------*/
#define SPI_FLASH_PageSize    0x100

// As per Winbond
/* Private define ------------------------------------------------------------*/
#define WRITE      0x02  /* Write to Memory instruction */
#define WRSR       0x01  /* Write Status Register instruction */
#define WREN       0x06  /* Write enable instruction */

#define READ       0x03  /* Read from Memory instruction */
#define RDSR       0x05  /* Read Status Register instruction  */
#define RDID       0x9F  /* Read identification */
#define SE         0x20  /* Sector Erase instruction */
#define BE         0xD8  /* Block Erase instruction */ 
#define CE         0xC7  /* Bulk Erase instruction */

#define PD         0xB9  /* Deep Power DOWN Mode */

#define WIP_Flag   0x01  /* Write In Progress (WIP) flag */

#define COLUMN          80     // Board column defination

#define ROW             0x16
#define DISABLE_COLUMN  0x40
#define Dummy_Byte      0xFF
#define RDID            0x9F  /* Read identification */
#define FLASH_ID        0xEF4018 

 

unsigned char SPI_FLASH_SendByte(uint8_t byte);
uint32_t W25Q_ReadID(void);
unsigned long SPI_FLASH_ReadID(void);
void SPI_FLASH_BufferWrite(char* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void SPI_FLASH_WaitForWriteEnd(void);
unsigned char SPI_FLASH_ReadByte(void);
void SPI_FLASH_PageWrite(char* pBuffer, unsigned long WriteAddr, unsigned short NumByteToWrite);
void SPI_FLASH_SectorErase(unsigned long SectorAddr );
void SPI_FLASH_BufferRead(char* pBuffer, unsigned long ReadAddr, unsigned short NumByteToRead);
void    FLASH_PageErase(uint32_t PageAddress);
void   FLASH_Program_HalfWord(uint32_t Address, uint16_t Data);
HAL_StatusTypeDef FLASH_OB_RDP_LevelConfig(uint8_t ReadProtectLevel);
  HAL_StatusTypeDef FLASH_OB_EnableWRP(uint32_t WriteProtectPage);

#endif
