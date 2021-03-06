#include "msp430x16x.h"		// Systemdefinitionen von TI f�r den MSP430F1612
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
// um den Programmfluss zu um n Millisekunden zu verz�gern
void delay(unsigned int time_mill) {	
	unsigned int i;
	for(i=0;i<=time_mill;++i){
		wait(100);
	}
}

void aufgabe3() {
    // wenn einer der beinen Taster gedr�ckt wird, starte einen Ampelzyklus
	if(P1IN & 0x03) {
		LED_ON(YELLOW);
		delay(2000);
		LED_OFF(YELLOW);
		LED_ON(RED);
		delay(3000);
		LED_ON(YELLOW);
		delay(2000);
		LED_OFF(RED);
		LED_OFF(YELLOW);
		LED_ON(GREEN);
		delay(3000);
		LED_OFF(GREEN);
        //Ampel ist wieder aus und es kann 4 sec lang kein neuer Zyklus gestartet werden
		delay(4000);
	}
}
