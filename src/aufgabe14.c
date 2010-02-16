#include "msp430x16x.h"		// Systemdefinitionen von TI fur den MSP430F1612
#include "init.h"			// Initialisierung des Mikrocontrollers
#include "CC1100.h"			// CC1100 Funktransceiver
#include "system.h"			// Systemfunktionen MSB430H
#include "interrupts.h"		// ISR - Interrupt Service Routinen
#include "stdio.h"			// includes TI MSP430F1612 
#include "SHT11.h"			// SHT11 Temperatur- und Feuchtesensor

void aufgabe15() {
	// Interrupts allgemein zulassen
	_bis_SR_register(GIE);
	// Interrupts fuer Schalter eins einschalten
	P1IE |= 0x02;
	// Interrupt bei low to high Flanke ausloesen
	P1IES = 0x00;
	
	while(1) {
		// in LPM4 gehen
        LPM4;
	}

}
