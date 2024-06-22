
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GPRS_H
#define __GPRS_H

 

#define MODEM_INIT              0                                               // MODEM INITIALIZATION
#define NORMAL                  1                                               // VTS NORMAL RUNNING MODE
#define VALID_SIGNAL    	8

#define TAISYS                                                                  // SENSORISE / IOTVT / TAISYS
#define TAISYS_AB                                                               // VODA_BSNL / AIRTEL_BSNL / TAISYS_AB / TAISYS_VB

#define INT_BATT_END            3.50                                            // INTERNAL BATTERY END VOLTAGE   


#define GPRSBuffer_Size     1024

#define FAIL                 0
#define GOLDEN_SWITCH_FAILED 1
#define FOTA_SWITCH_FAILED   2
#define UNABLE_TO_CONNECT    3
#define UNABLE_TO_DWLOAD     4
#define WRONG_URL            5
#define WRONG_FILE           6
#define SPI_FAIL             7
#define SPI_OK               8
#define ERROR                9
#define MISSINGDATA          10
#define SUCCESS              11
#define RECORD_FAIL          12
#define HTTP_REQUEST_SUCCESS 13
#define HTTP_RESPONCE_DATA   14
#define ERRORCODE            15 // ftp error code

//to be use during check signal nw status
#define Get_Signal_Strength		0
#define Get_SIM_Registration 		1

extern FlagStatus
Flag_GetFile,                                                                   // DOWNLOADING STARTING FROM FTP SERVER
Flag_WriteFile,                                                                 // SAVE RECEIVED DATA INTO FLASH
Flag_ModemOK,                                                                   // GPRS DATA READY           
Flag_ModemError,                                                                // ERROR GENERATED 
Flag_ValidNetwork,                                                              // VALID NETWORK
Flag_WrongIMEI,                                                                 // WRONG IMEI NUMBER                  
Flag_WrongFile,                                                                 // WRONG FILE NAME REQUESTED TO DOWNLOAD 
Flag_APNSet;                                                                    // APN NAME SET

extern char 
 IAP_Status,                                                                     // IAP STATUS 
IAP_Counter,                                                                    // IAP COUNTER 
NoData_Count,                                                                   // NO DATA  AFTER CONNECTION COUNTER        
Storage_Data[4096],                                                             // PROGRAMMING DATA
GPRS_SendBuffer[100],                                                           // GENERAL PURPOSE SENDING BUFFER
GPRS_Buffer[GPRSBuffer_Size],                                                   // GPRS DATA BUFFER 
DFU_Mob[15];                                                                    // DFU REQUESTING MOBILE NUMBER 

extern unsigned char
Reg_Error,                                                                      // SIM REGISTRATION ERROR                 
Modem_NoResp,                                                                   // MODEM NO RESPONSE COUNT
Init_Time;                                                                      // MODEM INITIALIZATION TIME

extern unsigned int
GPRS_Count;                                                                     // GPRS DATA COUNTER                   
//UpLoad_Time;                                                                    // GPRS DATA UPLOAD TIME

extern unsigned long 
File_Size;                                                                      // RECEIVED FILE SIZE 
typedef struct									// DATA STRUCTURE FOR GPRS 
{
  char Modem_FwVer[30];						                // FIRMWARE VERSION
  char IMEI_No[16];                                                             // IMEI NUMBER        
  char SIM_Present[2];                                                          // SIM PRESENT 
  char ICCID[30];                                                               // ICCID
  char Operator[11];								// OPERATOR 
  char APN_Name[33];                                                            // ACCESS POINT NAME 
  char SET_APN[33];                                                             // SET APN BY USER 
  unsigned char Signal_Strength;						// SIGNAL STRENGTH 
  char Network_Status;							        // NETWORK STATUS 
  FlagStatus Home_Network;							// HOME NETWORK 
  FlagStatus Roaming_Network;							// ROAMING NETWORK 
  char Network_Type[10];
}GPRS_InitTypeDef;

extern GPRS_InitTypeDef    GPRSData;


//void MQTT_Managment(void);
void WAIT_MODEM_CONNECT(void);
void WAIT_MODEM_RESP(uint8_t Wait_Time);
void Serial_PutString_GPRS(char *s);
void Serial_PutData_GPRS(char *s, char Place);
void Modem_PutString(char *s);
void Serial_SendData_GPRS(char SerialData);
void MODULE_EC200_RST(void);
void GET_ServingCell_INFO(void);
void GET_NRB_ServingCell_INFO(void);
void GET_GPRS_CLK(void);
void Get_APN_Routine(void);
void Check_Signal_Nw_GPRS(void);
void Set_APN(void);
void Check_Network_Validation(void);
void Extract_Cell_ID(void);
void SIM_Registration_INIT(void);
void Check_Signal_Strength(void);
void INIT_GPRS_Modem(void);
 
void Get_CellLocation(void);
void GPRS_Management(void);
void Swap_Service_Operator(void);
void Amswer_Call(void);
void Hang_Up_Call(void);
void Dail_Number(void);
void __Check_Call_Pickup(void);
void Programe_RTC(void);

int Count_Substring(const char *str, const char *sub);


#define CHECK_NETWORK_IF                        ((GPRSData.SIM_Present[0] == '1') && Flag_ProcessTick && (!Flag_ReadSMSData) &&  (!Flag_GNSS_Ongoing) && (!Flag_TCP_Process_Ongoing) && Flag_Nw_Check)

#define SYSTEM_SW_RESET                         NVIC_SystemReset()                      // SYSTEM SOFTWARE RESET REQUEST GENERATION


#endif