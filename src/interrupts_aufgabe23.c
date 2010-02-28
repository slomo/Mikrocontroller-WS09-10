// alle StandartHeaderfiles (siehe Aufgabe 12) einbinden
#include "aufgabe23.h"	// Headerdatei des Haubtprogramms

//Zuweisung der Interruptvektoren auf die ISR Behandlungsroutinen
TIMERB0_ISR(ISR_Timer_B_CCR0)    //int13 | 0xFFFA | Timer B CC0

__interrupt void ISR_Timer_B_CCR0 (void) {
	// Wert setzen um eine Wandlung im Haubtprogramm auszuloesen
	do_a_barrel_roll = 1;
	
	// Interruptflag entfernen um eine Wiederholung der ISR, wegen
	// gesetzem Interruptflag, zu verhindern
	TBCCTL0 &= ~CCIFG; 
}
