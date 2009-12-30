#include "msp430x16x.h"		// Systemdefinitionen von TI fur den MSP430F1612
#include "init.h"			// Initialisierung des Mikrocontrollers
#include "CC1100.h"			// CC1100 Funktransceiver
#include "system.h"			// Systemfunktionen MSB430H
#include "interrupts.h"		// ISR - Interrupt Service Routinen
#include "stdio.h"			// includes TI MSP430F1612 
#include "SHT11.h"			// SHT11 Temperatur- und Feuchtesensor

// Definitionen um das Ein- und Ausschalten von LEDs zu erleichtern
#define RED					(0x01)
#define GREEN				(0x04)
#define LED_OFF(led)		(P4OUT |= led)    
#define LED_ON(led)			(P4OUT &= ~led)    

int lpm_status;	// Nummer des angeforderten LPM Modus
int tick=0;		// Counter fur laenge des Schalterdrucks

void aufgabe15() {
	// lpm_status initial auf 4 setzen, das bedeutet, das der Controller
	// nach der Initialisierung in den LPM geht
	lpm_status=4;
	
	// Interrupts allgemein zulassen
	_bis_SR_register(GIE);
	// Interrupts fuer Schalter eins einschalten
	P1IE |= 0x02;
	// Interupt bei low to high Flanke ausloesen
	P1IES = 0x00;
	// Watchdog Interrupt aktivieren
	IE1 |= WDTIE;
	
	while(1) {
		// Wenn Aktiver Modus aktiv ist, schalte LED ein
		if (lpm_status == -1) { 
			LED_ON(RED);
		}
		// Wenn LPM4 angefordert LEDs abschalten und in LPM4 gehen
		else {
			LED_OFF(RED);
			LED_OFF(GREEN);
			LPM4;
		}
		//Wenn Schalter 2sec gedrueckt LPM4 anfordern
		while(P1IN & 0x02) {
			if(tick>2) {
				lpm_status=4;
				break;
			}
			// kurz warten um Prellverhalten abzufangen
			wait(100);
		}
	}

}
