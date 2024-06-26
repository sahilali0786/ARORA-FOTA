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
//static FLASH_EraseInitTypeDef EraseInitStruct; 
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
  char* PTR;
   char *Ptr;
  /*----- CHECK SIGNALS & NETWORK --------------------------------------------*/
  Check_Signal_Nw_GPRS();                                                       // CHECK SIGNAL STRENGTH NETWORK REGISTRATION GPRS ATTACHMENT 
  Get_CSQ++;
  if(Get_CSQ > 99)SYSTEM_SW_RESET;
  if(Flag_ValidNetwork == SET && Flag_APNSet == RESET){Get_CSQ = 0; Set_APN();}               // IF NETWORK VALID SET APN
  
  if(Flag_APNSet)
  {
      char buff[50];
      Serial_PutString_GPRS("AT+QHTTPURL=");                                    // AT+OHTTPURL - set URL address
      memset(buff,0,100);
      if(DFU_HTTP.URL[0] != 'h')
        sprintf(DFU_HTTP.URL,"http://%s/%s/%s.bin",DFU_HTTP.IP ,&DFU_HTTP.PATH[0],&DFU_HTTP.FILENAME[0]);
    
      sprintf(buff,"%d,20",strlen(DFU_HTTP.URL));
      Serial_PutString_GPRS(buff);                                              // URL address data length
      Serial_SendData_GPRS(13); Serial_SendData_GPRS(10);                                                 // Address upload time
      WAIT_MODEM_CONNECT();        
      
      
      // PUT URL & Wait for connect
      Modem_PutString(DFU_HTTP.URL);
      WAIT_MODEM_RESP(2);     
      
      
      // Get Request
      /*        3827 :-   wait http response timeout*/
      Modem_PutString("AT+QHTTPGET=80");

     Wait = 125 ;
      while(Wait)
      {
        mSec_Delay(100);
        if(Flag_Second){/*IWDG_ReloadCounter();*/ Flag_Second = RESET; Wait--;}
        if(Wait == 1){ Flag_APNSet = RESET;} // try again         
        
        if(strstr(GPRS_Buffer,"OK"))
        {
          Wait = 0;
        } 
        
          else if(strstr(GPRS_Buffer,"ROR: ")) 
        {
         SYSTEM_SW_RESET; 
        }
      
      }

      
      // Read in File
      Modem_PutString("AT+QHTTPDL=\"FOTA.txt\",1024");WAIT_MODEM_RESP(2);
      Wait = 80;
      while(Wait)
      {
             mSec_Delay(100);
         if(Flag_Second){Flag_Second = RESET; /*LL_IWDG_ReloadCounter(IWDG);*/Wait--;}
         if(Wait == 1){ Flag_APNSet = RESET;  } // try again
         
         /*extracting file length*/
         if(Ptr = strstr(GPRS_Buffer,"PDL:"))
          {
            //Flag_Download_Status = SET;
            Ptr += 5;
            Ptr = strtok(Ptr, ",");
            Ptr = strtok(NULL, ",");
            FileLength = atoi(Ptr);
            if(FileLength){Wait = 0;}
          }
         /*error*/
         else if(strstr(GPRS_Buffer,"ROR:"))
           SYSTEM_SW_RESET;
      }
      
       // Open File
      FileLength = 9313;  // for testing TBD
      int FileHandel = 0;Wait = 80;
      Modem_PutString("AT+QFOPEN=\"FOTA.txt\",2");WAIT_MODEM_RESP(2); // Open in Readonly mode
      while(Wait)
      {
        mSec_Delay(100);
        if(Flag_Second){Flag_Second = RESET;/*IWDG_ReloadCounter();*/ Wait--;}
        if(Wait == 1){ Flag_APNSet = RESET; } // try again
        if(Ptr = strstr(GPRS_Buffer,"OPEN: "))
        {
          Ptr += 6;
          Wait = 0;
          FileHandel = atoi(Ptr);
        }
      }
      
      // Start Download in SPI
      FlashAddress = SPIFLASH_DATA;Write_Loc = 0;
      Flag_ModemError = RESET;Modem_NoResp = 0;
      Flag_Download = SET;
      while(FileLength)
      {
        sprintf(buff,"AT+QFSEEK=%d,%d,0",FileHandel,FileLoc);
        Modem_PutString(buff);WAIT_MODEM_RESP(2); // Open in Readonly mode
        
        if(FileLength > FOTA_PKT)
        {
          sprintf(buff,"AT+QFREAD=%d,%d",FileHandel, FOTA_PKT);
          Modem_PutString(buff);
        }
        else
        {
          sprintf(buff,"AT+QFREAD=%d,%d",FileHandel,FileLength);
          Modem_PutString(buff);
        } 
        
        /*Flag_GetFile_Serial = RESET;*/ //TBD
        Flag_WriteFile = RESET;
        
        Data_WaitTime = 30; 
        Flag_GetFile = SET;
        while(Data_WaitTime > 0)
        {
          if(Flag_Second){Flag_Second = RESET; Data_WaitTime--;}
          if(Flag_WriteFile || strstr(Rev_Buffer,":00000001FF"))
          {
            
            mSec_Delay(50);
            PTR = strstr(&Rev_Buffer[0],"CONNECT");
            PTR = strstr(PTR,"\r\n") + 2;
            
            Flag_WriteFile = RESET;
            
            SPI_FLASH_SectorErase(FlashAddress);
            SPI_FLASH_SectorErase(FlashAddress + 4096);
            SPI_FLASH_BufferWrite(PTR,FlashAddress,8192);
            Rev_Count = 0;
            if(FileLength < 8192 && strstr(Rev_Buffer,":00000001FF"))
            {
              mSec_Delay(50);
              StoreSPI_Flash();
              if(Flag_LoadProg)Program_Flash();
              Flag_GetFile = RESET;
              return;
            }
            else FileLength -= 8192;
            Data_WaitTime = 0;
            FileLoc += 8192;  FlashAddress += 8192; 
            memset(Rev_Buffer,0,8192);
          }
        }
       
      }
      
      
    }// APN SET
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



/*******************************************************************************
* Function Name  : StoreSPI_Flash
* Description    : Store download data in SPI flash
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void StoreSPI_Flash(void)
{
  Flag_Download = RESET;
  Flag_LoadProg = RESET;
  Flag_SaveFlash = SET;
  uint16_t
  Current_Add = 0, Last_Add = 0;
  unsigned char Calc_ChkSum = 0;
  unsigned char No_OfBytes = 0,Record,CheckSum = 0;
  char Byt_Count[4];
  FlagStatus
  Flag_DataError = RESET,
  Flag_Prog = SET;
  char *StrPtr;
  StrPtr = NULL;
 
  NOLs = 0;
 
  SPIFlashAddress = SPIFLASH_DATA;
  FlashAddress  = SPIFLASH_FOTA;
  
  for(GP_Counter = 0; GP_Counter < 64 ; GP_Counter++,FlashAddress +=4096){SPI_FLASH_SectorErase(FlashAddress); mSec_Delay(10);} // 256 KB
 
  
  
  Current_Add = 0x0000;
  Last_Add    = 0xFFFF;
NEXT:  
  while(Flag_Prog)
  {
  memset(GPRS_SendBuffer,0,100);
  SPI_FLASH_BufferRead(Byt_Count,SPIFlashAddress,4);
  if(StrPtr = strstr(Byt_Count,":"))StrPtr +=1;
  No_OfBytes = CONVERTHEX(*StrPtr) *16 + CONVERTHEX(*(StrPtr+1));
  SPI_FLASH_BufferRead(GPRS_SendBuffer,SPIFlashAddress,(13+ 2*No_OfBytes));
  StrPtr = GPRS_SendBuffer;
 
    
   for(GP_Counter = 0; GP_Counter < 30; GP_Counter++,StrPtr++)
     if(*StrPtr == ':'){StrPtr += 1; break;}
   
   if(GP_Counter == 30){Flag_DataError = SET;};    
       
   if(StrPtr)
   {
   /*----- NUMBER OF BYTES ---------------------------------------------------*/
   No_OfBytes = CONVERTHEX(*StrPtr) *16 + CONVERTHEX(*(StrPtr+1));StrPtr += 2;
   Calc_ChkSum = No_OfBytes;
  
   /*----- FLASH ADDRESS CHECKSUM --------------------------------------------*/
   FlashAddress = CONVERTHEX(*StrPtr) *16 + CONVERTHEX(*(StrPtr+1));StrPtr += 2;
   Calc_ChkSum += FlashAddress;
   FlashAddress = CONVERTHEX(*StrPtr) *16 + CONVERTHEX(*(StrPtr+1));StrPtr += 2;
   Calc_ChkSum += FlashAddress;StrPtr -= 4;
 
   
   
   /*----- FLASH ADDRESS FOR WRITTING ----------------------------------------*/   
   Current_Add = (CONVERTHEX(*StrPtr) * 4096) + (CONVERTHEX(*(StrPtr+1)) * 256) + (CONVERTHEX(*(StrPtr+2)) * 16) + CONVERTHEX(*(StrPtr+3));
   //FlashAddress = SPIFLASH_FOTA + StartAddress + (CONVERTHEX(*StrPtr) * 4096) + (CONVERTHEX(*(StrPtr+1)) * 256) + (CONVERTHEX(*(StrPtr+2)) * 16) + CONVERTHEX(*(StrPtr+3));
   FlashAddress = SPIFLASH_FOTA + StartAddress + Current_Add;
   //FlashAddress -= 0x5000;                                                      // application start address in program is 0x4C00;
   FlashAddress  -= (ApplicationAddress - 0x08000000);
   StrPtr+=4;
    
   /*----- RECORD TYPE -------------------------------------------------------*/
   Record = CONVERTHEX(*StrPtr) *16 + CONVERTHEX(*(StrPtr+1));StrPtr +=2;
   Calc_ChkSum += Record;
  
   if(Record == 0x04)
     {
      FlashAddress = CONVERTHEX(*StrPtr) * 16 + CONVERTHEX(*(StrPtr+1));
      StartAddress =  FlashAddress * 0x1000;StrPtr += 2;
      FlashAddress = CONVERTHEX(*StrPtr) * 16 + CONVERTHEX(*(StrPtr+1));
      FlashAddress *= 0x10;
      StartAddress +=  FlashAddress;
      StartAddress *= 0x1000;
      StartAddress -= 0x08000000;
      
      SPIFlashAddress += (13 + (2*No_OfBytes));
      goto NEXT;
      };
   
   /*----- DATA BYTES --------------------------------------------------------*/
   for(GP_Counter = 0; GP_Counter < No_OfBytes;GP_Counter++)
    {
     Program_Byte[GP_Counter] = CONVERTHEX(*StrPtr) *16 + CONVERTHEX(*(StrPtr+1)) ; 
     StrPtr += 2;
     Calc_ChkSum += Program_Byte[GP_Counter];
    }
   
   /*----- CHECKSUM ----------------------------------------------------------*/
   CheckSum = CONVERTHEX(*StrPtr) *16 + CONVERTHEX(*(StrPtr+1));StrPtr +=2;
   
   /*----- CALCULATE CHECKSUM ------------------------------------------------*/
   Calc_ChkSum = 0x100 - Calc_ChkSum;
   if(Record == 0x00)                                                           // DATA VALUE
    {
       if(Calc_ChkSum == CheckSum)
       { SPI_FLASH_BufferWrite((char*)Program_Byte,FlashAddress,No_OfBytes); NOLs++;}
       else {Flag_Prog = RESET;Flag_DataError = SET;}
      };
     
     
     // CHECK ADDRESS DIFFERENCE == 16 BYTE
     if(Record == 0x00)
     {
     if(Last_Add == 0xFFFF)
     {
       if(Current_Add == (ApplicationAddress - 0x08000000))
       {
         Last_Add = Current_Add;
       }
       else 
       {
         DataErr();
       }
     }
     else 
     {
       if(Last_Add == 0xFFF0)
       {
         if(Current_Add != 0)
         {
           DataErr();
         }
         else 
         {
           Last_Add = Current_Add;
         }
       }
       else 
       {
         if((Current_Add - Last_Add) == 0x10)
         {
           Last_Add = Current_Add;
         }
         else 
         {
           DataErr();           
           
         }
       }
     }
   }
   
  }; // END OF StrPtr
   
   if(*StrPtr == 0x0D)
    {
     if(No_OfBytes == 0 && CheckSum == 0xFF && Record == 0x01)
     {
       Flag_Prog = RESET;Flag_SaveFlash = RESET;Flag_LoadProg = SET;

       GP_Counter = IAP_Counter << 8 | 'D'; 
        LL_RTC_BAK_SetRegister(RTC,LL_RTC_BKP_DR4,GP_Counter); 
       
     } 
     // VTS PROGRAM DATA HAVE BEEN EXTRACTED 
     else {Flag_Prog = SET;SPIFlashAddress += (13 + (2*No_OfBytes));}
    }
   else {Flag_Prog = RESET;Flag_DataError = SET;}
   
   if(Flag_Second == SET){ Flag_Second = RESET; }
  }
     
 mSec_Delay(50);
 if(Flag_DataError)
 {
   if(IAP_Status != 'S')
   {
     Send_SMS(ERROR);GP_Counter = IAP_Counter << 8 | 'E'; LL_RTC_BAK_SetRegister(RTC,LL_RTC_BKP_DR4,GP_Counter); 
   }
   else 
   { 
     GP_Counter = 255 << 8 | 'E';
      LL_RTC_BAK_SetRegister(RTC,LL_RTC_BKP_DR4,GP_Counter); 
   }
   SYSTEM_SW_RESET;
 }
}


/*******************************************************************************
* Function Name  : Program_Flash
* Description    : Prepare the flash for new Program
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Program_Flash(void)
{
  Flag_Download = RESET;
  Flag_SaveFlash = RESET;
  Flag_LoadProg = SET;
  
//  volatile status FLASHStatus;
  unsigned short FlashData;
  short Prog_Loc;
  char Prog_Data[2048];
  
  /* Unlock the Flash Program Erase controller */
  HAL_FLASH_Unlock();								// UNLOCK FLASH

  /* Erase the FLASH pages */
  for(FlashAddress = ApplicationAddress;FlashAddress <(EndAddr-(FLASH_PAGE_SIZE *2));FlashAddress += FLASH_PAGE_SIZE)
  {
    if(Flag_Second == SET){Flag_Second = RESET;  /*IWDG_ReloadCounter();*/}
     FLASH_PageErase(FlashAddress);				// ERASE FLASH PAGE
  }
  WrnData = 0;
  FlashAddress = ApplicationAddress;
  SPIFlashAddress = SPIFLASH_FOTA;
  
  for(GP_Counter = 0;GP_Counter < (EndAddr-ApplicationAddress-(FLASH_PAGE_SIZE*2))/FLASH_PAGE_SIZE; GP_Counter++)                            // UPTO CONFIGURATION LOCATION 
   {
      SPI_FLASH_BufferRead((char*)Prog_Data,SPIFlashAddress,FLASH_PAGE_SIZE);
      for(Prog_Loc = 0; Prog_Loc < FLASH_PAGE_SIZE; Prog_Loc++)
       {
        FlashData =   (Prog_Data[Prog_Loc+1]<<8) | Prog_Data[Prog_Loc];		// GET RAM DATA           
        FLASH_Program_HalfWord(FlashAddress, FlashData);
        FlashAddress += 2;Prog_Loc ++; 
        WrnData++;
        if(WrnData >= (NOLs*8))break;
       };
   
      SPIFlashAddress += FLASH_PAGE_SIZE;
     /* IWDG_ReloadCounter();*/
    
    if(WrnData >= (NOLs*8))break;
   };

  HAL_FLASH_Lock();
   
  IAP_Status = 'P';
  LL_RTC_BAK_SetRegister(RTC,LL_RTC_BKP_DR4,IAP_Status);
  mSec_Delay(1000);
  
  
  Save_Record_Try = 0;
//  if(Flag_SerialFlash == SET)
//  {
//    Serial_PutString_232("SUCCEED OK",3);
//  }
//  
  
  HAL_FLASH_Unlock();
  HAL_FLASH_OB_Unlock();
  HAL_FLASHEx_OBErase();
  FLASH_OB_RDP_LevelConfig(OB_RDP_LEVEL_1);
  /* Enable the pages write protection */
  FLASH_OB_EnableWRP(OB_WRP1_WRP1|OB_WRP2_WRP2|OB_WRP3_WRP3);
  HAL_FLASH_OB_Lock();
  HAL_FLASH_Lock(); 

  HAL_FLASH_OB_Launch();
  
 
  SYSTEM_SW_RESET;
 
}

