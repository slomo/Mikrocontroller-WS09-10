#include "msp430x16x.h"		// Systemdefinitionen von TI fr den MSP430F1612
#include "init.h"			// Initialisierung des Mikrocontrollers
#include "CC1100.h"			// CC1100 Funktransceiver
#include "system.h"			// Systemfunktionen MSB430H
#include "interrupts.h"		// ISR - Interrupt Service Routinen
#include "stdio.h"			// includes TI MSP430F1612 
#include "SHT11.h"			// SHT11 Temperatur- und Feuchtesensor

// Definitionen um das Ein- und Ausschalten von LEDs zu erleichtern
#define RED					(0x01)
#define YELLOW				(0x02)
#define GREEN				(0x04)
#define LED_OFF(led)	    (P4OUT |= led)    
#define LED_ON(led)      	(P4OUT &= ~led)    
#define LED_TOGGLE(led)  	(P4OUT ^=  led)


// Funtion, welche die bereits vorhandene Funktion wait verwendet
// um den Programmfluss zu um n Millisekunden zu verzgern
void delay(unsigned int time_mill) {	
	unsigned int i;
	for(i=0;i<=time_mill;++i){
		wait(100);
	}
}

void aufgabe1() {
    // Ampel auf rot schalten und warten
	LED_ON(RED);
	delay(3000);

    // Ampel auf gelb-rot schalten und warten
	LED_ON(YELLOW);
	delay(1000);

    // Ampel auf grn schalten und warten
	LED_OFF(YELLOW);
	LED_OFF(RED);
	LED_ON(GREEN);
	delay(3000);

    // Ampel auf gelb schalten und warten
	LED_OFF(GREEN);
	LED_ON(YELLOW);
	delay(1000);
    
    // Ampel fr den nchsten Durchlauf zurcksetzen
	LED_OFF(YELLOW);
}
