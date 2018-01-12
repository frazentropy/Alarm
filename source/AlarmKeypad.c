#include "AlarmKeypad.h"

void AlarmKeypadInit() {
	
	/* set digital outputs for rows M1 ... M4 */
	TRISDbits.TRISD4 = 0;
	TRISDbits.TRISD5 = 0;
	TRISDbits.TRISD6 = 0;
	TRISDbits.TRISD7 = 0;
	
	/* set digital inputs for columns N1 ... N4 */
	TRISCbits.TRISC0 = 1;
	TRISCbits.TRISC1 = 1;
	TRISCbits.TRISC2 = 1;
	TRISCbits.TRISC3 = 1;
	
	/* set up timer to sweep outputs */
	T0CON = 00000011;
	INTCON2bits.TMR0IP = 0;
	INTCONbits.TMR0IF = 0;
	INTCONbits.TMR0IE = 1;
	T0CONbits.TMR0ON = 1;
    
    printf("Keypad ready.\r\n");
}

char sweepKeypadRows() {
	char Dout, Cin;
	for (int i = 0; i < 4; i++) {
		Dout = 0x01 << (4+i);
		PORTD = Dout;
		r = i;
		Cin = PORTC & 0b00001111;
		if (Cin) {
			return Cin;
		}
	}
	
	lastKeyPressed = INVALID_KEY;
	return 0;
}

enum keyPress getKeyPress(char portC) {
	
	enum column col = INVALID_COLUMN;
	enum keyPress keyToReturn = INVALID_KEY;
	for (char i = 0; i < 4; i++) {
		if (((portC >> i) & 0x01)) {
			col = i;
			break;
		}	
	}
	switch (col) {
	case COL_N1:
		switch (r) {
		case ROW_M1:
			keyToReturn = KEY_1;
			break;
		case ROW_M2:
			keyToReturn = KEY_4;
			break;
		case ROW_M3:
			keyToReturn = KEY_7;
			break;
		case ROW_M4:
			keyToReturn = KEY_AST;
			break;
		default:
			break;
		}
		break;
	case COL_N2:
		switch (r) {
		case ROW_M1:
			keyToReturn = KEY_2;
			break;
		case ROW_M2:
			keyToReturn = KEY_5;
			break;
		case ROW_M3:
			keyToReturn = KEY_8;
			break;
		case ROW_M4:
			keyToReturn = KEY_0;
			break;
		default:
			break;
		}
		break;
	case COL_N3:
		switch (r) {
		case ROW_M1:
			keyToReturn = KEY_3;
			break;
		case ROW_M2:
			keyToReturn = KEY_6;
			break;
		case ROW_M3:
			keyToReturn = KEY_9;
			break;
		case ROW_M4:
			keyToReturn = KEY_OCT;
			break;
		default:
			break;
		}
		break;
	case COL_N4:
		switch (r) {
		case ROW_M1:
			keyToReturn = KEY_A;
			break;
		case ROW_M2:
			keyToReturn = KEY_B;
			break;
		case ROW_M3:
			keyToReturn = KEY_C;
			break;
		case ROW_M4:
			keyToReturn = KEY_D;
			break;
		default:
			break;
		}
		break;
	default:
		keyToReturn = INVALID_KEY;
		break;
	}
	
	if (lastKeyPressed == keyToReturn) {
		return INVALID_KEY;
	}
	
	lastKeyPressed = keyToReturn;
	return keyToReturn;
}

char keyToChar(enum keyPress key) {
	switch (key) {
	case KEY_0:
	case KEY_1:
	case KEY_2:
	case KEY_3:
	case KEY_4:
	case KEY_5:
	case KEY_6:
	case KEY_7:
	case KEY_8:
	case KEY_9:
		return (char)(key + 0x30);
	case KEY_A:
	case KEY_B:
	case KEY_C:
	case KEY_D:
		return (char)(key - KEY_A + 0x41);
	case KEY_AST:
		return '*';
	case KEY_OCT:
		return '#';
	default:
		break;
	}
	return 0;
}

void writeNewKeyChar(char c) {
	if ((keyWR - keyBuff) >= (sizeof(keyBuff) / sizeof(char))) {
		flushKeyBuff();
	}
	*keyWR++ = c;
}

char readNewKeyChar() {
	if ((keyRD - keyBuff) >= (sizeof(keyBuff) / sizeof(char))) {
		return 0;
	}
	
	if ((keyWR - keyRD) > 0) {
		return *keyRD++;
	}
	
	return 0;
}

void flushKeyBuff() {
	keyWR = keyBuff;
	keyRD = keyBuff;
}

char *getKeyBuff(char *size) {
	*size = getKeyCharCount();
	if (*size) return keyBuff;
	else return NULL;
}

char getKeyCharCount() {
	return (keyWR - keyBuff);
}
