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

int lpm_status = 4;	//Nummer des LPM Modus
	//Counter fur laenge des Schalterdrucks

void delay(unsigned int time_mill) {	
	unsigned int i;
	for(i=0;i<=time_mill;++i){
		wait(100);
	}
}

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
	_bis_SR_register(GIE); //Interrupts zulassen
	P1IE |= 0x02;	//Interrupts fur Schalter eins einschalten
	P1IES = 0x00;	//
	LPM4;
	while(1){
		
		if(lpm_status==-1){
			ampel();
		}
		if(lpm_status==4){
		
			P4OUT |= 0x7; 	// alle LEDs aus machen
			LPM4;
		}
		
	}
}
