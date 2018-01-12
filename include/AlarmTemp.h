/* 
 * File:   AlarmTemp.h
 * Author: frazbot
 *
 * Created on December 14, 2017, 8:47 AM
 */

#ifndef ALARMTEMP_H
#define	ALARMTEMP_H


#include <xc.h>
#include "AlarmUSART.h"
#include <stdio.h>

#define SLOPE 2.046   /* ADC val <=> 10 mV <=> 1 degC */

char currentTemp = 10;
char maxTemp = 27;
char tempAlarmEnabled = 0;
int adcVal0 = 153;
int temp0 = 20;
char tempAlarmTriggered = 0;

void AlarmTempInit(void);
void updateTemp(void);
char getCurrentTemp(void);
char getMaxTemp(void);
void setMaxTemp(char);
void setTempAlarm(void);
void clearTempAlarm(void);
char tempAlarmIsEnabled(void);
void calibrateTempSensor(char);
void triggerTempAlarm(void);
char tempAlarmIsTriggered(void);
void enableTempAlarm(void);
void disableTempAlarm(void);

#endif	/* ALARMTEMP_H */

