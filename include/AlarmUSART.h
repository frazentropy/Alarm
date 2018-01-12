/* 
 * File:   AlarmUSART.h
 * Author: frazbot
 *
 * Created on December 7, 2017, 6:35 PM
 */

#ifndef ALARMUSART_H
#define	ALARMUSART_H

#include <xc.h>
#include <stdio.h>              /* access to printf() */

/* function prototypes */
void putch(unsigned char);      /* re-route stdout */
void AlarmUSARTInit(void);      /* configuration for USART */
void AlarmUSARTClearRX(void);   /* clear FIFO buffer for clean receive */
void clrScreen(void);

char clr[] = {27, '[', '2', 'J', 27, '[', 'H', '\0'};


#endif	/* ALARMUSART_H */

