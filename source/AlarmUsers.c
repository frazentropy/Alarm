#include "AlarmUsers.h"

char isValidUser(enum User user) {
	return 1;
}

void userMenu(enum User user) {
    char inChar, k;
    char validInput[] = {'1','2','3','4'};
	while (1) {
        clrScreen();
        do {
            inChar = 0;
            k = 0;
			AlarmUSARTClearRX();
            flushKeyBuff();
			printf("===================\r\n");
			printf("Main Menu -- ");
			if (user == MASTER) printf("MASTER\r\n");
			else printf("USER %c\r\n", user+48);
			printf("===================\r\n");
            printf("[1] Motion: ");
            if (getMotionStatus() == MOTION_UNAVAILABLE) {
                printf(" Initializing, ");
            } else if (getMotionStatus() == MOTION_NO_TRIGGER) {
                printf(" Initialized, ");
            }
            if (getMotionStatus() == MOTION_TRIGGER) {
                printf(" MOTION ALERT TRIGGERED ([1][C] to clear)\r\n");
            } else if (motionAlarmIsEnabled()) {
                printf(" Enabled\r\n");
            } else {
                printf(" Disabled\r\n");
            }
            printf("[2] Temperature: ");
            if (tempAlarmIsTriggered()) {
                printf(" TEMPERATURE ALERT TRIGGERED ([2][C] to clear)\r\n");
            } else if (tempAlarmIsEnabled()) {
                printf(" Alert Enabled, %d degC\r\n", getCurrentTemp());
            } else {
                printf(" Alert Disabled, %d degC\r\n", getCurrentTemp());
            }
			printf("[3] User settings\r\n");
			printf("[4] Exit\r\n");
            
            while (!PIR1bits.RCIF)	{
                if (getKeyCharCount()) {
                    k = readNewKeyChar();
                    if (validateMenuInput(k, validInput, sizeof(validInput) / sizeof(char))) {
                        printf("-> %c\r\n", k);
                        inChar = k;
                        break;
                    }
                }
            }
            if (!inChar) {
                inChar = RCREG;
                while (inChar == '\n') {
                	inChar = RCREG;
                }
            }
        } while (!validateMenuInput(inChar, validInput, sizeof(validInput) / sizeof(char)));
    
		
        switch (inChar) {
        case '1':
            motionMenu();
            break;
        case '2':
            tempMenu();
            break;
        case '3':
            userSettingsMenu(user);
            break;
        case '4':
            return;
        default:
            break;
        }
    }
}

void userSettingsMenu(enum User user) {
    char inChar, k;
    char validInput[] = {'1','2','3','4'};
	while (1) {
        clrScreen();
        do {
            inChar = 0;
            k = 0;
            flushKeyBuff();
			AlarmUSARTClearRX();
            if (user != MASTER && inChar > '2') break;
            printf("================\r\n");
			printf("User Settings-- ");
			if (user == MASTER) printf("MASTER\r\n");
			else printf("USER %c\r\n", user+48);
			printf("================\r\n");
			printf("[1] Change security code\r\n");
			printf ("[2] Exit\r\n");
			if (user == MASTER) {
				printf("[3] Add user\r\n");
				printf("[4] Remove user\r\n");
			}
            while (!PIR1bits.RCIF)	{
                if (getKeyCharCount()) {
                    k = readNewKeyChar();
                    if (validateMenuInput(k, validInput, sizeof(validInput) / sizeof(char))) {
                        inChar = k;
                        break;
                    }
                }
            }
            if (!inChar) {
                inChar = RCREG;
                while (inChar == '\n') {
                	inChar = RCREG;
                }
            }
        } while (!validateMenuInput(inChar, validInput, sizeof(validInput) / sizeof(char)));
        
        switch (inChar) {
		case '1':
			break;
		case '2':
			return;
		case '3':
			if (user == MASTER) addUser();
			break;
		case '4':
			break;
		default:
			break;
		}
	}
}

void motionMenu() {
    char inChar, k;
    char validInput[] = {'1','2','3','C'};
	while (1) {
        clrScreen();
        do {
            inChar = 0;
            k = 0;
			AlarmUSARTClearRX();
            flushKeyBuff();
			printf("=====================\r\n");
			printf("Motion Detector\r\n");
			printf("=====================\r\n");
            if (getMotionStatus() == MOTION_UNAVAILABLE) {
                printf("Motion sensor initializing (this could take a minute)\r\n");
            } else if (getMotionStatus() == MOTION_TRIGGER) {
                printf("Motion sensor has been TRIGGERED. Press [C] to clear.\r\n");
            } else {
                printf("Motion sensor is initialized.\r\n");
            }
            if (motionAlarmIsEnabled()) {
                printf("Motion sensor is ENABLED.\r\n");
                printf("[1] Disable motion alarm\r\n");
            } else {
                printf("Motion sensor is DISABLED.\r\n");
                printf("[1] Enable motion alarm\r\n");
            }
			printf("[2] Update\r\n");
			printf("[3] Exit\r\n");
            
            while (!PIR1bits.RCIF)	{
                if (getKeyCharCount()) {
                    k = readNewKeyChar();
                    if (validateMenuInput(k, validInput, sizeof(validInput) / sizeof(char))) {
                        printf("-> %c\r\n", k);
                        inChar = k;
                        break;
                    }
                }
            }
            if (!inChar) {
                inChar = RCREG;
                while (inChar == '\n') {
                	inChar = RCREG;
                }
            }
        } while (!validateMenuInput(inChar, validInput, sizeof(validInput) / sizeof(char)));
    
		
        switch (inChar) {
        case '1':
            if (motionAlarmIsEnabled()) {
                disableMotionAlarm();
            } else {
                enableMotionAlarm();
            }
            break;
        case '2':
            printf("Updating ...\r\n");
            break;
        case '3':
            return;
        case 'C':
            disableMotionAlarm();
            printf("Motion trigger alert cleared.\n\r");
            break;
        default:
            break;
        }
    }
}

void tempMenu() {
    char inChar, k;
    char validInput[] = {'1','2','3','4','5','C'};
	while (1) {
        clrScreen();
        do {
            char thisTemp = getCurrentTemp();
            char max = getMaxTemp();
            inChar = 0;
            k = 0;
			AlarmUSARTClearRX();
            flushKeyBuff();
			printf("=====================\r\n");
			printf("Temperature Sensor\r\n");
			printf("=====================\r\n");
            if (tempAlarmIsTriggered()) {
                printf("TEMPERATURE ALERT: Temperature above threshold (%d degC > %d degC).Press [C] to clear.\r\n", getCurrentTemp(), getMaxTemp());
            } else {
                printf("Temperature: %d degC (Max: %d degC)\r\n", thisTemp, max); 
            }
            if (tempAlarmIsEnabled()) {
                printf("Temperature alert system is ENABLED (%d degC)\r\n", thisTemp);
                printf("[1] Disable\r\n");
            } else {
                printf("Temperature alert system is DISABLED.\r\n");
                printf("[1] Enable\r\n");
            }
			printf("[2] Update\r\n");
            printf("[3] Change Max Temperature\r\n");
            printf("[4] Calibrate\r\n");
			printf("[5] Exit\r\n");
            
            while (!PIR1bits.RCIF)	{
                if (getKeyCharCount()) {
                    k = readNewKeyChar();
                    if (validateMenuInput(k, validInput, sizeof(validInput) / sizeof(char))) {
                        printf("-> %c\r\n", k);
                        inChar = k;
                        break;
                    }
                }
            }
            if (!inChar) {
                inChar = RCREG;
                while (inChar == '\n') {
                	inChar = RCREG;
                }
            }
        } while (!validateMenuInput(inChar, validInput, sizeof(validInput) / sizeof(char)));
    
		int newTemp = -1;
        char enteredCode[4] = {' ',' ',' ','\0'};
        switch (inChar) {
        case '1':
            if (tempAlarmIsEnabled()) {
                disableTempAlarm();
            } else {
                enableTempAlarm();
            }
            break;
        case '2':
            printf("Updating ...\r\n");
            break;
        case '3':
        case '4':
            if (inChar == '3') {
                printf("Enter new max degC (000 - 100): \r\n");
            } else {
                printf("Enter ambient degC (000 - 100): \r\n");
            }
            do {
                flushKeyBuff();
                AlarmUSARTClearRX();
                char size;
                int i = 0;
                while (i < 3) {
                    while (!PIR1bits.RCIF)	{
                        if (getKeyCharCount() == 3) {
                            writeNewKeyChar('\0');
                            if ((newTemp = atoi(getKeyBuff(&size))) <= 100) {
                                if (newTemp >= 0) {
                                    break;
                                }
                                newTemp = -1;
                            }
                        }
                    }

                    flushKeyBuff();
                    if (newTemp < 0) {    
                        enteredCode[i] = RCREG;
                        if (isdigit(enteredCode[i])) {
                            i++;
                        } else if (enteredCode[i] == 'B') {
                            break;
                        } else {
                            continue;
                        }
                    }
                }
            } while ((atoi(enteredCode) >= 100) || (atoi(enteredCode) <= 0));
            if (inChar == '3') {
                if (newTemp >= 0) {
                    setMaxTemp(newTemp);
                } else {
                    setMaxTemp(atoi(enteredCode));
                }
            } else {
                if (newTemp >= 0) {
                    calibrateTempSensor(newTemp);
                } else {
                    calibrateTempSensor(atoi(enteredCode));
                }
            }
            break;
        case '5':
            return;
        case 'C':
            disableTempAlarm();
            printf("Motion trigger alert cleared.\n\r");
            break;
        default:
            break;
        }
    }
}


void addUser() {
	unsigned char enabledUsers = AlarmEEPROMRead(ENABLED_USERS);
	char i = 1;
	while ((enabledUsers >> i) & 0x01) {
		i++;
		if (i > 7) {
			AlarmUSARTClearRX();
			printf("Max users reached.\r\n");
			printf("Press any key to exit.\r\n");
			while (!PIR1bits.RCIF)	;
			return;
		}
	}
	char user_num = i;
	enabledUsers |= (0x01 << user_num);
	
	char code[4];
	
	requestUserCode(user_num, code);
	setUserCode(user_num, code);
	
	AlarmEEPROMWrite(ENABLED_USERS, enabledUsers);
    INTCONbits.GIE = 1;
	
	printf("USER %c added successfully.\r\n", user_num+48);
	printf("Press any key to exit.\r\n");
	while (RCREG == '\n')	;
	while (!PIR1bits.RCIF)	;
	return;
	
}

void requestUserCode(enum User user, char *code) {
	AlarmUSARTClearRX();
	char isValidCode = 0;
	while (!isValidCode) {
		AlarmUSARTClearRX();
		printf("Enter a new four-digit security code for ");
		if (user == MASTER) printf("MASTER:\r\n");
		else printf("USER %c:\r\n", user+48);
		for (int i = 0; i < 4; i++) {
			while (!PIR1bits.RCIF)	;
			code[i] = RCREG;
			while (code[i] == '\n') {
				code[i] = RCREG;
			}
			if (isdigit(code[i])) {
				isValidCode = 1;
			} else {
				AlarmUSARTClearRX();
				printf("Code must be four digits.\r\n");
				isValidCode = 0;
				break;
			}
		}
	}
	
	
	
	printf("The code for ");
	if (user == MASTER) printf("MASTER ");
	else printf("USER %c ", user+48);
	printf("has been set to %c%c%c%c.\r\n",
		code[0],
		code[1],
		code[2],
		code[3]
	);
}
void setUserCode(enum User user, char *code) {
	for (int i = 0; i < 4; i++) {
		AlarmEEPROMWrite(MASTER_CODE + 4*user + i, code[i]);
	}
}

char validateMenuInput(char input, char *validInputs, char size) {
	char found = 0;
	for (int i = 0; i < size; i++)
		if (input == validInputs[i]) found = 1;
	return found;
}