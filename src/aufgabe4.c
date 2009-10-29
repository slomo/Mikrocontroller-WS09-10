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

void delay(unsigned int time_mill) {	
	unsigned int i;
	for(i=0;i<=time_mill;++i){
		wait(100);
	}
}

void ausgabe(int zahler){
	int out = 0;
	out = zahler & 0x02;
	out |= (zahler & 0x01) << 2;
	out |= (zahler & 0x04) >> 2;
	
	P4OUT = ~ out;  
}

int unsigned i;

void aufgabe4() {
	
	if(P1IN & 0x03){
		switch (P1IN & 0x03) {
			case 0x00 : // kein Taster gedrückt
			break;
			case 0x01 :  // rechter Taster gedrückt
				i = (i - 1) % 8;
			break;
			case 0x02 : // linker Taster gedrückt
				i = (i + 1) % 8;
			break;
			case 0x03 : //beide gedrückt
				i = 0;
			break;
			default :
		}
		
		ausgabe(i);
		delay(100);
	}
}
