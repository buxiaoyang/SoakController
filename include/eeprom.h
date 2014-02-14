/*------------------------------------------------------------------*/
/* --- STC MCU Limited ---------------------------------------------*/
/* --- STC12C5Axx Series MCU ISP/IAP/EEPROM Demo -------------------*/
/* --- Mobile: (86)13922805190 -------------------------------------*/
/* --- Fax: 86-755-82905966 ----------------------------------------*/
/* --- Tel: 86-755-82948412 ----------------------------------------*/
/* --- Web: www.STCMCU.com -----------------------------------------*/
/* If you want to use the program or the program referenced in the  */
/* article, please specify in which data and procedures from STC    */
/*------------------------------------------------------------------*/


#ifndef __EEPROM_H__
#define __EEPROM_H__

typedef unsigned char BYTE;
typedef unsigned int WORD;

//Start address for STC11/10xx EEPROM
#define IAP_ADDRESS 0x0000

void Delay(BYTE n);
void IapIdle();
BYTE IapReadByte(WORD addr);
void IapProgramByte(WORD addr, BYTE dat);
void IapEraseSector(WORD addr);
#endif
