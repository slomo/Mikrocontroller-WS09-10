// alle StandartHeaderfiles (siehe Aufgabe 12) einbinden
#include "aufgabe16.h"	// Headerdatei des Haubtprogramms

//Zuweisung der Interruptvektoren auf die ISR Behandlungsroutinen
PORT1_ISR(ISR_Port1)    //int04 | 0xFFE8 | Port1

__interrupt void ISR_Port1 (void) {
	 
	if(lpm_status==4) {	// es handelt sich um einen wakeup
		LPM4_EXIT;		// LPM4 beenden und in aktiven Modus wechseln
		lpm_status=-1;	// Status Variable auf aktiven Modus setzen
		P1IES = 0xff;	// auf high-low Flanke reagieren
		
	}
	else {	// andernfalls wieder schlafen legen
		// LPM4 anfordern
		lpm_status=4;
		// auf low-high Flanke reagiern
		P1IES = 0x0;
	}
	// Alle Interruptflags fuer Port 1 entfernen um eine Wiederholung
	// der ISR, wegen gesetzem Interruptflag, zu verhindern
	CLEAR(P1IFG, 0xFF);
}
