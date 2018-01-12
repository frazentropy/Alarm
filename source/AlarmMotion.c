#include "AlarmMotion.h"

void AlarmMotionInit() {
	TRISBbits.TRISB0 = 1;
    
    printf("Motion sensor initializing ...\r\n");
}

/* called every tick to delay motion sensor initialization by a minute */
void bringMotionOnline() {
    /* quick return if motion is already initialized */
	if (getMotionStatus() != MOTION_UNAVAILABLE) {
		return;
	}
    /* increment counter until a minute has passed */
	if (tmrOvfCtr < OVF_PER_MIN) {
		tmrOvfCtr++;
		return;
	}
	
    /* activate motion sensor after a minute has passed */
	printf("Motion sensor initialized.\r\n");
	motionStatus = MOTION_NO_TRIGGER;
	INTCON2bits.INTEDG0 = 1;
	INTCONbits.INT0IF = 0;
	INTCONbits.INT0E = 1;
}

/* called by ISR to trigger motion alarm, if enabled */
enum motionState updateMotionStatus() {
	if (PORTBbits.RB0 && (motionStatus != MOTION_TRIGGER) && enabled) {
		printf("MOTION ALARM TRIGGERED!\r\n");
		printf("Enter security code now.\r\n");
		PORTEbits.RE1 = 1;
		motionStatus = MOTION_TRIGGER;
	}
}

enum motionState getMotionStatus() {
	return motionStatus;
}

char motionAlarmIsEnabled() {
	return enabled;
}

void enableMotionAlarm() {
	enabled = 1;
}

void disableMotionAlarm() {
	enabled = 0;
	PORTEbits.RE1 = 0;
    if (motionStatus != MOTION_UNAVAILABLE)
        motionStatus = MOTION_NO_TRIGGER;
}
