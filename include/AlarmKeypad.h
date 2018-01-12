/* 
 * File:   AlarmKeypad.h
 * Author: frazbot
 *
 * Created on December 10, 2017, 11:30 PM
 */

#ifndef ALARMKEYPAD_H
#define	ALARMKEYPAD_H

#include <xc.h>
#include "AlarmUSART.h"

/* columns */
#define M4  PORTDbits.RD7
#define M3  PORTDbits.RD6
#define M2  PORTDbits.RD5
#define M1  PORTDbits.RD4

/* rows */
#define N1  PORTCbits.RC0
#define N2  PORTCbits.RC1
#define N3  PORTCbits.RC2
#define N4  PORTCbits.RC3

enum keyPress {
    KEY_0,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,
    KEY_A,
    KEY_B,
    KEY_C,
    KEY_D,
    KEY_AST,
    KEY_OCT,
    INVALID_KEY
};

enum row {
    ROW_M1,
    ROW_M2,
    ROW_M3,
    ROW_M4,
    INVALID_ROW
};

enum column {
    COL_N1,
    COL_N2,
    COL_N3,
    COL_N4,
    INVALID_COLUMN
};

enum keyPress lastKeyPressed = INVALID_KEY;
enum row r = ROW_M1;
char keyBuff[5];
char *keyWR = keyBuff;
char *keyRD = keyBuff;

enum keyPress getKeyPress(char);
void AlarmKeypadInit(void);
char sweepKeypadRows(void);
char keyToChar(enum keyPress);
void writeNewKeyChar(char);
char readNewKeyChar(void);

char *getKeyBuff(char *);
char getKeyCharCount(void);
void flushKeyBuff(void);

#endif	/* ALARMKEYPAD_H */

