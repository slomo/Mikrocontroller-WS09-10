#include "msp430x16x.h"		// Systemdefinitionen von TI fur den MSP430F1612
#include "init.h"			// Initialisierung des Mikrocontrollers
#include "CC1100.h"			// CC1100 Funktransceiver
#include "system.h"			// Systemfunktionen MSB430H
#include "interrupts.h"		// ISR - Interrupt Service Routinen
#include "stdio.h"			// includes TI MSP430F1612 
#include "SHT11.h"			// SHT11 Temperatur- und Feuchtesensor
#include "aufgabe18.h"

void aufgabe18() {
	// Timer soll hochzaehlen und den ACLK Takt mit 8fachem Teiler 
	// als Quelle verwenden
	TBCTL 	= MC_1 + TBSSEL_1 + ID0 + ID1;
	// Interrupts fuer Timer B aktivieren
	TBCCTL0 = CCIE; 
	// Timerinterrupt nach 4000 mal der gewuenschten Verzoegerung in
	// Sekunden Takten auslösen (32000kHz/8fachen Teiler = 4000)
	// LEDDELAY ist in aufgabe18.h definiert
	TBCCR0 	= 4000*LEDDELAY;
	
	// Interrupts allgemein zulassen
	_bis_SR_register(GIE);
	
	// sobald die Moeglichkeit besteht in den LPM3 wechseln
	while(1){	
		LPM3;
	}
}
