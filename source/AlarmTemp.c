#include "AlarmTemp.h"

void AlarmTempInit(void) {
	TRISAbits.TRISA0 = 1;
	
	ADCON1 = 0b00001110;
	ADCON0 = 0b00000000;
	ADCON2 = 0b10110110;
	ADCON0 |= 0x01;
	PIR1bits.ADIF = 0;
	PIE1bits.ADIE = 1;
    
    printf("Temperature sensor ready.\r\n");
    //calibrateTempSensor(21);
}

/* called after successful conversion to get new temperature */
void updateTemp(void) {
    PORTEbits.RE0 = 1;
    while (ADCON0bits.GO) ;
    currentTemp = (char)(((double)(((ADRESH << 8) + ADRESL) - adcVal0) / SLOPE) + temp0);
    if ((currentTemp > maxTemp) && tempAlarmEnabled)
        triggerTempAlarm();
    else
        clearTempAlarm();
    if (!tempAlarmTriggered) {
        PORTEbits.RE0 = 0;
    }
}

char getCurrentTemp() {
    return currentTemp;
}

char getMaxTemp() {
    return maxTemp;
}

void setMaxTemp(char newTemp) {
    maxTemp = newTemp;
}

void enableTempAlarm() {
    tempAlarmEnabled = 1;
}

void disableTempAlarm() {
    tempAlarmTriggered = 0;
    PORTAbits.RA5 = 0;
    tempAlarmEnabled = 0;
}

void clearTempAlarm() {
    tempAlarmTriggered = 0;
    PORTAbits.RA5 = 0;
}

char tempAlarmIsEnabled() {
    return tempAlarmEnabled;
}

void triggerTempAlarm() {
    tempAlarmTriggered = 1;
    PORTAbits.RA5 = 1;
}

/* changes offset used by temperature formula--maps current ambient temp (given
 * by user) to current ADC value) */
void calibrateTempSensor(char temp) {
    INTCONbits.GIE = 0;
    temp0 = temp;
    while (!ADCON0bits.DONE) ;
    adcVal0 = (ADRESH << 8) + ADRESL;
    INTCONbits.GIE = 1;
}

char tempAlarmIsTriggered() {
    return tempAlarmTriggered;
}