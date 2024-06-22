#ifndef __MISC_H
#define __MISC_H

void App_Init(void);

void GP_timer(void);
void FLASH_Erase(uint32_t FlashEraseAddress);
void HAL_FLASH_READ(char *Buff, uint32_t Addr, int Buff_Lnt);
void Management_Processes(void);
void Wait_For_Resp(void);

#endif