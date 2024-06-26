/******************** (C) COPYRIGHT 2024 EDS INDIA ****************************
* File Name     : FOTA.h					       	                                    *
* Author        : TEcC SANTOSH KUAMR                                          *
* Date          : 10/12/2023                                                  *
* Description   : This File Defines variables & misc functions                *
* Revision		: Rev0.0.1						      	                                  *
*******************************************************************************/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __Functions_H
#define __Functions_H

#define Reveive_Buffer_Size 12300

#define RECORD_MARK            ':'
#define IntelHexDataSize       16         // HexData Size 16

#define RECLEN    0x02
#define MAXRECLEN 0xFF              // Maximumm value of Reclen field used in IntelHex

#define FOTA_PKT               8192 //11000 // FOTA Packet Download size
#define FLASH_EMPTY_VALUE      0xFFFFFFFF

#define GOLDEN_IMG_ADDR              (uint32_t)(0x08008800)
#define FOTA_IMG_ADDR                ((uint32_t)0x08014000)

#define Start_Address                   0x0801F890 // 3th Sector
#define End_Address                     0x0801FFFF//0x0801F7FF

#define DFU_IP_ADDR                     Start_Address + 8  //32
#define DFU_PATH_ADDR                   Start_Address + 32  //64
#define DFU_FILE_NAME_ADDR              Start_Address + 88 //32
#define DFU_MOB_ADDR                    Start_Address + 112 //32

//#define DFU_IP_ADDR                     Start_Address + 8  //24
//#define DFU_PATH_ADDR                   Start_Address + 32  //56
//#define DFU_FILE_NAME_ADDR              Start_Address + 88 //24
//#define SMS_MOB_NO_ADDR                 Start_Address + 112 //16
//#define SERVER_MAIN_IP_ADDR             Start_Address + 128 //24
//#define SERVER_PORT_ADDR                Start_Address + 152 //8
//#define PROTOCOL_ADDR                   Start_Address + 160//96

#define APP_TYPE_ADDR                  ((uint32_t)0x0801f810) //Start_Address + 180
extern int read_length;
extern uint32_t AppStart_Address;
extern uint32_t Jump_Addr;
extern uint16_t GP_Counter;

extern FlagStatus 
Flag_Fota_Start,
      Flag_Golden_Try,
      Flag_TryCnt,
      Flag_WrongFile,
      Flag_FlashProg,
      Flag_FOTA_Try,
      Flag_DFUConnect_Retry,
      Flag_Download_Status,
      //Flag_Download,
      Flag_DFU,
      Flag_GetFile,
      Flag_FileConnect,
      Flag_WriteFile,
      Flag_HTTP_Content,
      Flag_EndofSector,
      Flag_NextEndOfSector,
      Flag_Int_FlashError,
      Flag_SaveFlash,
      Flag_LoadProg,
      Flag_DataError,
      Flag_FWFail;

typedef enum 
{
    DATA                     = 0x00,
    END_OF_FILE              = 0x01,
    EXTENDED_SEGMENT_ADDRESS = 0x02,
    START_SEGMENT_ADDRESS    = 0x03,
    EXTENDED_LINEAR_ADDRESS  = 0x04,
    START_LINEAR_ADDRESS     = 0x05
}IntelHex;

typedef struct                                                                  // DATA STRUCTURE FOR DFU 
{
  char IP[20];                                                                   // DFU/HTTP IP ADDRESS 
  char PATH[50];                                                                // DFU/HTTP Path
  char FILENAME[20];                                                            // DFU/HTTP File Name
  char URL[100];                                                                // DFU/HTTP URL
  char DFU_Mob[16];
  char GOLDEN[10];  
}HTTP_InitTypeDef;

extern HTTP_InitTypeDef       DFU_HTTP;
extern uint16_t Coneect_FileLen;
extern char Rev_Buffer[Reveive_Buffer_Size];
extern uint32_t FileLength;
extern FlagStatus
Flag_DFUConnect_Retry;


extern uint32_t FileLoc;
extern uint32_t FileHandel;
                                                           // DFU REQUESTING MOBILE NUMBER 
extern uint8_t Data_WaitTime;
void DataErr(void);
void mSec_Delay(unsigned int);
void Get_Firmware(void);
void HAL_FLASH_Write_Buff(uint32_t FlashAddress, char *Data, uint32_t DataSize);
void HAL_FLASH_READ(char *Buff, uint32_t Addr, int Buff_Lnt);
void JumpToAppAddr(uint32_t AppStart_Address);
void StoreSPI_Flash(void);
void Program_Flash(void);
 
#endif
/******************* (C) COPYRIGHT 2024 EDS INDIA ********* END OF FILE ************/