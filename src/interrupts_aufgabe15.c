#include "msp430x16x.h"	// Systemdefinitionen von TI fur den MSP430F1612
#include "interrupts.h"	// ISR - Interrupt Service Routinen
#include "system.h"		// Systemfunktionen MSB430H
#include "CC1100.h"		// CC1100 Funktransceiver
#include "stdio.h"		// includes TI MSP430F1612
#include "aufgabe15.h"	// Headerdatei des Haubtprogramms

//Zuweisung der Interruptvektoren auf die ISR Behandlungsroutinen
PORT1_ISR(ISR_Port1)    //int04 | 0xFFE8 | Porrt1
WDT_ISR(ISR_Watchdog)   //int10 | 0xFFF4 | Watchdog Timer

__interrupt void ISR_Port1 (void) { 
	if(lpm_status==4) {	// Wenn wakeup durch Schalterdruck angefordert
		LPM4_EXIT;		// LPM4 beenden und in aktiven Modus wechseln
		tick=0;			// Tick zuruecksetzen
		lpm_status=-1;	// Status Variable auf aktiven Modues setzen
		// Watchog rauslassen
		WDTCTL = (WDTPW+WDTTMSEL+WDTCNTCL+WDTSSEL);
	}
	else {
		//Wenn kein wakeup counter zurucksetzen
		tick=0;
	}
	// Alle Interuptflags fuer Port 1 entfernen um eine Wiederholung
	// der ISR, wegen gesetzem Interuptfalg, zu verhindern
	CLEAR(P1IFG, 0xFF);
}

__interrupt void ISR_Watchdog (void) {
	// tick hochsetzen
	tick++;
	// gruene LED toggeln
	P4OUT ^= 0x04;

	// nach 60 sec im aktivemode fordere LPM4 an
	// und schalte gruene LED aus
	if ((lpm_status == -1) && (tick > 60)) {
		lpm_status=4;
		P4OUT |= 0x04;
	}
	// Das Watchdog Interuptflag entfernen um eine Wiederholung
	// der ISR, wegen gesetzem Interuptfalg, zu verhindern
	CLEAR(IFG1, WDTIFG);
}
