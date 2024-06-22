/******************** (C) COPYRIGHT 2023 EDS INDIA *****************************
* File Name     : VTS_Functions.c					       *
* Author        : TEC SANTOSH  KUMAR                                           *
* Date          : 09/11/2023                                                   *
* Description   : This file contains all misc functions of VTS                 *
* Revision	: Rev0    						       *
********************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "main.h"

HTTP_InitTypeDef       DFU_HTTP;

uint8_t Data_WaitTime;
/* USER CODE BEGIN PD */

uint32_t FirstPage = 0, NbOfPages = 0, BankNumber = 0;
uint32_t Address = 0, PageError = 0;
__IO uint32_t MemoryProgramStatus = 0;
__IO uint32_t data32 = 0;

/*Variable used for Erase procedure*/
static FLASH_EraseInitTypeDef EraseInitStruct; 
//static uint32_t GetPage(uint32_t Address); 
//static uint32_t GetBank(uint32_t Address); 




typedef  void (*pFunction)(void);
pFunction Jump_To_Application;


uint32_t AppStart_Address = 0;
uint32_t Jump_Addr = 0;


char* charToHex(char* input); 
extern FlagStatus Flag_FWFail,
                  Flag_FOTA_GETPATH;

long
StartAddress,                                                                   // START ADDRESS FLASH DATA
Check_FOTA_CodeAddr,
FlashAddress;


FlagStatus
Flag_Golden_Try,
Flag_TryCnt,
Flag_WrongFile,
Flag_FlashProg,
Flag_FOTA_Try,
Flag_DFUConnect_Retry,                                                                // TRY TO CONNECT DFU FTP SERVER FOR DOWNLOADING FILE 
Flag_Download_Status,
//Flag_Download,                                                                  // DOWNLOADING STARTED
Flag_DFU,
Flag_GetFile,                                                                   // DOWNLOADING STARTING FROM FTP SERVER
Flag_FileConnect,                                                                // FTP SERVER CONNECTED 
Flag_WriteFile,                                                                 // SAVE RECEIVED DATA INTO FLASH
Flag_HTTP_Content,
Flag_EndofSector,
Flag_NextEndOfSector,
Flag_Int_FlashError,
Flag_SaveFlash,
Flag_LoadProg,
Flag_DataError,
Flag_FWFail;                                                                // ERROR GENERATED

uint32_t FileHandel = 0;

unsigned short

Write_Loc;

/* Private typedef -----------------------------------------------------------*/
 
 char Hex_Deta[33];
 char Program_Dbuff[FOTA_PKT + 200],
   Rev_Buffer[Reveive_Buffer_Size];                                               // BUFFER TO RECEIVE BYTES FROM FTP SERVER 

 
 
 char *StrPtr = NULL;
 
 unsigned short
Check_Systick,
Write_Loc,                                                                      // WRITE LOCATION OF FLASH DATA FROM RECEIVING BUFFER 
Flash_Write_Size,
//Rev_Count,                                                                      // COUNTER OF RECEIVING BYTES @ GPRS USART INTERRUPT
Loc;                                                                            // SAVE LAST LOCATION OF RECEIVE COUNT AT THE END OF RECEIVING FTP FILE 


// Define an enum for HTTP(S) error codes
  uint32_t FirstSector, NbOfSectors, BankNumber,PageError;

  void DataErr(void);
unsigned int NOLs =0, WrnData = 0;
int Wait = 80;
uint32_t FileLength = 0; //tbd
uint32_t FileLoc = 0;

FlagStatus
Flag_FOTA_FILE_Addr, 
Flag_EndofFile ;
/*******************************************************************************
* Function Name  : mSec_Delay
* Description    : Insert Delay of milli Second
* Input          : val Number of cycle
* Output         : None
* Return         : None
*******************************************************************************/
void mSec_Delay(unsigned int nTime)
{

 while(nTime != 0)                                                              // Msec Delay 
  { 
    if(Flag_mSec == SET){Flag_mSec = RESET;nTime--;};                           // MSec Flag is set by Sys tick timer
  };
 
}
/*******************************************************************************
* Function Name : Get Firmware
* Description   : Get firmware file from FTP
* Input         : None
* Output        : None
* Return        : None
*******************************************************************************/
void Get_Firmware(void)
{
  char* Ptr=NULL;
  char  buff[200];
    /*----- CHECK SIGNALS & NETWORK --------------------------------------------*/
  Check_Signal_Nw_GPRS();                                                       // CHECK SIGNAL STRENGTH NETWORK REGISTRATION GPRS ATTACHMENT 
  Get_CSQ++;
  if(Get_CSQ > 31)SYSTEM_SW_RESET;
  if(Flag_ValidNetwork == SET && Flag_APNSet == RESET){Get_CSQ = 0; Set_APN();}               // IF NETWORK VALID SET APN
  
  if(Flag_APNSet )
  {
    
      Serial_PutString_GPRS("AT+QHTTPURL=");                                    // AT+OHTTPURL - set URL address
      memset(buff,0,100);
     // if(DFU_HTTP.URL[0] != 'h')
     if((DFU_HTTP.IP[0] != 0)&& (DFU_HTTP.PATH[0] != 0) && (DFU_HTTP.FILENAME[0] != 0))
       sprintf(DFU_HTTP.URL,"http://%s/%s/%s.bin",DFU_HTTP.IP,DFU_HTTP.PATH,DFU_HTTP.FILENAME);
      else
      {
        IAP_Counter++;
        //Send_SMS(WRONG_URL);tbd
        
        LL_RTC_BAK_SetRegister(RTC,LL_RTC_BKP_DR2,IAP_Counter);
        SYSTEM_SW_RESET;
      }
      
      memset(buff,0,sizeof(buff)); 
      
     sprintf(buff,"%d,10",strlen(DFU_HTTP.URL));
      Serial_PutString_GPRS(buff);                                               // URL address data length
      Serial_SendData_GPRS(13); Serial_SendData_GPRS(10);                        // Address upload time
      //Wait_For_Resp();     tbd      

      Serial_PutString_GPRS(DFU_HTTP.URL);                                       // Send URL Address
     
      Serial_SendData_GPRS(13); Serial_SendData_GPRS(10);             
      //Wait_For_Resp();  tbd
      memset(Rev_Buffer,0,sizeof(Rev_Buffer));
      while (Wait)
      {
        mSec_Delay(100);
        if(Flag_Second){Flag_Second = RESET; /*LL_IWDG_ReloadCounter(IWDG); */  Wait--;}
        if(Wait == 1){ Flag_APNSet = RESET; return;} // try again         
        if(Ptr = strstr(GPRS_Buffer,"CONNECT"))
        {
           
          Wait = 0;
        }
        else if(Ptr = strstr(GPRS_Buffer,"ERROR: "))
          SYSTEM_SW_RESET;
      }
 
      /*--------------------- Get File Request  ------------------------------------*/
      Wait = 120;
      Modem_PutString("AT+QHTTPGET=80");WAIT_MODEM_RESP(2); 
      FileLength = 0;
      while(Wait)
      {
        mSec_Delay(100);
        if(Flag_Second){Flag_Second = RESET; /*LL_IWDG_ReloadCounter(IWDG);*/   Wait--;}
        if(Wait == 1){ Flag_APNSet = RESET; return;} // try again         
        if(Ptr = strstr(GPRS_Buffer,"OK"))
        {
          Wait = 0;
        } 
        else if(strstr(GPRS_Buffer,"ROR: "))
          SYSTEM_SW_RESET;
      }
      
  
      
    /*----- GET FIRMWARE FILE URL ---------------------------------------------------*/
       

       /*--------------------- File Open in Readonly mode ------------------------*/
        Wait = 200;
       Modem_PutString("AT+QHTTPDL=\"FOTA.txt\",1024");WAIT_MODEM_RESP(5);
 
       while(Wait)
       {
         LL_mDelay(10);
         if(Flag_Second){Flag_Second = RESET; /*LL_IWDG_ReloadCounter(IWDG);*/Wait--;}
         if(Wait == 1){ Flag_APNSet = RESET; return;} // try again
         if(Ptr = strstr(GPRS_Buffer,"PDL:"))
          {
            Flag_Download_Status = SET;
            Ptr += 5;
            Ptr = strtok(Ptr, ",");
            Ptr = strtok(NULL, ",");
            FileLength = atoi(Ptr);
            Wait = 0;
          }
         else if(strstr(GPRS_Buffer,"ROR:"))
           SYSTEM_SW_RESET;
       }
        
       
       uint32_t FileHandel = 0;
       Wait = 80;
      
      Modem_PutString("AT+QFOPEN=\"FOTA.txt\",2");WAIT_MODEM_RESP(5); // Open in Readonly mode
      while(Wait)
      {
        mSec_Delay(100);
        if(Flag_Second){Flag_Second = RESET;  Wait--;}
        if(Wait == 1){ Flag_APNSet = RESET; return;} // try again
        if(Ptr = strstr(GPRS_Buffer,"OPEN: "))
        {
          Ptr += 6;
          Wait = 0;
          FileHandel = atoi(Ptr);
          Flag_Download_Status  = RESET;
          Flag_SaveFlash = SET;
        }
        else if(Ptr = strstr(GPRS_Buffer,"ERROR: "))
          SYSTEM_SW_RESET;
      } 
       
       /*---------------------- Start Downloading in Flash ---------------------*/
       FlashAddress        = FOTA_IMG_ADDR;
       long StartFotaAddr  = FOTA_IMG_ADDR;
       Write_Loc = 0;
       unsigned char Calc_ChkSum = 0, CheckSum = 0;
       uint32_t FlashCodeAddr = 0;
       uint16_t GP_Counter = 0;
       unsigned char No_OfBytes = 0, RecordType = 0;
       Modem_NoResp = 0;
       Flag_Download = SET;
       //Flag_DataError = SET;
       /*--------------------- Erase Flash -------------------------------------*/
       if (FlashAddress != 0x00000000)
       {

         HAL_FLASH_Unlock();            


//        __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
         
         /* clear all error flags*/
         __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP);
         __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_WRPERR);
         __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_PGERR);
         
         /*taking first page of flash */   //TBD
//        FirstPage = GetPage(FlashAddress);
        
        /* taking number of pages*/
//        NbOfPages = GetPage(End_Address) - FirstPage;
        
//        BankNumber = GetBank(FlashAddress);

        EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
        //EraseInitStruct.Banks       = BankNumber;
        EraseInitStruct.PageAddress        = FirstPage;
        EraseInitStruct.NbPages     = NbOfPages;


        if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK) Error_Handler();

        FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);
         
      }
      
      mSec_Delay(100);
      /*------------------- Read Data from Modem ----------------*/
     // FileLength = 131597;
     // FileHandel = 19202049;
      while(FileLength)
       {
         sprintf(buff,"AT+QFSEEK=%d,%d,0",FileHandel,FileLoc);
         Modem_PutString(buff);WAIT_MODEM_RESP(5);                      // Open in Readonly mode
         
         int read_length = FOTA_PKT;  // Default read length
         
         if (FileLoc < FOTA_PKT) { read_length += 40; FileLoc += 40; FileLength -= 40;}
         
         else if (Flag_NextEndOfSector) {  read_length += 42; FileLoc += 42; FileLength -= 42;}
         
         else if (Flag_EndofSector) {  read_length += 70; FileLoc += 70; FileLength -= 70;}
         
         else if (FileLength < FOTA_PKT) read_length = FileLength;
         
         else { read_length += 25; FileLoc += 25; FileLength -= 25;}
         
         sprintf(buff, "AT+QFREAD=%d,%d", FileHandel, read_length);  // Read File
        
         
         Modem_PutString(buff);
         
         Flag_GetFile          = SET;
         Flag_WriteFile        = RESET;
         Data_WaitTime         = 30; 
         Flag_LoadProg         = SET;
         Flag_SaveFlash        = RESET;
         Flag_Download_Status  = RESET;
         Rev_Count = 0;
         while(Data_WaitTime > 0)
         {
           if(Flag_Second){Flag_Second = RESET;  Data_WaitTime--;}
           if(Flag_WriteFile)
           {
             Data_WaitTime = 0;
             Flag_GetFile = RESET;
            if(StrPtr = strstr(Rev_Buffer,"CONNECT"))
             {
               StrPtr += 8;
               //Coneect_FileLen = atoi(StrPtr);
               StrPtr = strstr(StrPtr,":");
             }
             else if(strstr(GPRS_Buffer,"ROR: 400"))SYSTEM_SW_RESET;
             else StrPtr = NULL;
             
//             if(Flash_Write_Size >= FLASH_PAGE_SIZE)   // ERASE SECTOR IN FLASH
//             {
//               Flash_Write_Size = 0;
//               FLASH_Erase(FlashAddress);
//             }
             
             memset(Program_Dbuff,0,sizeof(Program_Dbuff)); // CLEAR BUFFER
           NEXT:
             while ((*StrPtr != NULL) && Write_Loc < FLASH_PAGE_SIZE) 
             { 
               mSec_Delay(100);
               /*------- Start Code  with `:` ----------------------------------------------*/
               StrPtr = strstr(StrPtr,":");
               if (StrPtr != NULL) 
               {
                 for (GP_Counter = 0; GP_Counter < (IntelHexDataSize * 2) && *StrPtr != '\0'; GP_Counter++, StrPtr++) 
                 {
                   if (*StrPtr == RECORD_MARK) { StrPtr++; GP_Counter = 0; Flag_DataError = RESET; break;}
                 }
               }
               else 
                 DataErr();
               
               /*------- CHECK DATA ERROR ---------------------------------------------------*/
               if(GP_Counter == (IntelHexDataSize*2)) 
               {
                 Flag_DataError = SET;
                 DataErr();
               }
               
               /*-------- FIRST BYTE : NUMBER OF BYTES --------------------------------------*/
               if (StrPtr != NULL) No_OfBytes = CONVERTHEX(*StrPtr) *16 + CONVERTHEX(*(StrPtr+1));StrPtr += 2;	
                Calc_ChkSum = No_OfBytes;
               
               /*-------- BYTE 2+3 : CodeADDR -----------------------------------------------*/
              if (StrPtr != NULL) FlashCodeAddr = (CONVERTHEX(*StrPtr) * 4096) + (CONVERTHEX(*(StrPtr+1)) * 256) + (CONVERTHEX(*(StrPtr+2)) * 16) + CONVERTHEX(*(StrPtr+3));StrPtr += 4;
               Calc_ChkSum += FlashCodeAddr;
               
               Check_FOTA_CodeAddr = StartFotaAddr + FlashCodeAddr;
               
               // if((FlashCodeAddr) != (FlashAddress - Check_FOTA_CodeAddr)) {Flag_DataError = SET; DataErr();}
               
               /*------- 4th BYTE : RECORD TYPE ---------------------------------------------*/
              if (StrPtr != NULL) RecordType = CONVERTHEX(*StrPtr) *16 + CONVERTHEX(*(StrPtr+1));StrPtr +=2;	   
               Calc_ChkSum += RecordType;
               
               /*----- CHECK HEX STATRTING --------------------------------------------------*/
               if((RecordType == 0x04) && (No_OfBytes == 0x02))
               {
                if (StrPtr != NULL) Check_FOTA_CodeAddr = (CONVERTHEX(*StrPtr) * 4096) + (CONVERTHEX(*(StrPtr+1)) * 256) + (CONVERTHEX(*(StrPtr+2)) * 16) + CONVERTHEX(*(StrPtr+3));StrPtr += 4;
                 Check_FOTA_CodeAddr *= 0x10000;
                 
                 //if(Check_FOTA_CodeAddr == FLASH_FOTA_IMG_ADDR )
                   Flag_FOTA_FILE_Addr = SET;
                 
                 goto NEXT;
               } 
               
               /*----- END OF SECTOR --------------------------------------------------------*/
               if((FlashCodeAddr == 0xFFF0) &&(RecordType == 0x00)) Flag_EndofSector = SET;
               else if((FlashCodeAddr == 0xFE00) &&(RecordType == 0x00)) 
                 Flag_NextEndOfSector = SET;
               
               /*----- END OF FILE ----------------------------------------------------------*/
               if((RecordType == 0x01) && (No_OfBytes == 0x00) && (FlashCodeAddr == 0x0000))
               {
                 Flag_EndofFile = SET; break;
               }
               
               /*----- DATA BYTES -----------------------------------------------------------*/
               if((RecordType == 0x00) && No_OfBytes && !Flag_DataError && Flag_FOTA_FILE_Addr)
               {
                 memcpy(Hex_Deta, StrPtr, 2*No_OfBytes);
                 StrPtr += 2*No_OfBytes;
                 char* HexPtr = charToHex(Hex_Deta);
                 
                 for(GP_Counter = 0; GP_Counter < No_OfBytes; GP_Counter++)
                   Program_Dbuff[Write_Loc++] = HexPtr[GP_Counter];
                 
                 memset(Hex_Deta,0, sizeof(Hex_Deta));
                 free(HexPtr);  // = &test;    // Free Memory
               }
               
               /*----- CHECKSUM -----------------------------------------------------------*/
               CheckSum = CONVERTHEX(*StrPtr) *16 + CONVERTHEX(*(StrPtr+1));StrPtr += 2;
               
               /*----- CALCULATE CHECKSUM -------------------------------------------------*/
               Calc_ChkSum = 0x100 - Calc_ChkSum;
             }
             
             if ((Program_Dbuff != NULL) && (Write_Loc != NULL)) 
             {
               //HAL_FLASH_Write_Buff(FlashAddress, Program_Dbuff, Write_Loc); tbd
             }
             else 
               DataErr();
             
             Flag_HTTP_Content = RESET;
             Flag_WriteFile = RESET;
             Flash_Write_Size   += Write_Loc;   // WRITE SIZE IN FLASH
             FlashAddress       += Write_Loc;
             Check_FOTA_CodeAddr = Write_Loc;
             Data_WaitTime       = 0; 
             Write_Loc           = 0;
             FileLoc            += FOTA_PKT;            // FILE LOCATION IN MODEM
             if(FileLength > FOTA_PKT)FileLength         -= FOTA_PKT;
             else FileLength = 0;
             memset(Rev_Buffer,0,sizeof(Rev_Buffer));
           }
         }
         if(Flag_EndofFile)break;  //(FileLoc >= FileLength)&&
       }
       
       /*--------------------- Flash Lock -------------------------------------*/
        //HAL_FLASH_Lock();  tbd
       
       if (!Flag_DataError)
       {
         
 
          
          
        LL_RTC_BAK_SetRegister(RTC, LL_RTC_BKP_DR3, 'P');
        LL_RTC_BAK_SetRegister(RTC, LL_RTC_BKP_DR1, 'F');
         //FLASH_Erase(GOLDEN_OR_FOTA_IMG_ADD);
        // Send_SMS(SUCCESS); TBD
         Flag_FlashProg = RESET;
       }
       
       //CPU_ResetTime = 2;tbd

  }
}
/*******************************************************************************
* Function Name  : Program_Flash
* Description    : Prepare the flash for new Program
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
//TBD
void HAL_FLASH_Write_Buff(uint32_t FlashAddress, char *Data, uint32_t DataSize)
{
    for (int i = 0; i < DataSize; i += 8, FlashAddress += 8)
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, FlashAddress,*(uint64_t *)(Data + i));
}

/**
  * @brief  Gets the page of a given address
  * @param  Addr: Address of the FLASH Memory
  * @retval The page of a given address
  */
//TBD
//static uint32_t GetPage(uint32_t Addr)
//{
//  uint32_t page = 0;
//  page = (Addr < (FLASH_BASE + FLASH_BANK_SIZE)) ? (Addr - FLASH_BASE) / FLASH_PAGE_SIZE : (Addr - (FLASH_BASE + FLASH_BANK_SIZE)) / FLASH_PAGE_SIZE;
//  return page;
//}


/*******************************************************************************
* Function Name  : FLASH_Erase
* Description    : Flash
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
//void FLASH_Erase(uint32_t FlashEraseAddress)
//{
//  //TBD
//  HAL_FLASH_Unlock();
//
//  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
//  FirstPage = GetPage(FlashEraseAddress);
//
//  NbOfPages = GetPage(End_Address) - FirstPage;
//  BankNumber = GetBank(FlashEraseAddress);
//
//  EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
//  EraseInitStruct.Banks       = BankNumber;
//  EraseInitStruct.Page        = FirstPage;
//  EraseInitStruct.NbPages     = NbOfPages;
//
//
//  if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK) Error_Handler();
//
//  FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);
//}



/*--------------------- Flash Read ------------------------------------*/
void HAL_FLASH_READ(char *Buff, uint32_t Addr, int Buff_Lnt) 
{
    if (!Buff || Buff_Lnt <= 0) return;

    uint32_t Prog_Loc = 0;
    while (Prog_Loc < FLASH_PAGE_SIZE && Prog_Loc < (uint32_t)Buff_Lnt) 
    {
        uint32_t flash_data = *(uint32_t*)(Addr + Prog_Loc);
        if (flash_data == FLASH_EMPTY_VALUE) break;

        int copy_size = (Prog_Loc + 4 <= (uint32_t)Buff_Lnt) ? 4 : Buff_Lnt - Prog_Loc;
        memcpy(Buff + Prog_Loc, &flash_data, copy_size);
        Prog_Loc += 4;
    }
}

/**
  * @brief  Gets the bank of a given address
  * @param  Addr: Address of the FLASH Memory
  * @retval The bank of a given address
  */

//TBD
//static uint32_t GetBank(uint32_t Addr)
//{
//  return FLASH_BANK_1;
//}


/*******************************************************************************
* Function Name  : DataErr
* Description    : Data Error
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DataErr(void)
{
  //Send_SMS(MISSINGDATA);tbd
  
  LL_RTC_BAK_SetRegister(RTC,LL_RTC_BKP_DR3,'E');
  //CPU_ResetTime = 5;
  SYSTEM_SW_RESET;
}

/*******************************************************************************
* Function Name  : FcharToHex
* Description    : Fchar To Hex
* Input          : char data
* Output         : None
* Return         : None
*******************************************************************************/
char* charToHex(char* input) 
{
    size_t length = strlen(input);
    char* output = (char*)malloc(length/2 + 1);

    for (size_t i = 0; i < length; i += 2)
    {
      char hexPair[3] = {input[i], input[i + 1], '\0'};
      sscanf(hexPair, "%02hhx", &output[i / 2]);
    }
    output[length / 2] = '\0';
    return output;
}



/*******************************************************************************
* Function Name : JumpToAppAddr
* Description   : Jump Application Address
* Input         : AppStart_Address
* Output        : None
* Return        : None
*******************************************************************************/
//void JumpToAppAddr(uint32_t AppStart_Address)
//{
//  pFunction Jump_To_Application;
//  
//  uint32_t Jump_Addr  = *((uint32_t *)(AppStart_Address + 4U));
//  Jump_To_Application = (pFunction)Jump_Addr;
//  uint32_t msp_value  = *((uint32_t *)AppStart_Address);
//    
//  __enable_irq();
//  
//  /* Disable Systick timer */
//  SysTick->CTRL = 0;
//  
//  /* Set the clock to the default state */
//  HAL_RCC_DeInit();  //tbd
// 
//  __set_MSP(msp_value);
//  
//  Jump_To_Application();
//}

/******************* (C) COPYRIGHT 2023 EDS INDIA ********* END OF FILE *********/