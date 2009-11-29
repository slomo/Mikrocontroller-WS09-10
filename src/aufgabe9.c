#include "msp430x16x.h"		// Systemdefinitionen von TI fur den MSP430F1612
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

void delay(unsigned int time_mill) {	
	unsigned int i;
	for(i=0;i<=time_mill;++i){
		wait(100);
	}
}

void aufgabe9() {
	//Wachund rauslassen
	BCSCTL1	|= DIVA0;
	BCSCTL1	|= DIVA1;
	WDTCTL = (WDTPW+WDTCNTCL+WDTSSEL);
	//WDTCTL = ((WDTCTL & ~0xff00) | WDTPW | WDTSSEL) & ~WDTHOLD & ~WDTIS0 & ~WDTIS1;	
	while(1){
		delay(500);
		LED_OFF(RED);
		//WDTCTL = (WDTCTL & ~0xff00) | WDTPW | WDTCNTCL;	
		WDTCTL = (WDTPW+WDTCNTCL+WDTSSEL);
		delay(500);
		LED_ON(RED);
	}
}
