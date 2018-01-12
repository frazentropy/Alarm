/* 
 * File:   AlarmMotion.h
 * Author: frazbot
 *
 * Created on December 14, 2017, 6:28 AM
 */

#ifndef ALARMMOTION_H
#define	ALARMMOTION_H

#include "AlarmUSART.h"
#include <xc.h>
#include <stdio.h>

#define OVF_PER_MIN  2440
#define MOTION_PIN PORTBbits.RB0;

enum motionState {
    MOTION_UNAVAILABLE,
    MOTION_NO_TRIGGER,
    MOTION_TRIGGER
};

enum motionState motionStatus = MOTION_UNAVAILABLE;
unsigned int tmrOvfCtr = 0;
char enabled = 0;

void AlarmMotionInit(void);
void bringMotionOnline(void);
enum motionState updateMotionStatus(void);
enum motionState getMotionStatus(void);
char motionAlarmIsEnabled(void);
void enableMotionAlarm(void);
void disableMotionAlarm(void);

#endif	/* ALARMMOTION_H */

