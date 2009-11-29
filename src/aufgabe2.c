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
#define LED_OFF(led)	    (P4OUT |= led)    
#define LED_ON(led)      	(P4OUT &= ~led)    
#define LED_TOGGLE(led)  	(P4OUT ^=  led)

void aufgabe2() {
    // Input Maskieren um nur die beinen Taster zu erfassen
	switch (P1IN & 0x03) {
		case 0x00 : // kein Taster gedrueckt oder beide gedrueckt
		case 0x03 : // schlate nur gelbe LED ein
			LED_ON(YELLOW);
			LED_OFF(RED);
			LED_OFF(GREEN);
		break; 
		case 0x01 :  // rechter Taster gedrckt schalte nur gruene LED ein
			LED_OFF(YELLOW);
			LED_OFF(RED);
			LED_ON(GREEN);
		break;
		case 0x02 : // linker Taster gedrueckt schalte nur rote LED ein
			LED_OFF(YELLOW);
			LED_ON(RED);
			LED_OFF(GREEN);
		break;
		default : // dieser Fall sollte nicht eintreten fuer den Fall das doch,
                  // sollen alle LEDs leuchten
			LED_ON(YELLOW);
			LED_ON(RED);
			LED_ON(GREEN);
	}
}
