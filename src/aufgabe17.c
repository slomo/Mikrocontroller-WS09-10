#include "msp430x16x.h"		// Systemdefinitionen von TI fur den MSP430F1612
#include "init.h"			// Initialisierung des Mikrocontrollers
#include "CC1100.h"			// CC1100 Funktransceiver
#include "system.h"			// Systemfunktionen MSB430H
#include "interrupts.h"		// ISR - Interrupt Service Routinen
#include "stdio.h"			// includes TI MSP430F1612 
#include "SHT11.h"			// SHT11 Temperatur- und Feuchtesensor

void aufgabe17() {
	// Timer soll hochzaehlen und den ACLK Takt als Quelle verwenden
	TBCTL = MC_1 + TBSSEL_1;
	// Interrupts fuer Timer B aktivieren
	TBCCTL0 = CCIE; 
	// Timerinterrupt nach 32000 Takten auslösen
	// das fuehrt bei 32kHz Takt zu einem Interupt pro Sekunde
	TBCCR0 = 32000; //bei 32kHz
	
	//Interrupts allgemein zulassen
	_bis_SR_register(GIE); 
	while(1){
	}
}
