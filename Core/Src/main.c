/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "rtc.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

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
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_RTC_Init();
  MX_USART1_UART_Init();
  App_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
   if(Flag_Second)
   {
     Flag_Second = RESET;
       switch(VTS_State)                                                         // DO ACT AS PER VEHICLE STATUS
      {
      case MODEM_INIT : 
                        GP_timer();
                        
                        if(Flag_ModemRdy == SET)
                        {
                          /*none blocking mode Sir code is in blocking mode*/
                            INIT_GPRS_Modem(); 
                        };
          break;
      case NORMAL     :  
                  if((IAP_Counter > 10) && (IAP_Counter < 20))
                          {
                            Send_SMS(FAIL); 
                            
                            IAP_Counter = 0x0A;IAP_Status ='I';
                            GP_Counter = IAP_Counter << 8 | IAP_Status; 
        
                            LL_RTC_BAK_SetRegister(RTC,LL_RTC_BKP_DR3,GP_Counter);
                            
                            
//                            LL_RTC_BKP_SetRegister(TAMP,LL_RTC_BKP_DR3,'0');/*TBD*/
//                            LL_RTC_BKP_SetRegister(TAMP,LL_RTC_BKP_DR2,0x0A);/*TBD*/
                            LL_RTC_BAK_SetRegister(RTC,LL_RTC_BKP_DR1 ,'0');/*TBD*/
                            SYSTEM_SW_RESET;
                          }
                          else if(Flag_FlashProg)
                          { 
                            Get_Firmware();
                          }
                          else if(Flag_FOTA_DONE){__NOP();}
                          else IAP_Counter++;
                          
            break;
      default :   
        /*TBD*/
                       if((IAP_Counter >= 10) || (Flag_TryCnt >= 10)) 
                        {
                          Send_SMS(FAIL);
                          
                          IAP_Status = 'P';
                          GP_Counter = IAP_Counter << 8 | IAP_Status; 
                          LL_RTC_BAK_SetRegister(RTC,LL_RTC_BKP_DR3,GP_Counter);
                          SYSTEM_SW_RESET;
                        }
                       IAP_Counter++; 
                 
            break;
    };
    
   }
    
//    if(Flag_Second == SET)             
//    {
//      Flag_Second = RESET;
// 
//      switch(VTS_State)                                                         // DO ACT AS PER VEHICLE STATUS
//      {
//        
//      case MODEM_INIT : if(Init_Time)Init_Time--;                               // Modem Initialization Time
//                        else 
//                        {
//                           LL_RTC_BAK_SetRegister(RTC,LL_RTC_BKP_DR4,0);
//                          SYSTEM_SW_RESET;
//                        }
//                        if(Init_Time == 20)GPRS_ON;
//                        else if(Init_Time < 20)
//                        {
//                          if(Init_Time == 1)Flag_ModemRdy = SET;                  // if modem does not gives call ready
//                          if(Flag_NoResp == SET) Modem_PutString("AT");           // Sent AT for Autobauding 
//                          if(Flag_ModemRdy == SET)
//                          {
//                            Init_Time = 0;mSec_Delay(100);INIT_GPRS_Modem();     //if call ready 
//                            VTS_State = NORMAL; 
//                            if(Flag_SPIFail == SET)
//                            {
//                             Send_SMS(SPI_FAIL);
//                             LL_RTC_BAK_SetRegister(RTC,LL_RTC_BKP_DR4,0);
//                             SYSTEM_SW_RESET;
//                            };
//                          };
//                        }
//                        break;
//                        
//      case NORMAL     :   if(IAP_Counter >= 10){Send_SMS(FAIL);  SYSTEM_SW_RESET;  }
//                          else if(Flag_WrongFile){Send_SMS(WRONG_FILE);LL_RTC_BAK_SetRegister(RTC,LL_RTC_BKP_DR4,0);SYSTEM_SW_RESET;}
//                          else if(NoData_Count > 3){Send_SMS(UNABLE_TO_DWLOAD);LL_RTC_BAK_SetRegister(RTC,LL_RTC_BKP_DR4,0);SYSTEM_SW_RESET;}
////                          else if(Modem_NoResp >= 8)INIT_APP();
//                          else if(Flag_FlashProg)
//                          {
//                            IAP_Counter +=1;
//                            Get_Firmware();
//                          }
//                         break;
//                         
//                         
////      case SERIAL      :  if(IAP_Counter >= 10)
////                          {
////                            if(IAP_Status == 'S')
////                            {
////                              Serial_PutString_232("FAIL",3);
////                            }
////                            else 
////                            {
////                            Send_SMS(FAIL);
////                            }
////                            RTC_WriteBackupRegister(RTC_BKP_DR4,0);
////                            SYSTEM_SW_RESET;
////                          }
////                        
////                          GPRS_ON;   
////                          if(Flag_FlashProg)
////                          {
////                            IAP_Counter +=1;
////                            Get_Firmware_Serial();
////                            
////                          }
////                          else if(Flag_SaveFlash)
////                          {
////                            IAP_Counter +=1;
////                            StoreSPI_Flash();
////                          }
////                          else if(Flag_LoadProg)
////                          {
////                            IAP_Counter +=1;
////                            Program_Flash();
////                          } 
////                      else Flag_FW_Start = RESET;
////                         break;
////       default :   if(Flag_FWFail == RESET)
////                   {
////                       if(IAP_Counter >= 10)
////                        {
////                          if(IAP_Status == 'S')
////                          {
////                           /* Serial_PutString_232("FAIL",3);*/
////                          }
////                          else 
////                          {
////                            Send_SMS(FAIL);
////                          }
////                         LL_RTC_BAK_SetRegister(RTC,LL_RTC_BKP_DR4,0);
////                          SYSTEM_SW_RESET;
////                        }
////                       IAP_Counter++; 
////                   }
////                   else 
////                   {
////                     if(Flag_LogIn == RESET)
////                     {
////                       respond_time++;
////                       if(respond_time > 4)
////                       {
////                         respond_time = 0;
////                         Serial_PutString_232("Please Flash The Device",3);
////                         SYSTEM_SW_RESET;
////                       }
////                     }
////                     Flag_FlashProg = SET;
////                   }
////                   
////                    break;
//        
//      };// End of switch statement
//      
//    };// End of Second Flag
//    
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL4;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_RTC;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
