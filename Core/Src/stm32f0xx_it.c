/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f0xx_it.c
  * @brief   Interrupt Service Routines.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f0xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M0 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
   while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */
Modem_PutString("STUCK IN HARD FAULT");
  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVC_IRQn 0 */

  /* USER CODE END SVC_IRQn 0 */
  /* USER CODE BEGIN SVC_IRQn 1 */

  /* USER CODE END SVC_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
 
  Flag_mSec = SET;                                                                // SET MILI SECOND FLAG
  if(Flag_FWFail == SET)
  {
    if((Sec_Tick % 400) == 0)STATUS_LED_TOGGLE;
  }
  else if(Flag_Download)
  {
    if((Sec_Tick % 200) == 0)STATUS_LED_TOGGLE;
  }
  else if(Flag_SaveFlash)
  {
    if((Sec_Tick % 100) == 0)STATUS_LED_TOGGLE;
  }
  else if(Flag_LoadProg)
  {
    if((Sec_Tick % 50) == 0)STATUS_LED_TOGGLE;
  }
  else STATUS_LED_ON;
  
  if(Sec_Tick)Sec_Tick--; 
  else 
  {
    if(Flag_ValidNetwork){GPRS_LED_ON;}
    Sec_Tick = 999;
    Flag_Second = SET;
  };
  
  if(Config_Counter)Config_Counter--;
  
  
  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */
  
  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f0xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles USART1 global interrupt / USART1 wake-up interrupt through EXTI line 25.
  */
void USART1_IRQHandler(void)
{
  unsigned char RcvIn;
  
 if(LL_USART_IsActiveFlag_RXNE(USART1) )                        //Checking for USART1 interrupt has occurred or not.
  {
    RcvIn = (unsigned short)(USART1->RDR & (unsigned short)0x01FF);
 
    
    if(RcvIn != 0)
    {
      if(Flag_GetFile) 
      {
        Data_WaitTime = 60;

        if(Flag_GetFile == SET) 
    {
       Data_WaitTime = 30;
       Rev_Buffer[Rev_Count] = RcvIn;
       Rev_Count ++;
       if((Rev_Count % FOTA_PKT)==0)
       {
         Flag_WriteFile = SET;
       }
    }
        
        //        Rev_Buffer[Rev_Count] = RcvIn;   TBD
//        Rev_Count ++;
//        
//        /* this line is missing from here 
//        if((Rev_Count % FOTA_PKT)==0)Flag_WriteFile = SET;
//        */
//        
//        /*-------------TBD---------------*/
//        if (FileLength > FOTA_PKT)
//        {
//          if(Flag_NextEndOfSector)
//          {
//            if(Rev_Count % (FOTA_PKT+59) == 0)
//            {
//              Flag_NextEndOfSector = RESET;
//              Flag_WriteFile = SET;  // Set Flag_WriteFile to SET
//              Rev_Count = 0;         // Reset Rev_Count
//            }
//          }
//          else if ((!Flag_NextEndOfSector) &&(FileLoc < FOTA_PKT) && (!Flag_EndofSector))
//          {
//            if (Rev_Count % (FOTA_PKT+57) == 0)  //
//            {   
//              Flag_EndofSector = RESET;
//              Flag_WriteFile = SET;  // Set Flag_WriteFile to SET
//              Rev_Count = 0;         // Reset Rev_Count
//            }
//          }
//          else if (!Flag_NextEndOfSector && Flag_EndofSector)
//          {
//            if (Rev_Count % (FOTA_PKT+87) == 0)  //
//            {   
//              Flag_EndofSector = RESET;
//              Flag_WriteFile = SET;  // Set Flag_WriteFile to SET
//              Rev_Count = 0;         // Reset Rev_Count
//            }
//          }
//          else
//          {
//            if (Rev_Count % (FOTA_PKT+42) == 0) //
//            {
//              Flag_WriteFile = SET;  // Set Flag_WriteFile to SET
//              Rev_Count = 0;         // Reset Rev_Count
//            }
//          }
//        } 
//        else
//        {
//          if (Rev_Count % (FileLength+13) == 0)
//          {
//            Flag_WriteFile = SET;  // Set Flag_WriteFile to SET
//            Rev_Count = 0;         // Reset Rev_Count
//          }
//        }
        /*----------------------------------------------*/  
      }
      else
      {
        if(RcvIn == 0x0A)
        {
          /*----- +CGREG --------------------------------------------------------*/
          
          if((strstr((char*)GPRS_Buffer,"REG: 1"))||(strstr((char*)GPRS_Buffer,"REG: 2,0"))/*||(strstr((char*)GPRS_Buffer,"REG: 2,1"))*/)
          {
            GPRS_Count = 0;Flag_ValidNetwork = RESET;/*Network_Status = Get_Signal_Strength ;Flag_Modem_process_ongoing = RESET;*/
          }    
          
          /*----- STK RESPONSE ---------------------------------------------------*/  
          if(strstr((char*)GPRS_Buffer,"STKGI: "))
          {
            if (strstr((char*)GPRS_Buffer,"STKGI: 37") && strstr((char*)GPRS_Buffer,"OK"))Flag_ModemOK = SET;
            else if((strstr((char*)GPRS_Buffer,"STKGI: 36") && strstr((char*)GPRS_Buffer,"OK")))Flag_ModemOK = SET;
          }
          /*----- ERROR RESPONSE ------------------------------------------------*/
          else if(strstr((char*)GPRS_Buffer,"+CME ERROR:") || (strstr((char*)GPRS_Buffer,"ERROR"))){/*Flag_Error = SET;*/GPRS_Count = 0;}
          
        }
        else
        {
          if(RcvIn != 0x0D)
          {
            GPRS_Buffer[GPRS_Count] = RcvIn;
            GPRS_Count++;
          }
          if(GPRS_Count >= 2048)GPRS_Count = 0;
        }
      }
    }
  }
  else
  {
    LL_USART_ClearFlag_TC  (USART1);
    LL_USART_ClearFlag_ORE (USART1);
    LL_USART_ClearFlag_FE  (USART1);
    LL_USART_ClearFlag_NE  (USART1);
    LL_USART_ClearFlag_PE  (USART1);
    LL_USART_ClearFlag_IDLE(USART1);
    LL_USART_ClearFlag_EOB (USART1);
    RcvIn = (unsigned short)(USART1->RDR & (unsigned short)0x01FF);
  };
  
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
