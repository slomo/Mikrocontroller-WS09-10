#include "msp430x16x.h"		// Systemdefinitionen von TI fur den MSP430F1612
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
#define LED_OFF(led)		(P4OUT |= led)    
#define LED_ON(led)			(P4OUT &= ~led)    
#define LED_TOGGLE(led)		(P4OUT ^=  led)

// Funtion, welche die bereits vorhandene Funktion wait verwendet
// um den Programmfluss zu um n Millisekunden zu verzoegern
void delay(unsigned int time_mill) {
	unsigned int i;
	for(i=0;i<=time_mill;++i){
		wait(100);
	}
}

// Nummer des angeforderten LPM Modus
int lpm_status = 4;

// Diese Funktion erzeugt eine Ampelsequenz
void ampel() {
		LED_ON(YELLOW);
		delay(200);
		LED_OFF(YELLOW);
		LED_ON(RED);
		delay(300);
		LED_ON(YELLOW);
		delay(200);
		LED_OFF(RED);
		LED_OFF(YELLOW);
		LED_ON(GREEN);
		delay(300);
		LED_OFF(GREEN);
}

void aufgabe16() {
	//Interrupts allgemein zulassen
	_bis_SR_register(GIE);
	//Interrupts fuer Schalter eins einschalten
	P1IE |= 0x02;
	// Interrupt bei low to high Flanke ausloesen
	P1IES = 0x00;
	// in LPM4 wechseln
	LPM4;
	while(1){
		// wenn wir uns im aktiven Modus befinden zeige eine Ampel
		// Sequenz
		if(lpm_status==-1){
			ampel();
		}
		// Wenn lpm angefordert ist...
		if(lpm_status==4){
			// ...alle LEDs ausschalten
			P4OUT |= 0x7;
			// und in LMP4 wechseln
			LPM4;
		}
	}
}
