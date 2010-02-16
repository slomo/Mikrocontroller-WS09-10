#include "msp430x16x.h"	// Systemdefinitionen von TI fur den MSP430F1612
#include "interrupts.h"	// ISR - Interrupt Service Routinen
#include "system.h"		// Systemfunktionen MSB430H
#include "CC1100.h"		// CC1100 Funktransceiver
#include "stdio.h"		// includes TI MSP430F1612
#include "aufgabe15.h"	// Headerdatei des Haubtprogramms

//Zuweisung der Interruptvektoren auf die ISR Behandlungsroutinen
PORT1_ISR(ISR_Port1)    //int04 | 0xFFE8 | Port1

// Funtion, welche die bereits vorhandene Funktion wait verwendet
// um den Programmfluss zu um n Millisekunden zu verzoegern
void delay(unsigned int time_mill) {	
	unsigned int i;
	for(i=0;i<=time_mill;++i){
		wait(100);
	}
}

__interrupt void ISR_Port1 (void) { 
	LPM4_EXIT;		// LPM4 beenden und in aktiven Modus wechseln

	delay(10000);

	// Alle Interruptflags fuer Port 1 entfernen um eine Wiederholung
	// der ISR, wegen gesetzem Interruptflag, zu verhindern
	CLEAR(P1IFG, 0xFF);
}