/* 
 * File:   AlarmUsers.h
 * Author: frazbot
 *
 * Created on December 8, 2017, 2:05 AM
 */

#ifndef ALARMUSERS_H
#define	ALARMUSERS_H

#include "AlarmEEPROM.h"
#include "AlarmUSART.h"
#include "AlarmKeypad.h"
#include "AlarmMotion.h"
#include "AlarmTemp.h"
#include <xc.h>
#include <ctype.h>
#include <stdlib.h>

enum User {
    MASTER,
    USER_1,
    USER_2,
    USER_3,
    USER_4,
    USER_5,
    USER_6,
    USER_7,
    INVALID
};

/* function prototypes */
char isValidUser(enum User);
void userMenu(enum User);
void userSettingsMenu(enum User user);
void motionMenu(void);
void tempMenu(void);
void addUser(void);
void requestUserCode(enum User user, char *code);
void setUserCode(enum User user, char *code);
char validateMenuInput(char input, char *validInputs, char size);


#endif	/* ALARMUSERS_H */

