#ifndef __SMS_H
#define __SMS_H

/*------------------ SMS Cmd -------------------*/
typedef enum
{
  SET_APN,
  GET_INFO,
  GET_STATUS,
  CPU_RESET,
  INVALID_CMD,
  INVALID_LOGIN,
  INVALID_PSW
}SMS_CmdType_TypeDef;

void SMS_Init(void);
void Send_SMS(unsigned char Subject);
#endif /*__SMS_H*/
