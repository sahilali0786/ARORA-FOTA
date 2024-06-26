/******************** (C) COPYRIGHT 2024 EDS ***********************************
* @File Name          : SMS.c
* @Author             : SANTOSH KUMAR
* @Version            : V1.0
* @Date               : 15/01/2024
* @Description        : SMS
*******************************************************************************/
#include "main.h"

/*******************************************************************************
* Function Name : SMS_Init
* Description   : Initialize SMS
* Input         : None
* Output        : None
* Return        : None
*******************************************************************************/
void SMS_Init(void)
{
  Modem_PutString("AT+CSMS=1");
  Modem_PutString("AT+CMGF=1");
  Modem_PutString("AT+CSMP=17,167,0,241");
  Modem_PutString("AT+CPMS=\"SM\",\"SM\",\"SM\"");
  Modem_PutString("AT+CSDH=1");
}


/*******************************************************************************
* Function Name : SMS_Send
* Description   : Send SMS
* Input         : SMS_CmdType_TypeDef SMS_CmdType
* Output        : None
* Return        : None
*******************************************************************************/
void Send_SMS(unsigned char Subject)
{
  
 char Temp_Buff[50];
 
 if(DFU_HTTP.DFU_Mob[0] == 0x00)sprintf(DFU_HTTP.DFU_Mob, "8287136204");
    
 if((DFU_HTTP.DFU_Mob[0] > '3') && (DFU_HTTP.DFU_Mob[0] <= '9'))
 {
  sprintf(Temp_Buff,"AT+CMGS=\"+91%s\"",DFU_HTTP.DFU_Mob);                 // SEND COMMAND TO START SMS
  Serial_PutData_GPRS(Temp_Buff,strlen(Temp_Buff));

  Serial_PutString_GPRS(" Device-");Serial_PutString_GPRS(GPRSData.IMEI_No);

  switch(Subject)
   {
     default                         : break;
     case FAIL                       : Serial_PutString_GPRS("-Maximum Attempts Reached, Rollback to last version");break;
     case GOLDEN_SWITCH_FAILED       : Serial_PutString_GPRS("-GOLDEN Jump Attempts Failed & Try To Jump Fota");break;
     case FOTA_SWITCH_FAILED         : Serial_PutString_GPRS("-FOTA Jump Attempts Failed & Try To Jump Golden");break;
     case UNABLE_TO_CONNECT          : Serial_PutString_GPRS("-Unable to connect FOTA Server");break;
     case WRONG_URL                  : Serial_PutString_GPRS("-Unable to connect to the server: Incorrect server URL");break;
     case UNABLE_TO_DWLOAD           : Serial_PutString_GPRS("-Unable to Download");break;
     case WRONG_FILE                 : Serial_PutString_GPRS("-Wrong File on Server,");break;
     case SPI_FAIL                   : Serial_PutString_GPRS("-SPI FLASH FAIL");break;
     case SPI_OK                     : Serial_PutString_GPRS("-SPI FLASH OK");break;
     case ERROR                      : Serial_PutString_GPRS("-Download Error ");break;
     case MISSINGDATA                : Serial_PutString_GPRS("-MISSING Data ");break;
     case SUCCESS                    : sprintf(Temp_Buff,"-FOTA SUCCESSFULLY DONE");
                                       //Flag_Fota_Finish = SET; /* when fota successfully done this Flag set*/
                                       Serial_PutString_GPRS(Temp_Buff);break;
     case RECORD_FAIL                : Serial_PutString_GPRS("-Failed to save status file on server");break;
     case HTTP_REQUEST_SUCCESS       : Serial_PutString_GPRS("-HTTP REQUEST SUCCESS");break;
 
   }

  LL_RTC_BAK_SetRegister(RTC,LL_RTC_BKP_DR4,GP_Counter);
  
  
   IAP_Status = LL_RTC_BAK_GetRegister(RTC, LL_RTC_BKP_DR3) & 0x00ff;           // READ IAP STATUS
    Serial_SendData_GPRS(10); // <LF> - LINE FEED
    Serial_SendData_GPRS(26); // SUBSTITUTE
    
    
    /* putting delay because after message send modem get busy and send ok response late  */
    mSec_Delay(5000);
    Modem_PutString("AT+CMGD=1,4");
    mSec_Delay(5000);
    /* when flag fota finish system will restart*/
   if((IAP_Counter >= 10))
    {
//      Flag_Fota_Finish = RESET; TBD
     IAP_Counter = 0;
     
     LL_RTC_BAK_SetRegister(RTC,LL_RTC_BKP_DR3,0);                              // RETURNING TO PREVIOUS VERSION
     SYSTEM_SW_RESET;
    }
 }
}                        
