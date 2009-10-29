#include "msp430x16x.h"		// Systemdefinitionen von TI für den MSP430F1612
#include "init.h"			// Initialisierung des Mikrocontrollers
#include "CC1100.h"			// CC1100 Funktransceiver
#include "system.h"			// Systemfunktionen MSB430H
#include "interrupts.h"		// ISR - Interrupt Service Routinen
#include "stdio.h"			// includes TI MSP430F1612 
#include "SHT11.h"			// SHT11 Temperatur- und Feuchtesensor

#define RED					(0x01)
#define YELLOW				(0x02)
#define GREEN				(0x04)
#define LED_OFF(led)	    (P4OUT |= led)    
#define LED_ON(led)      	(P4OUT &= ~led)    
#define LED_TOGGLE(led)  	(P4OUT ^=  led)

void aufgabe2() {
	switch (P1IN & 0x03) {
		case 0x00 : // kein Taster gedrückt oder beide gedrückt
		case 0x03 :
			LED_ON(YELLOW);
			LED_OFF(RED);
			LED_OFF(GREEN);
		break; 
		case 0x01 :  // rechter Taster gedrückt
			LED_OFF(YELLOW);
			LED_OFF(RED);
			LED_ON(GREEN);
		break;
		case 0x02 : // linker Taster gedrückt
			LED_OFF(YELLOW);
			LED_ON(RED);
			LED_OFF(GREEN);
		break;
		default :
			LED_ON(YELLOW);
			LED_ON(RED);
			LED_ON(GREEN);
	}
}
