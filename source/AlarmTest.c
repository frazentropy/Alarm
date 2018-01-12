#include "AlarmConfig.h"
#include "AlarmEEPROM.h"
#include "AlarmUSART.h"
#include "AlarmUsers.h"
#include "AlarmKeypad.h"
#include "AlarmMotion.h"
#include "AlarmTemp.h"

#include <xc.h>

void AlarmPicInit(void);
void AlarmInitialSetup(void);
void startInterrupts(void);
enum User getValidCode(void);
enum User verifyCode(char *);
void invalidCode(void);
void interrupt low_priority Alarm_ISR_Low(void);
void interrupt high_priority Alarm_ISR_High(void);

void main() {
    /* make hardware assignments and initialize components */
    AlarmPicInit();
    
    /* if first boot, enter initial setup */
    char c = AlarmEEPROMRead(IS_FIRST_BOOT);
    if (c) {
        AlarmInitialSetup();
        AlarmEEPROMWrite(ENABLED_USERS, 0x01);
		AlarmEEPROMWrite(IS_FIRST_BOOT, 0x00);
    }
    
    startInterrupts();
    
    while(1) {
        /* get rid of latent key presses */
        flushKeyBuff();
        
        /* empty the screen */
        clrScreen();
        printf("\r\nEnter security code: ");
        
        /* get user back after waiting for passcode */
		char user = getValidCode();
		if (user == INVALID) {
			invalidCode();
		} else {
			disableMotionAlarm();
            
            /* log in validated user */
			userMenu(user);
		}
    }
}

/* makes pin assignments used by main() and initializes individual components */
void AlarmPicInit() {
    /* assign LED outputs */
    TRISEbits.TRISE0 = 0;
    TRISEbits.TRISE1 = 0;
    TRISAbits.TRISA4 = 0;
    TRISAbits.TRISA5 = 0;
    
    PORTEbits.RE0 = 0;
    PORTEbits.RE1 = 0;
    PORTAbits.RA4 = 0;
    PORTAbits.RA5 = 0;
    
    /* initialize components */
    AlarmUSARTInit();	/* USART configuration */
	AlarmKeypadInit();	/* keypad configuration */
	AlarmMotionInit();	/* motion sensor configuration */
	AlarmTempInit();	/* temperature sensor configuration */
}

/* starts core system functionality */
void startInterrupts() {
    RCONbits.IPEN = 1;      /* enable interrupt priorities */
	INTCONbits.PEIE = 1;	/* enable low priority interrupts */
	INTCONbits.GIE = 1;     /* enable high priority interrupts */
	ADCON0bits.GO = 1;      /* start AD conversion */
    PORTAbits.RA5 = 1;      /* turn on SYSTEM READY LED */
    printf("Alarm system is ready.\r\n");
}

/* gets and saves Master passcode; only called when system is booted
 up the first time after programming */
void AlarmInitialSetup() {
    clrScreen();
    printf("Entering initial system setup ...\r\n");
	printf("============================\r\n");
	printf("Initial System Configuration\r\n");
	printf("============================\r\n");
	char masterCode[4];
	requestUserCode(MASTER, masterCode);
	setUserCode(MASTER, masterCode);
}

/* validates an entered code char-by-char, over serial and keypad
 * simultaneously */
enum User getValidCode() {
    AlarmUSARTClearRX();
	enum User user = INVALID;
	char size;
	char enteredCode[] = {' ',' ',' ',' '};
	int i = 0;
	while (i < 4) {
		while (!PIR1bits.RCIF)	{
            if (getKeyCharCount() == 4) {
                user = verifyCode(getKeyBuff(&size));
                flushKeyBuff();
                if (user != INVALID) {
                    return user;
                } else {
                    printf("Invalid code.\r\n");
                }
            }
		}
        
        flushKeyBuff();
        printf("\r\n");        
		enteredCode[i] = RCREG;
		if (isdigit(enteredCode[i])) {
			i++;
		} else if (enteredCode[i] == 'R') {
			printf("Rebooting ...\r\n");
			RESET();
		} else {
			continue;
		}
	}
    
	return verifyCode(enteredCode);
}

enum User verifyCode(char *enteredCode) {
    enum User user = MASTER;
	unsigned char enabledUsers = AlarmEEPROMRead(ENABLED_USERS);
	char match = 0;
	while (user <= USER_7) {
		for (int i = 0; i < 4; i++) {
			if (enteredCode[i] != AlarmEEPROMRead(MASTER_CODE + 4*user + i)) {
				match = 0;
				break;	/* does not match this user's code */
			}
			match = 1;
		}
		if (match) break;
		user++;
		if (!((enabledUsers >> user) & 0x01)) user++;
	}
	return user;
}

void invalidCode() {
    
}

/* Low-priority interrupts:
 * Timer 0 (THIS IS AN ERROR--TIMER 0 cannot be low
 */
void interrupt low_priority Alarm_ISR_Low() {
    	if (INTCONbits.TMR0IE && INTCONbits.TMR0IF) {
		T0CONbits.TMR0ON = 0;
		bringMotionOnline();
		char Cin = sweepKeypadRows();
		if (Cin) {
			PORTAbits.RA4 = 1;
			char c = keyToChar(getKeyPress(Cin));
			if (c) {
				writeNewKeyChar(c);
				char size;
				char *buff = getKeyBuff(&size);
				if (size >= 4) {
					for (int i = 0; i < size; i++) {
						printf("*");
					} printf("\r\n");
				}
			}
			PORTAbits.RA4 = 0;
		}
		
		INTCONbits.TMR0IF = 0;
		T0CONbits.TMR0ON = 1;
	}
}

/* High-priority interrupts: 
 * Motion sensor
 * Temperature sensor
 */
void interrupt high_priority Alarm_ISR_High() {
    if (INTCONbits.INT0E && INTCONbits.INT0IF) {
		updateMotionStatus();
		INTCONbits.INT0IF = 0;
	} else if (PIE1bits.ADIE && PIR1bits.ADIF) {
		updateTemp();
		PIR1bits.ADIF = 0;
		ADCON0bits.GO = 1;
	}
}
