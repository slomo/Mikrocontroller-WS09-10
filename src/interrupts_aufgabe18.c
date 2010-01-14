#include "msp430x16x.h"	// Systemdefinitionen von TI fur den MSP430F1612
#include "interrupts.h"	// ISR - Interrupt Service Routinen
#include "system.h"		// Systemfunktionen MSB430H
#include "CC1100.h"		// CC1100 Funktransceiver
#include "stdio.h"		// includes TI MSP430F1612
#include "aufgabe18.h"	// Headerdatei des Haubtprogramms

//Zuweisung der Interruptvektoren auf die ISR Behandlungsroutinen
TIMERB0_ISR(ISR_Timer_B_CCR0)     //int13 | 0xFFFA | Timer B CC0

__interrupt void ISR_Timer_B_CCR0 (void) { 
	// Wenn die LED aus ist, schalte sie an und den Timer so um, dass
	// ein 0,5 sec langes Leuchten erzeugt werden kann
	if (P4OUT & 0x01) {
		// LED einschalten
		P4OUT &= ~0x01;
		// Timer auf 0,5 sec setzen (32000kHz/8/2 = 2000)
		TBCCR0 = 2000;
	}
	else {
		// LED auschalten
		P4OUT |= 0x01;
		// Timer wieder auf den alten Delaywert stellen
		// LEDDELAY ist in aufgabe18.h definiert
		TBCCR0 = 4000*LEDDELAY;
	}

	// Interruptflag entfernen um eine Wiederholung der ISR, wegen
	// gesetzem Interruptflag, zu verhindern
	TBCCTL0 &= ~CCIFG;
}	
