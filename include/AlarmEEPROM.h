/* 
 * File:   AlarmEEPROM.h
 * Author: frazbot
 *
 * Created on December 7, 2017, 10:52 PM
 */

#ifndef ALARMEEPROM_H
#define	ALARMEEPROM_H

#include <xc.h>

/* EEPROM addresses */
#define	IS_FIRST_BOOT           0x00    /* 1 byte */
#define MOTION_ALARM_ENABLED    0x02    /* 1 byte */
#define TEMP_ALARM_ENABLED      0x04    /* 1 byte */
#define ENABLED_USERS           0x06    /* 1 byte */
#define MASTER_CODE             0x08    /* 4 bytes */
#define USER_1_CODE             0x0C    /* 4 bytes */
#define USER_2_CODE             0x10    /* 4 bytes */
#define USER_3_CODE             0x14    /* 4 bytes */
#define USER_4_CODE             0x18    /* 4 bytes */
#define USER_5_CODE             0x1C    /* 4 bytes */
#define USER_6_CODE             0x20    /* 4 bytes */
#define USER_7_CODE             0x24    /* 4 bytes */

/* initial EEPROM values */
__EEPROM_DATA (0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00);
__EEPROM_DATA (0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
__EEPROM_DATA (0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
__EEPROM_DATA (0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
__EEPROM_DATA (0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

/* function prototypes */
unsigned char AlarmEEPROMRead(unsigned char);
unsigned char AlarmEEPROMWrite(unsigned char, unsigned char);

#endif	/* ALARMEEPROM_H */

