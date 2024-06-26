/******************** (C) COPYRIGHT 2023 EDS_INDIA  ****************************
* @Name          : GPRS.c					       	       
* @Author        : GOVINDA                                                  
* @Date          : 05/12/2023                                                  
* @Description   : This file provides all fn used for GPRS configuration and working.                  
* @Revision	 : 0.0							       
*******************************************************************************/
#include "main.h" 

GPRS_InitTypeDef        GPRSData;                                               // DATA STRUCTURE FOR GPRS
//HTTP_InitTypeDef        DFU_HTTP;                                               // DATA STRUCTURE FOR DFU HTTP

FlagStatus
//Flag_GetFile,                                                                   // DOWNLOADING STARTING FROM FTP SERVER
//Flag_WriteFile,                                                                 // SAVE RECEIVED DATA INTO FLASH
Flag_ModemOK,                                                                   // GPRS DATA READY           
Flag_ModemError,                                                                // ERROR GENERATED 
Flag_ValidNetwork,                                                              // VALID NETWORK
Flag_WrongIMEI,                                                                 // WRONG IMEI NUMBER                  
//Flag_WrongFile,                                                                 // WRONG FILE NAME REQUESTED TO DOWNLOAD 
Flag_APNSet;                                                                    // APN NAME SET

char 
IAP_Status,                                                                     // IAP STATUS 
IAP_Counter,                                                                    // IAP COUNTER 
NoData_Count,                                                                   // NO DATA  AFTER CONNECTION COUNTER        
Storage_Data[4096],                                                             // PROGRAMMING DATA
GPRS_SendBuffer[100],                                                           // GENERAL PURPOSE SENDING BUFFER
GPRS_Buffer[GPRSBuffer_Size],                                                   // GPRS DATA BUFFER 
DFU_Mob[15];                                                                    // DFU REQUESTING MOBILE NUMBER

unsigned char
Reg_Error,                                                                      // SIM REGISTRATION ERROR                 
Modem_NoResp,                                                                   // MODEM NO RESPONSE COUNT
Init_Time = 25;                                                                      // MODEM INITIALIZATION TIME

unsigned int GPRS_Count;                                                                     // GPRS DATA COUNTER                   
//UpLoad_Time;                                                                    // GPRS DATA UPLOAD TIMEe

unsigned long 
File_Size;    // RECEIVED FILE SIZE 

//
///*******************************************************************************
//* Function Name : GPRS_Management
//* Description   : GPRS_Management functions
//* Input         : None
//* Output        : Mone
//* Return        : None
//*******************************************************************************/

//void GPRS_Management(void)
//{
//  /* GPRS configuration routine to be done only if required */
//  if(Flag_Init_GPRS){INIT_GPRS_Modem();}
//  
//  /* After initialization if N/w strenght > MIN then register to the N/w */
//  else if(CHECK_NETWORK_IF)
//  {
//         if(!Flag_ValidNetwork)Check_Signal_Nw_GPRS();
//    else if(!Flag_APNSet)      Set_APN();                                       /* Srt the apn of the network connected with */
// 
//  }
//}
// 





/*******************************************************************************
* Function Name : INIT_GPRS_MODEM
* Description   : Initialize GPRS Modem
* Input         : string
* Output        : None
* Return        : None
*******************************************************************************/
void INIT_GPRS_Modem(void)
{
  switch(Config_State)
  {
  case 0:
    {
      if(!Config_Counter){ Modem_PutString("AT+QSTK=1"); Config_Counter = 2000;}
      else
      {
        Wait_For_Resp(); 
        if(Flag_ModemOK)Config_State = 1;
      }
      break;
    }
  case 1:
    {
      if(!Config_Counter){ Modem_PutString("AT+IFC=0,0"); Config_Counter = 2000;}
      else
      {
        Wait_For_Resp();
        
        if(Flag_ModemOK)Config_State = 2;
      }
      break;
    }  
  case 2:
    {
      if(!Config_Counter){ Modem_PutString("AT+QSIMSTAT=1"); Config_Counter = 2000;}
      else
      {
        Wait_For_Resp();
        if(Flag_ModemOK)Config_State = 3;//4
      }
      break;
    }
  case 3:
    {
      
            if(!Config_Counter){ Modem_PutString("AT+CMGF=1"); Config_Counter = 2000;}
      else
      {
        Wait_For_Resp();
        if(Flag_ModemOK)Config_State = 4;
      }
      break;
    }
  case 4:
    {
      if(!Config_Counter){ Modem_PutString("AT+CTZU=2"); Config_Counter = 2000;}
      else
      {
        Wait_For_Resp();
        if(Flag_ModemOK)
        {
          if(strstr((char*)GPRS_Buffer,"STK: 0")){Modem_PutString("AT+QSTK=1"); Flag_MODULE_EC200_RST = SET;} //TBD
          else{GPRS_Count = 0; Config_Counter = 0; Config_State = 5;} 
        }
      }
      break;
    }
  case 5:
    {
      if(!Config_Counter){ Modem_PutString("AT+CSCS=\"IRA\""); Config_Counter = 2000;}
      else
      {
        Wait_For_Resp();
        if(Flag_ModemOK)
        {
//          char *RPtr = NULL;
//          if((RPtr = strstr((char*)GPRS_Buffer,"sion:")) != NULL)
//          {
//            RPtr += 6;
//            char* Endptr = strtok(RPtr, "OK");
//            strcpy(GPRSData.Modem_FwVer, Endptr);
//          }
          Config_State = 6;
        }
      }
      break;
    }
  case 6:
    {
      if(!Config_Counter){  Modem_PutString("AT+QVBATT=0,3451,1"); Config_Counter = 2000;}
      else
      {
        Wait_For_Resp();
        if(Flag_ModemOK)Config_State = 7;
      }
      break;
    }
  case 7:
    {
      if(!Config_Counter){ Modem_PutString("AT+QVBATT=1,3300,1"); Config_Counter = 2000;}
      else
      {
        Wait_For_Resp();
        if(Flag_ModemOK)Config_State = 8;
      }
      break;
    }
  case 8:
    {
      if(!Config_Counter){ Modem_PutString("AT+QVBATT=2,4550,1"); Config_Counter = 2000;}
      else
      {
        Wait_For_Resp();
        if(Flag_ModemOK)Config_State = 9;
      }
      break;
    }
    case 9:
    {
      if(!Config_Counter){ Modem_PutString("AT+CSMP=17,167,0,241"); Config_Counter = 2000;}
      else
      {
        Wait_For_Resp();
        if(Flag_ModemOK)Config_State = 10;
      }
      break;
      break;
    }
  case 10:
    {
      if(!Config_Counter){ Modem_PutString("AT+QVBATT=3,4730,1"); Config_Counter = 2000;}
      else
      {
        Wait_For_Resp();
        if(Flag_ModemOK)Config_State = 11;
      }
      break;
    }
  case 11:
    {
      if(!Config_Counter){ Modem_PutString("AT&W"); Config_Counter = 2000;}
      else
      {
        Wait_For_Resp();
        if(Flag_ModemOK)Config_State = 12;
      }
      break;
    }
  case 12:
    {
      if(!Config_Counter){ Modem_PutString("AT+GSN"); Config_Counter = 2000;}
      else
      {
        Wait_For_Resp();
        if(Flag_ModemOK)Config_State = 13;
          char *RPtr = NULL;
	        uint16_t GP_Counter = 0;
	  
          if((RPtr = strstr((char*)GPRS_Buffer,"GSN")) != NULL)
          {
            Flag_WrongIMEI = RESET;
            RPtr += 3;
            if(*RPtr < '0' || *RPtr > '9') RPtr += 1;                           // SET POINTER TO READ IMEI NUMBER 
            for(GP_Counter = 0;GP_Counter <= 14; GP_Counter++, RPtr++)          // EXTRACT 15 DIGIT IMEI NUMBER 
            {
              if(*RPtr < '0' || *RPtr > '9'){Flag_WrongIMEI = SET; break;}      // IF CHAR OTHER THAN NUMERIC IT MEANS WRONG IMEI NUMBER 
              GPRSData.IMEI_No[GP_Counter] = *RPtr;                             // SAVE IMEI NUMBER                               
            };
            GPRSData.IMEI_No[GP_Counter] = 0;
          }
      }
      break;
    }
  case 13:
    {
      if(!Config_Counter){ Modem_PutString("AT+CCID"); Config_Counter = 2000;}
      else
      {
        Wait_For_Resp();
        if(Flag_ModemOK)Config_State = 14;
        
        char *RPtr = NULL;
        uint16_t GP_Counter = 0;
        
        if((RPtr = strstr((char*)GPRS_Buffer,"ID: ")) != NULL)
        {
          RPtr += 4;
          GPRSData.SIM_Present[0] = '1';                                      // MODEM RESPONDING i.e. SIM IS PRESENT
          for(GP_Counter = 0;GP_Counter <=20; GP_Counter++,RPtr++)            // EXTRACT 15 DIGIT IMEI NUMBER 
            GPRSData.ICCID[GP_Counter] = *RPtr;                               // SAVE IMEI NUMBER                               
        }
        
      } 
      break;
    }
    
    
  case 14:
    {
      if(!Config_Counter){ Modem_PutString("AT+CGREG=2"); Config_Counter = 4000;}
      else
      {
        Wait_For_Resp();
       
        if(Flag_ModemOK)Config_State = 15;
      }
      break;
    }
  case 15:
    {
      if(!Config_Counter){ Modem_PutString("AT+CSMP=17,167,0,241"); Config_Counter = 2000;}
      else
      {
        Wait_For_Resp();
        if(Flag_ModemOK)
        {

          Config_State = 16;
        }
      }
      break;
    }
    
  case 16:
    {
        SMS_Init();
    Config_State = 17;
      break;
    }
    
  case 17:
    {
      if(!Config_Counter){ Modem_PutString("AT+QSPN?"); Config_Counter = 2000;}
      else
      {
        Wait_For_Resp();
        if(Flag_ModemOK)
        {
          Config_State = 18;
        }
        
        Get_APN_Routine();
        VTS_State = NORMAL;
        if(GPRSData.SIM_Present[0] == '1')Flag_ModemRdy = SET;
        Flag_Init_GPRS = RESET;
        /*Network_Status = 0;*/ 
        Config_State = 0;
        /*Flag_Nw_Check = SET;*/
        
      }
      break;
    }
    
    
  }
}
 




//
//
/*******************************************************************************
* Function Name : Wait_For_Resp
* Description   : Check for response from Modem
* Input         : Parameter name & place
* Output        : Parameter new value
* Return        : Paremeter
*******************************************************************************/
void Wait_For_Resp(void)
{
  mSec_Delay(50);
  if((strstr((char*)GPRS_Buffer,"OK")))
  {
 
    Flag_ModemOK = SET;
    Flag_Error = RESET;
    Flag_NoResp = RESET;
    Flag_Modem_process_ongoing  = RESET;
    GPRS_Count = 0;
    Config_Counter = 0;
    Modem_NoResp = 0;
    GPRS_ModemErr = 0;
  }
  else if((strstr((char*)GPRS_Buffer,"FAIL"))  ||(strstr((char*)GPRS_Buffer,"ERROR")) )
  {
    
    Flag_Error = SET;
    Flag_ModemOK = RESET;
    Flag_NoResp = RESET;
    Config_Counter = 0;
    GPRS_Count = 0;
    respond_time = 0;
    Modem_NoResp = 0;
    GPRS_ModemErr++;
    Flag_Modem_process_ongoing  = RESET;
  }
  else Flag_NoResp = SET;
}

/*******************************************************************************
* Function Name : Wait Modem Connect
* Description   : Waits for the Modem connect Response
* Input         : None
* Output        : None
* Return        : None
*******************************************************************************/
void WAIT_MODEM_CONNECT(void)
{
  unsigned char UpLoad_Time;
 
  UpLoad_Time = 75;Flag_ModemOK = RESET;                           // SET UPLOAD TIME 
  while(UpLoad_Time > 0 )
    {
      if(Flag_Second == SET)
      {
         Flag_Second = RESET;UpLoad_Time--;/*IWDG_ReloadCounter();*/
      };
      
     
      if(strstr((char*)GPRS_Buffer,"CONNECT")){Flag_ModemOK = SET; Modem_NoResp = 0; break;}
      if(strstr((char*)GPRS_Buffer,"ERROR")){Flag_ModemError = SET; Flag_ModemOK = RESET;break;} 
      
    };
  if(UpLoad_Time == 0) Flag_ModemOK = RESET;

}

/*******************************************************************************
* Function Name : Wait Modem Response
* Description   : Waits for the Modem Response
* Input         : None
* Output        : None
* Return        : None
*******************************************************************************/
void WAIT_MODEM_RESP(unsigned char UpLoad_Time)                           
{  
  char *StrPtr;
  StrPtr = NULL;
 
  Flag_ModemError = RESET;Flag_ModemOK = RESET;                                 // RESET ALL MODEM RELATED FLAGS
  while(Flag_ModemOK == RESET && UpLoad_Time > 0 && Flag_ModemError == RESET)
   {
     if(Flag_Second == SET)
        {
         Flag_Second = RESET; UpLoad_Time--;/*IWDG_ReloadCounter();*/               // DECREMENT UPLOAD TIME 
        };
     
     if(StrPtr = strstr((char*)GPRS_Buffer,"OK")){Flag_ModemOK = SET;GPRS_Count = 0;Modem_NoResp = 0;}
     if(StrPtr != NULL){*StrPtr = ' '; StrPtr++;*StrPtr = ' '; StrPtr = NULL;}; // INSERT 2 BLANKS
       
  };
  
  if(UpLoad_Time == 0){Modem_NoResp++;};                       // UPLOAD ERROR 
  mSec_Delay(50);
}
/*******************************************************************************
* Function Name : Modem_PutString_GPRS
* Description   : Send data to Modem and add 13(LF) at the end
* Input         : string
* Output        : None
* Return        : None
*******************************************************************************/
void Modem_PutString(char *s)
{
 // Flag_Modem_process_ongoing = SET;
  Flag_ModemOK = RESET;
  
  memset(GPRS_Buffer,0,sizeof(GPRS_Buffer));
  GPRS_Count = 0;
  LL_mDelay(2);
  
  while (*s != '\0')
  {
    while (!LL_USART_IsActiveFlag_TXE(USART1)){};		// WAIT FOR COMPLETE TRANSMISSION TILL FLAG IS 0
    USART1->TDR = (*s & (uint16_t)0x01FF);                                      // SEND NEXT STRING VALUE
    s++;                                                                        // INCREMENT POINTER 
    LL_mDelay(1);
  };
  LL_mDelay(2);
  
  while (!LL_USART_IsActiveFlag_TXE(USART1)){};	        
  USART1->TDR = (13 & (uint16_t)0x01FF);					// SEND DATA BYTE 13<CR>
   while (!LL_USART_IsActiveFlag_TXE(USART1)){};	                  
  USART1->TDR = (10 & (uint16_t)0x01FF);					// Send Data Byte 10<LF>
}








/*******************************************************************************
* Function Name  : Serial_PutString
* Description    : Print a string on the GPRS Modem
* Input          : A String till null character
* Output         : None
* Return         : None
*******************************************************************************/
void Serial_PutString_GPRS(char *s)                                             // THIS FUNCTION SEND STRING'S CHARACTER TILL NULL CHARACTER
{
  LL_mDelay(2);
  
  while (*s != '\0')                                                            // PUT STRING OVER USART TILL NULL CHARACTER 
  {
     while (!LL_USART_IsActiveFlag_TXE(USART1)){};                               // WAIT UNTILL LAST TRANSMISSION COMPLETES
    USART1->TDR = (*s & (uint16_t)0x01FF);                                      // WRITE IN DATA REGISTER OF GPRS FOR SENDING NEXT DATA
    s++;                                                                        // INCREMENT POINTER
  };
  
  LL_mDelay(2);                                                                 // WAIT FOR 5 MILI SECOND
}






/*******************************************************************************
* Function Name : Serial_SendData_GPRS
* Description   : Send Data to Modem
* Input         : one hex data only
* Output        : None
* Return        : None
*******************************************************************************/
void Serial_SendData_GPRS(char SerialData)					// THIS FUNCTION SEND ONLY ONE HEX VALUE AT A TIME	
{
  for(uint8_t i = 0 ; i < 30; i++)__NOP();
   while (!LL_USART_IsActiveFlag_TXE(USART1)){};                               // WAIT UNTILL LAST TRANSMISSION COMPLETES
  USART1->TDR = (SerialData & (uint16_t)0x01FF);                                   // WRITE IN DATA REGISTER OF GPRS FOR SENDING NEXT DATA
}


///*******************************************************************************
//* Function Name  : Serial_PutData
//* Description    : Print a string on the GPRS Modem till place define
//* Input          : A String up to given place
//* Output         : None
//* Return         : None
//*******************************************************************************/
void Serial_PutData_GPRS(char *s, char Place)                                   // THIS FUNCTION SEND A STRING'S CHARACTERS UPTO GIVEN PLACE 
{
  while (*s != '\0' && Place)
  {
    for(uint8_t i = 0 ; i < 50; i++)__NOP();
    Place--;                                                                    // PUT DATA TO USART UPTO NUMBER OF PLACE AS POINTER BY POINTER 
     while (!LL_USART_IsActiveFlag_TXE(USART1)){};                // WAIT UNTILL LAST TRANSMISSION COMPLETES
    USART1->TDR = (*s & (uint16_t)0x01FF);                                      // WRITE IN DATA REGISTER OF GPRS FOR SENDING NEXT DATA
    s++;                                                                        // INCREMENT POINTER
  };
  LL_mDelay(2);                                                                 // WAIT FOR 5 MILI SECOND
  Serial_SendData_GPRS(13); // <CL> - CARRIGE RETURN
  Serial_SendData_GPRS(10); // <LF> - Line Feed
}







/*******************************************************************************
* Function Name : Check_Signal_Nw
* Description   : Check Signal Strength & Network Parameters
* Input         : Parameter name & place
* Output        : Parameter new value
* Return        : Paremeter
*******************************************************************************/
void Check_Signal_Nw_GPRS(void)
{     
  char *StrPtr;
  StrPtr = NULL;
 
  Flag_ValidNetwork = RESET;                                                    // TO AUTHENTICATE CORRECT EXECUTION
  memset(GPRS_Buffer,0,50); GPRS_Count = 0;
  Modem_PutString("AT+CSQ");WAIT_MODEM_RESP(5);                                 // CHECK SIGNAL STRENGTH 
 
  /*----- GET SIGNAL STRENGTH & REQUEST NETWORK INFORMATION ------------------*/
  if(Flag_ModemOK == SET)
    {
      Flag_ModemOK = RESET;
      if(StrPtr = strstr((char*)GPRS_Buffer,"Q:"))
       {
         StrPtr += 3;GPRSData.Signal_Strength = atoi(StrPtr);        // EXTRACT SIGNAL STRENGTH
         if(GPRSData.Signal_Strength >= 35 )GPRSData.Signal_Strength = 0;       // SET SIGNAL STRENGTH TO 0 IF INVALID/GARBAGE
         if(GPRSData.Signal_Strength >= VALID_SIGNAL )                          // CHECK AUTHENTICATION OF SIGNAL STRENGTH
         {
          memset(GPRS_Buffer,0,50); GPRS_Count = 0; 
          Modem_PutString("AT+CGREG?");WAIT_MODEM_RESP(5);
         } 
       }
     
      if(StrPtr = strstr((char*)GPRS_Buffer,"Q:")){*StrPtr = ' ';StrPtr++;*StrPtr = ' ';};// INSERT BLANK 
    };
  
  /*----- GET LOCATION & GPRS AVAILABILITY -----------------------------------*/
  if(Flag_ModemOK == SET)                                                                       
     {  
      if(StrPtr = strstr((char*)GPRS_Buffer,"EG:"))
        {
          StrPtr += 6;
          if((*StrPtr == '1') || (*StrPtr == '5'))
          {memset(GPRS_Buffer,0,50); GPRS_Count = 0;Modem_PutString("AT+CGATT?");WAIT_MODEM_RESP(5);}
          else {Flag_ModemOK = RESET;Reg_Error++;};
        }
      else{Flag_ModemOK = RESET;Reg_Error++;};
    }
  
  /*----- CHECK NETWORK VALIDATION -------------------------------------------*/
   if(Flag_ModemOK == SET)                                                                        
     { 
      if(StrPtr = strstr((char*)GPRS_Buffer,"TT:"))
      {
        StrPtr += 4;
        if(*StrPtr == '1'){Flag_ValidNetwork = SET;Reg_Error = 0;}
        else Reg_Error++;
      }
     };
  
//  if(Reg_Error > 50){Reg_Error = 0;INIT_APP();};       tbd                         // RESET APPLICATION

  /*----- REDO APN SETTING IF NETWORK IS NOT VALID ---------------------------*/
  if(Flag_ValidNetwork == RESET)Flag_APNSet = RESET;
           
}

///*******************************************************************************
//* Function Name : Check_Signal_Strength
//* Description   : check signal strength and retrieve it
//* Input         : None
//* Output        : None
//* Return        : None
//*******************************************************************************/
//void Check_Signal_Strength(void)
//{
//  char *pSignl;
//  pSignl  = NULL;
//  
//  Flag_CSQErr = SET;							// SET THE CSQ ERROR AND RESET IF CSQ IS VALID
//  Flag_ModemOK = RESET;							// RESET THE MODEM OK SET IF EVERYTHING IS OK IN RESPONSE
//  if((pSignl = strstr((char*)GPRS_Buffer,"Q:")) != NULL)    		// +CSQ: 29,99
//  {
//    pSignl += 3;								// INCREMENT THE POINTER TO RIGHT POSITION
//    GPRSData.Signal_Strength = atoi(pSignl);                            	// EXTRACT SIGNAL STRENGTH
//    
//    if(GPRSData.Signal_Strength >= 35 )GPRSData.Signal_Strength = 0;    // SET SIGNAL STRENGTH TO 0 IF INVALID/GARBAGE
//    if(GPRSData.Signal_Strength >= VALID_SIGNAL )                       // CHECK AUTHENTICATION OF SIGNAL STRENGTH
//    {
//      Flag_CSQErr = RESET;						// RESET THE CSQERR FLAG
//      Flag_GetCGREG = SET;						// RESET FLAG FREE BUFFER FOR NEXT PROCESS
//      memset(GPRS_Buffer,0,sizeof(GPRS_Buffer));
//      GPRS_Count = 0;
//      Network_Status = Get_SIM_Registration;				// GO TO SIM REGISTATION STATE
//    }
//  }
//  else Flag_ModemOK = RESET;
//  
//  if((pSignl = strstr((char*)GPRS_Buffer,"Q:"))){*pSignl = ' ';pSignl++;*pSignl = ' ';};// INSERT BLANK 
//}
//
//
//



//
///*******************************************************************************
//* Function Name : SIM_Registration_INIT
//* Description   : Initialise sim registration process
//* Input         : None
//* Output        : None
//* Return        : None
//*******************************************************************************/
//void SIM_Registration_INIT(void)
//{
//  char *pSim_init;
//  pSim_init = NULL;
//  
//  if((pSim_init = strstr((char*)GPRS_Buffer,"G:")))				// +CGREG: 2,2,"CD96","66EF30C"
//  {
//    pSim_init += 5; 
//    Flag_GetCGREG = RESET;
//    if(*pSim_init != '1')
//    {
//      memset(GPRS_Buffer,0,sizeof(GPRS_Buffer));
//      GPRS_Count = 0;
//      Modem_PutString("AT+CGREG=2"); //WAIT_MODEM_RESP(5);
//      LL_mDelay(500);
//    }
//    
//    else if((pSim_init = strstr((char*)GPRS_Buffer,"G:")) != NULL)
//    {
//      pSim_init += 5;
//      GPRSData.Network_Status = *pSim_init;
//      if(GPRSData.Network_Status == '1')     {GPRSData.Home_Network = SET;GPRSData.Roaming_Network = RESET;Flag_GPRSConnected = SET; SwapCnt = 0;} // HOME NETWORK DETECTED  
//      else if(GPRSData.Network_Status == '5'){GPRSData.Roaming_Network = SET;GPRSData.Home_Network = RESET;Flag_GPRSConnected = SET; SwapCnt = 0;} // ROAMING NETWORK DETECTED
//      else if(GPRSData.Network_Status == '3'){GPRSData.Roaming_Network = RESET;GPRSData.Home_Network = RESET; if(SwapCnt > 3)Sim_Profile_SwapReq = 1; Flag_SWAP = SET;} // If registration Denied Switch Immedieately
//      else 
//      {
//        Flag_GPRSConnected = RESET;
//        Flag_ModemOK = RESET;
//        Reg_Error++; SIM_Err++;
//        if(Flag_Error){ Flag_Error = RESET; GPRS_ModemErr++;}
//        Network_Status = Get_Signal_Strength;
//      };
//    }
//  }
//}
//
//
//
//
//
//
///*******************************************************************************
//* Function Name : Extract_Cell_ID
//* Description   : Retrieve cell id from the received string
//* Input         : None
//* Output        : None
//* Return        : None
//*******************************************************************************/
//void Extract_Cell_ID(void)
//{
//  char *pCell, *pCell_id;
//  pCell = NULL;
//  pCell_id = NULL;
//  if((pCell = strstr((char*)GPRS_Buffer,"EG:")) != NULL)
//  {
//    /* check for cell id */
//    pCell += 15;    	                                                        // CHECK STARTING QUOTES
//    if(*pCell == '"')
//    {
//      pCell += 1;                                                                 // GET START VALUE 
//      pCell_id = strstr(pCell,"\"");
//      if(pCell_id)
//      {
//        *pCell_id = 0;			           				// ASSIGN NULL TO THE POINTER 
//        pCell_id = strstr(pCell,GPRSData.Cell_ID);
//        if((strstr(pCell,GPRSData.Cell_ID) == 0) || GPRSData.Cell_ID[0] == 0)
//        {
////          Flag_URC = SET;                                                       // Check NEbourging Cell
//          Flag_CellChange = SET;                                                // CELL TOWER CHENGED
//          pCell_id = &GPRSData.Cell_ID[0];
//          if(GPRSData.Cell_ID[0] == 0)sprintf(pCell_id,"N.A");		        // WRITE NA IS THE CELL ID FIELD               
//          else memcpy(pCell_id,pCell,7);                                              // EXTRACT NEW CELL TOWER ID
//        }
//      }
//    }
//  }
//}
//
//
//
//
//
///*******************************************************************************
//* Function Name : Check_Network_Validation
//* Description   : Check network validation 
//* Input         : None
//* Output        : None
//* Return        : None
//*******************************************************************************/
//void Check_Network_Validation(void)
//{
//  char *pNetwrk;     
//  pNetwrk  = NULL; 
//
//  if(GPRSData.Network_Status == '1' || GPRSData.Network_Status == '5')
//  {
//    /* reset the error counts,buffer count & empty the used buffer*/
//    GPRS_ModemErr = 0;
//    Reg_Error     = 0;
//    SIM_Err       = 0;
//    GPRS_Count    = 0;
//    memset(GPRS_Buffer,0,sizeof(GPRS_Buffer));
//    
//    /* ask list of supported states*/
//    Modem_PutString("AT+CGATT?");
//    LL_mDelay(1000);
//    //WAIT_MODEM_RESP(5);//tbd
//    
//    /*----- CHECK NETWORK VALIDATION -------------------------------------------*/
//    if((pNetwrk = strstr((char*)GPRS_Buffer,"TT:")) /*&& (Flag_ModemOK == SET)*/)
//    {
//      pNetwrk += 4;
//      if(*pNetwrk == '1'){Flag_ValidNetwork = SET; Flag_No_Server_Area = RESET; Modem_RST_Cnt = 0; Flag_APNSet = RESET; Reg_Error = 0; SIM_Err = 0;Cell_Change_Stage = 1; Network_Status = 0;}
//      else {Reg_Error++; SIM_Err++;}
//    }
//  }
//}
//


//
//
//
/*******************************************************************************
* Function Name : Set_APN
* Description   : Set Access Point Name based on service provider
* Input         : None
* Output        : None
* Return        : None
*******************************************************************************/
void Set_APN(void)
{
  char *Ptr;
  Ptr = 0;
  
  memset(GPRS_Buffer,0,50);GPRS_Count = 0;
                                                                  /* 604 :- unspecified error*/
  if(Flag_ModemOK == SET){ memset(GPRS_Buffer,0,50);GPRS_Count = 0;Modem_PutString("AT+QSPN?");WAIT_MODEM_RESP(2);}
 
  if(Ptr = strstr((char*)GPRS_Buffer,"PN:")) 
  {
     Ptr += 4;
     if(*Ptr == 0x22)
     {
       Ptr += 1;
       for(GP_Counter = 0;GP_Counter < 10; GP_Counter ++,Ptr++)
       {
          if(*Ptr == 0x22){GPRSData.Operator[GP_Counter] = 0; break;}
          else 
            {
             GPRSData.Operator[GP_Counter] = *Ptr;
             if(islower(GPRSData.Operator[GP_Counter]))GPRSData.Operator[GP_Counter] = toupper(GPRSData.Operator[GP_Counter]);
            };
        };
    };     
    
   memset(GPRS_Buffer,0,50);GPRS_Count = 0;
   char APN[100] = {'\0'};
   
   // GET APN Name from Operator
   if((strstr((char*)GPRSData.SET_APN,"no-apn")) || (strstr((char*)GPRSData.SET_APN,"NO-APN")))  
   {
    if(strstr((char*)GPRSData.Operator,"AIRTEL"))sprintf(APN,"taisysnet");
    else if(strstr((char*)GPRSData.Operator,"BSNL"))sprintf(APN,"bsnlnet");
    else if(strstr((char*)GPRSData.Operator,"VODAFONE"))sprintf(APN,"m2misafe");
   }
   else  sprintf(APN,"%s",GPRSData.SET_APN);
   
   // Request APN Name to check
   Modem_PutString("AT+CGDCONT?");WAIT_MODEM_RESP(2);
   if(Ptr = strstr((char*)GPRS_Buffer,"CONT: "))
   {
      if(Ptr = strstr((char*)GPRS_Buffer,APN))
      {
        Flag_APNSet = SET;
        memset(GPRSData.APN_Name,0,19);
        for(GP_Counter = 0;GP_Counter < 19; GP_Counter ++,Ptr++)
        {
          if (*Ptr == 0x22){GPRSData.APN_Name[GP_Counter] = 0; break;}
          else GPRSData.APN_Name[GP_Counter] = *Ptr;
        };
      }
    };
  
   // If not set then SET APN Name
   if(Flag_APNSet == RESET)
   {
     char GPRS_MsgBuffer[100];
     memset(GPRS_MsgBuffer,0,sizeof(GPRS_MsgBuffer));
     sprintf(GPRS_MsgBuffer,"AT+CGDCONT=1,\"IP\",\"%s\"",APN);
     Modem_PutString(GPRS_MsgBuffer); WAIT_MODEM_RESP(2);
   }
 }
  
  Flag_ModemOK = RESET;

}


/*******************************************************************************
* Function Name : Get_APN_Routine
* Description   : Config GPRS_Modem
* Input         : Parameter name & place
* Output        : Parameter new value
* Return        : Paremeter
*******************************************************************************/
void Get_APN_Routine(void)
{
  char *pApn;
  pApn = NULL;
  
  /* extract the name of the operator */
  if((pApn = strstr((char*)GPRS_Buffer,"PN:")) != NULL) 					// RECEIVE RESPONSE FROM THE MODEM
  {
    pApn += 4;
    if(*pApn == 0x22)
    {
      pApn += 1;
      for(GP_Counter = 0;GP_Counter < 10; GP_Counter ++,pApn++)
      {
	if(*pApn == 0x22){GPRSData.Operator[GP_Counter] = 0; break;}
	else 
	{
	  GPRSData.Operator[GP_Counter] = *pApn; Flag_Modem_process_ongoing = RESET;
	  if(islower(GPRSData.Operator[GP_Counter]))GPRSData.Operator[GP_Counter] = toupper(GPRSData.Operator[GP_Counter]);
	};
      };
    };
  }
  
  memset(GPRS_Buffer,0,50);GPRS_Count = 0;
  char APN[100] = {'\0'};
  
  /* set apn name on the basis of the operator */
  if((strstr((char*)GPRSData.SET_APN,"no-apn")) || (strstr((char*)GPRSData.SET_APN,"NO-APN"))||(strstr((char*)GPRSData.SET_APN,"")))  
  {
         if( strstr((char*)GPRSData.Operator,"AIRTEL")) sprintf(GPRSData.SET_APN,"airtelgprs.com");//sprintf(SET_APN,"IOT.COM");
    else if((strstr((char*)GPRSData.Operator,"IDEA")) || (strstr((char*)GPRSData.Operator,"!DEA"))|| (strstr((char*)GPRSData.Operator,"ESCOTEL")))sprintf(GPRSData.SET_APN,"isafe");//internet
    else if((strstr((char*)GPRSData.Operator,"HUTCH")) || (strstr((char*)GPRSData.Operator,"ESSAR"))||(strstr((char*)GPRSData.Operator,"VODAFONE")))sprintf(GPRSData.SET_APN,"www"); // was www
    else if((strstr((char*)GPRSData.Operator,"40510")) || (strstr((char*)GPRSData.Operator,"RELIANCE"))) sprintf(GPRSData.SET_APN,"rcomnet");
    else if((strstr((char*)GPRSData.Operator,"40580")) || (strstr((char*)GPRSData.Operator,"AIRCEL")))sprintf(GPRSData.SET_APN,"aircelgprs.com");
    else if((strstr((char*)GPRSData.Operator,"MAHANAGAR")) || (strstr((char*)GPRSData.Operator,"BSNL"))||(strstr((char*)GPRSData.Operator,"SANCHARNET")))sprintf(GPRSData.SET_APN,"bsnlnet");
    else if(strstr((char*)GPRSData.Operator,"MTNL"))sprintf(APN,"mtnl.net");
    else if(strstr((char*)GPRSData.Operator,"VI INDIA"))sprintf(GPRSData.SET_APN,"www");
    //Cell_location_state = 1;//tbd
  }
  else  sprintf(APN,"%s",GPRSData.SET_APN);
  
  Flag_APNSet = SET;
}
 
///*******************************************************************************
//* Function Name : MODULE_EC200_RST
//* Description   : Reset the 4G Modem
//* Input         : none
//* Output        : None
//* Return        : None
//*******************************************************************************/
//void MODULE_EC200_RST(void)
//{
//  
//  Flag_MODULE_EC200_RST = RESET;
// TCP_State  = OPEN_SOCKET;
///*---------- RESET GPRS DATA -------------------------------------------------*/
//  GPRSData.SIM_Present[0] = '0';
//  sprintf(GPRSData.Operator,"N.A");                                             // SET OPERATOR NAME
//  
//  sprintf(GPRSData.Cell_ID,"");                                              // SET CELL ID NONE
//  sprintf(GPRSData.Mobile_No,"");                                     // SET MOBILE NUMBER NONE
//  sprintf(GPRSData.IMEI_No,"N.A.000000");
//  sprintf(GPRSData.Cell_Long," ");
//  sprintf(GPRSData.ICCID,"N.A");
//  sprintf(GPRSData.Operator,"");
//  
//  sprintf(GPRSData.Cell_Lat," ");
//  GPRSData.Signal_Strength = 0;
//  sprintf(GPRSData.SIM_Present,"0");
//  GPRSData.Home_Network = RESET;
//  GPRSData.MCC = 0;
//  GPRSData.MNC = 0;
// 
//  /*----- FLAG STATUS --------------------------------------------------------*/
//  Flag_ModemRdy             = RESET;                                            // MODEM READY RESET       
//  Flag_ValidNetwork         = RESET;                                            // VALID NETWORK RESET 
//  Flag_APNSet               = RESET;                                            // APN NAME RESET 
//  Flag_NoResp               = SET;                                              // INITIAL NO RESPONSE FROM MODEM 
//  Flag_CellChange           = SET;                                              // AS TO INITIALIZE GPRS              
//  Flag_GNSS_Ongoing         = RESET;
//  Flag_TCP_Process_Ongoing  = RESET;
////  Flag_TCP_Routine          = RESET;
//  Flag_Init_GPRS            = RESET;
//   Flag_GPRSConnected = RESET;
//  Check_SMSTime     = 10;                                                       // CHECK SMS INTERVAL 60 SECONDS @ STARTUP
//   Modem_NoResp      = 0;
////  No_Hit_Time       = 0;
//  GPRS_ModemErr     = 0;
//  EC200_ONTime      = 15;
//  Config_State      = 0;
//  Modem_RST_Cnt++;
////  GPRS_OFF;
//}
//


