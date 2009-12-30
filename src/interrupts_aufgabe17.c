#include "msp430x16x.h"	// Systemdefinitionen von TI fur den MSP430F1612
#include "interrupts.h"	// ISR - Interrupt Service Routinen
#include "system.h"		// Systemfunktionen MSB430H
#include "CC1100.h"		// CC1100 Funktransceiver
#include "stdio.h"		// includes TI MSP430F1612
#include "aufgabe17.h"	// Headerdatei des Haubtprogramms

//Zuweisung der Interruptvektoren auf die ISR Behandlungsroutinen
TIMERB0_ISR(ISR_Timer_B_CCR0)    //int13 | 0xFFFA | Timer B CC0

__interrupt void ISR_Timer_B_CCR0 (void) {
	// rote LED toggeln
	P4OUT ^=  0x01;

	// Interuptflag entfernen um eine Wiederholung der ISR, wegen
	// gesetzem Interuptfalg, zu verhindern
	TBCCTL0 &= ~CCIFG;
}	
