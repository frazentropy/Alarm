#include "AlarmUSART.h"

void putch(unsigned char data) {
	//while (!PIR1bits.TXIF)	/* wait until transmitter is ready */
	//	continue;
	while (!TXSTAbits.TRMT) /* wait until transmitter is ready */
		continue;
	
	TXREG = data;		/* send one character */
}

void AlarmUSARTInit() {
	/* USART configuration */
	TRISCbits.RC7 = 1;		/* RX is INPUT */
	TRISCbits.RC6 = 0;		/* TX is OUTPUT */
	SPBRG = 129;			/* high speed: 9600 baud for Fosc = 20 MHz */
	TXSTAbits.SYNC = 0;		/* asynchronous mode */
	TXSTAbits.BRGH = 1;		/* high speed baud rate */
	TXSTAbits.TX9 = 0;		/* 8-bit transmit */
	RCSTAbits.RX9 = 0;		/* 8-bit receive */
	RCSTAbits.SPEN = 1;		/* enable RX and TX */
	RCSTAbits.CREN = 1;		/* enable continuous receive */
	TXSTAbits.TXEN = 1;		/* enable transmitter */
	
	/* set up USART interrupts*/
	IPR1bits.RCIP = 0;		/* RX set as low priority interrupt */
	//PIE1bits.RCIE = 1;		/* enable interrupt on RX */
	//PIE1bits.TXIE = 0;		/* disable interrupt on TX */
    
	printf("Serial connection ready.\r\n");
}

/* ensure RX buffer is empty */
void AlarmUSARTClearRX() {
	volatile char c;
	c = RCREG;
	c = RCREG;
	c = RCREG;
}

/* VT-100 compatible clear screen command */
void clrScreen() {
    printf("%s", clr);
}