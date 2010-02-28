// alle StandartHeaderfiles (siehe Aufgabe 12) einbinden
#include "aufgabe17.h"	// Headerdatei des Haubtprogramms

//Zuweisung der Interruptvektoren auf die ISR Behandlungsroutinen
TIMERB0_ISR(ISR_Timer_B_CCR0)    //int13 | 0xFFFA | Timer B CC0

__interrupt void ISR_Timer_B_CCR0 (void) {
	// rote LED toggeln
	P4OUT ^=  0x01;

	// Interruptflag entfernen um eine Wiederholung der ISR, wegen
	// gesetzem Interruptflag, zu verhindern
	TBCCTL0 &= ~CCIFG;
}	
