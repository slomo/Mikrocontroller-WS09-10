#include "msp430x16x.h"	// Systemdefinitionen von TI fur den MSP430F1612
#include "interrupts.h"	// ISR - Interrupt Service Routinen
#include "system.h"		// Systemfunktionen MSB430H
#include "CC1100.h"		// CC1100 Funktransceiver
#include "stdio.h"		// includes TI MSP430F1612
#include "aufgabe19.h"	// Headerdatei des Haubtprogramms

//Zuweisung der Interruptvektoren auf die ISR Behandlungsroutinen
TIMERB0_ISR(ISR_Timer_B_CCR0)     //int13 | 0xFFFA | Timer B CC0

__interrupt void ISR_Timer_B_CCR0 (void) { 
	// Feststellen welche Werte im timestruct der aktuellen Zeit
	// geaendert werden muessen
	if (now.s==10) { // eine Sekunde ist vergangen
		now.s=0;
		now.ss++;
		// Sekundentakt durch toggeln der roten LED veranschaulichen
		P4OUT ^= 0x01;
		if (now.ss==60) { // eine Minute ist vergangen
			now.ss=0;
			now.mm++;
			if (now.mm==60) { // eine Stunde ist vergangen
				now.mm=0;
				now.hh++;
				if(now.hh==24) { // ein Tag ist vergangen
					now.hh=0;
				}
			}
		}
	}
	else { // eine Zentelsekunde ist Vergangen
		now.s++;
	}

	// Wenn die aktuelle Zeit mit der ersten Schaltzeit uebereinstimmt,
	// schalte die gruene LED ein
	if (timecmp(&now,&t1)) {
		P4OUT &= ~0x04;
	}
	// Wenn die aktuelle Zeit mit der zweiten Schaltzeit uebereinstimmt,
	// schalte die gruene LED aus
	if (timecmp(&now,&t2)) {
		P4OUT |= 0x04;
	}

	// Interruptflag entfernen um eine Wiederholung der ISR, wegen
	// gesetzem Interruptflag, zu verhindern
	TBCCTL0 &= ~CCIFG;
}
