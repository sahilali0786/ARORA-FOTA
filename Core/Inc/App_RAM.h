
#ifndef __App_RAM_H
#define __App_RAM_H

typedef  void (*pFunction)(void);

extern pFunction Jump_To_Application;

extern FlagStatus
Flag_SerialFlash,
Flag_FOTA_DONE,
Flag_START,
Flag_Init_GPRS,
Flag_FileSize,                                                                  // INDICATE FILE SIZE IS ABOUT TO RECEIVE                                                   
//Flag_LoadProg,                                                                  // LOAD PROGRAM TO FLASH MEMORY OF MICROCONTROLLER 
//Flag_FlashProg,                                                                 // DOWNLOAD VTS PROGRAM FROM FTP SERVER 
//Flag_SaveFlash,                                                                 // SAVE VTS PROGRAM IN SPI FLASH MEMORY
Flag_Download,                                                                  // DOWNLOAD VTS PROGRAM IN MODEM
Flag_MODULE_EC200_RST,
Flag_NoResp,                                                                    // MODEM NO RESPONSE FLAG
Flag_ModemRdy,                                                                  // INIT MODEM BOOT SUCCESS
Flag_SPIFail,                                                                   // SPI FAIL FLAG
Flag_mSec,                                                                      // Flag Set Every Milli Second
Flag_Second,                                                                    // FLAG SET EVERY SECOND
Flag_Error,
Flag_Modem_process_ongoing;



extern char
Ser_Buffer[50],
Curr_SWVer[10],
SW_VER[10];                                                                      // SOFTWARE VERSION INPUT 

extern unsigned char
Get_CSQ,
Save_Record_Try,
Program_Byte[17],                                                               // PROGRAM BYTE TO SAVE A 2 BYTES INTO FLASH
VTS_State;                                                                      // VTS OPERATING STATE

extern unsigned short
ErrorCode,
Check_Systick,
//Write_Loc,                                                                      // WRITE LOCATION OF FLASH DATA FROM RECEIVING BUFFER 
Rev_Count,                                                                      // COUNTER OF RECEIVING BYTES @ GPRS USART INTERRUPT
//Loc,                                                                            // SAVE LAST LOCATION OF RECEIVE COUNT AT THE END OF RECEIVING FTP FILE 
Sec_Tick,                                                                       // TICK @ 1 MILI SECOND 
GP_Counter;                                                                     // GENERAL PURPOSE COUNTER 

extern long
//StartAddress,                                                                   // START ADDRESS FLASH DATA
SPIFlashAddress;                                                                // SPI FLASH ADDRESS
//FlashAddress;                                                                   // FLASH PAGE ADDRESS
extern uint16_t Config_Counter;
extern uint32_t JumpAddress;
extern uint8_t respond_time,Config_State,EC200_ONTime,GPRS_ModemErr ;
#endif