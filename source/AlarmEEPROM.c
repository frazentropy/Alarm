#include "AlarmEEPROM.h"

unsigned char AlarmEEPROMRead(unsigned char address) {
	EEADR = address;        /* set EEPROM read address */
	EECON1bits.EEPGD = 0;	/* access EEPROM memory */
	EECON1bits.CFGS = 0;	/* do not access configuration registers */
	EECON1bits.RD = 1;      /* read */
	return EEDATA;          /* return byte from EEPROM */
}

unsigned char AlarmEEPROMWrite(unsigned char address,unsigned char data) {
	EECON1bits.WREN = 1;	/* enable EEPROM writes */
	EEADR = address;        /* set EEPROM write address */
	EEDATA = data;          /* load data to write to EEPROM */
	EECON1bits.EEPGD = 0;	/* access EEPROM memory */
	EECON1bits.CFGS = 0;	/* do not access configuration registers */
	INTCONbits.GIE = 0;     /* disable interrupts while writing to EEPROM */
	
	EECON2 = 0x55;
	EECON2 = 0xAA;
	EECON1bits.WR = 1;
	
	while (EECON1bits.WR) ;	/* wait for write to complete */
	EECON1bits.WREN = 0;	/* disallow EEPROM writes */
	
	if (AlarmEEPROMRead(address) == data) {
		return 0;	/* write successful */
	} else {
		return 1;	/* write failed */
	}
}
