#include "main.h"

void App_Init(void)
{
 GPRS_ON; 
    
IAP_Status   = LL_RTC_BAK_GetRegister(RTC,LL_RTC_BKP_DR4) & 0x00ff;                   // READ IAP STATUS 
IAP_Counter  = LL_RTC_BAK_GetRegister(RTC,LL_RTC_BKP_DR4) >> 8;                       // READ IAP COUNTER
  
    //else 
  //{
//    sprintf(SER_USN,"MMI");     tbd
//    sprintf(SER_PWD,"MMI@1234");tbd
    
    sprintf(DFU_HTTP.IP,"35.154.23.247");
    sprintf(DFU_HTTP.PATH,"EFOTA");
    sprintf(DFU_HTTP.FILENAME,"MMI-1.0.0");
    sprintf(DFU_HTTP.URL,"http://%s/%s/%s.bin",DFU_HTTP.IP ,&DFU_HTTP.PATH[0],&DFU_HTTP.FILENAME[0]);
      
  //}
  
  
  
  /*----- INITIALIZE DEVICE --------------------------------------------------*/
  if(VTS_State > 2)VTS_State = MODEM_INIT;                                                       // FIRST INITIALIZE GPRS MODEM   
  
  Flag_ModemRdy = RESET;                                                        // MODEM READY RESET       
  Flag_ValidNetwork = RESET;                                                    // VALID NETWORK RESET 
  Flag_APNSet = RESET;                                                          // APN NAME RESET 
  Flag_NoResp = SET;                                                            // INITIAL NO RESPONSE FROM MODEM 
  
  Init_Time = 25;                                                               // MODEM INITIALIZATION TIME 15 SECONDS
  Modem_NoResp = 0;                                                             // RESET MODEM NO RESPONSE 
 
 /* if device face any error or programm successfully it will go to the application firmware*/
  if(((IAP_Status == 'E') && (IAP_Counter == 255)) || (IAP_Status == 'P'))
  {
    goto APP_FW;
  }
  if((IAP_Status == 'I') || (IAP_Status == 'S'))Flag_FlashProg = SET;
  
  if(IAP_Status == 'S')
  {
    //Flag_SerialFlash = SET;tbd
//    VTS_State = SERIAL;
    GPRS_ON;  
  }
  
  //INIT_APP();                                                                   // INITIALIZE APPLICATION
  
  if((Flag_FlashProg == SET) && (IAP_Counter < 10))
  {
    Flag_LoadProg = RESET;  
//    if(Flag_SerialFlash){ GP_Counter = IAP_Counter << 8 | 'S'; }tbd
//    else
      GP_Counter = IAP_Counter << 8 | 'I'; 
    LL_RTC_BAK_SetRegister(RTC,LL_RTC_BKP_DR4,GP_Counter);                            // will be cleared after sucess programming
  }
  else if(IAP_Status == 'E')
  {
//    if(Flag_SerialFlash){ GP_Counter = IAP_Counter << 8 | 'S'; }tbd
//    else 
      GP_Counter = IAP_Counter << 8 | 'I';
      LL_RTC_BAK_SetRegister(RTC,LL_RTC_BKP_DR4,GP_Counter);
 
    SYSTEM_SW_RESET;
   }  
  else if(IAP_Status == 'R')Flag_SaveFlash = SET;
//  else if(IAP_Status == 'D')Program_Flash();      tbd
 
  /* Keep the user application running */
  else
  {   
  APP_FW:   
    
    if((IAP_Status == 'E') && (IAP_Counter == 255))
    {
      //Serial_PutString_232("FAIL",3);   tbd
    }
   /* Test if user code is programmed starting from address "ApplicationAddress" */
    if (((*(__IO uint32_t*)ApplicationAddress) & 0x2FFE0000 ) == 0x20000000)
     { 
      /* Jump to user application */
      JumpAddress = *(__IO uint32_t*) (ApplicationAddress + 4);
      Jump_To_Application = (pFunction) JumpAddress;
      /* Initialize user application's Stack Pointer */
      __set_MSP(*(__IO uint32_t*) ApplicationAddress);
      Jump_To_Application();
     }
    else 
    {
        if(IAP_Status == 'D')
        {
          IAP_Counter = 5;
          Flag_FlashProg = RESET;
          Flag_SaveFlash = RESET;
          Flag_LoadProg = SET;
          VTS_State = NORMAL;
        }
        else 
        {
         // if(IAP_Status == 0)Flag_SerialFlash = SET;
          //if(Flag_SerialFlash){ GP_Counter = IAP_Counter << 8 | 'S'; }   tbd
            GP_Counter = IAP_Counter << 8 | 'I'; 
          LL_RTC_BAK_SetRegister(RTC,LL_RTC_BKP_DR4,GP_Counter);
          Flag_FWFail = SET;
//          if(Flag_SerialFlash)VTS_State = SERIAL;   tbd
//          else 
            VTS_State = MODEM_INIT; 
          Flag_FlashProg = SET;
        }
    };                                                                          // return to IAP if Flash is not correct
  }
  
  
  //Flag_PowerON = SET;
 if(IAP_Status == 'S')
 {
  IAP_Counter += 2;
  memset(Rev_Buffer,0,sizeof(Rev_Buffer));
  Rev_Count = 0;
  
   memset(GPRS_Buffer,0,50);
   GPRS_Count = 0;
    
//   if(Flag_SPIFail == SET)  tbd
//    {
////     Serial_PutString_232("SPI FLASH FAIL",3); 
////     LL_RTC_BAK_SetRegister(RTC_BKP_DR4,0);
////     mSec_Delay(1000);
//     SYSTEM_SW_RESET;
//    }
//   else 
//   {
//     //Serial_PutString_232("OK",3); 
//   }
 }
  
}
/*******************************************************************************
* Function Name  : GP_timer
* Description    : None
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GP_timer(void)
{
  //  Get_RTC_Time();
  
  Flag_Second = RESET;
  
  /* Inital Turn On Delay for Modem*/
  
  if(EC200_ONTime)EC200_ONTime--; //tbd
  
  /*Send ST CMD After Some time*/
  if(EC200_ONTime > 0 && EC200_ONTime < 6)
  {
    if(EC200_ONTime == 5)Modem_PutString("AT");
    
    /*Modem Is Ready to Init*/
    if(strstr((char*)GPRS_Buffer,"PB DONE") || EC200_ONTime == 1)
    {
      Flag_ModemRdy = SET;
      EC200_ONTime = 0;
      Flag_Init_GPRS = SET;
    };
  }
}


