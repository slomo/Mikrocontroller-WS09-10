#include "msp430x16x.h"	// Systemdefinitionen von TI für den MSP430F1612
#include "../init.h"	// Initialisierung des Mikrocontrollers
#include "../CC1100.h"	// CC1100 Funktransceiver
#include "../system.h"	// Systemfunktionen MSB430H
#include "interrupts.h"	// ISR - Interrupt Service Routinen
#include "stdio.h"		// includes TI MSP430F1612 
#include "../SHT11.h"	// SHT11 Temperatur- und Feuchtesensor
#include "project.h"

main(void) {

	// Initialisierung der Port Register
	initPort();
	
	// Dco Taktquelle aktivieren mit 7.3728MHz
	DCO ();
	
	// UART-RS232 mit 115.2kBit/s initialisieren
	initUART1();

	while(1) {
		project();
	}

}


