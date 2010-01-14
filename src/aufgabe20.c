#include "msp430x16x.h"		// Systemdefinitionen von TI fur den MSP430F1612
#include "init.h"			// Initialisierung des Mikrocontrollers
#include "CC1100.h"			// CC1100 Funktransceiver
#include "system.h"			// Systemfunktionen MSB430H
#include "interrupts.h"		// ISR - Interrupt Service Routinen
#include "stdio.h"			// includes TI MSP430F1612 
#include "SHT11.h"			// SHT11 Temperatur- und Feuchtesensor
#include "aufgabe20.h"

void aufgabe20() {
	// Zur Initialisierung des UART1 muss die Funktion initUART() in der
	// main einkommentiert werden
	while(1){
		// Wenn der Sendepuffer leer ist ...
		if (U1TCTL & TXEPT) {
			// Schreibe ein '?' in den Sendepuffer, dass es ueber UART1
			// ausgegeben wird
			U1TXBUF = '?';
		}
	}
}
