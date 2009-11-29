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

int lpm_status;	//Nummer des LPM Modus
int tick;		//Counter fur laenge des Schalterdrucks

void delay(unsigned int time_mill) {	
	unsigned int i;
	for(i=0;i<=time_mill;++i){
		wait(100);
	}
}

void aufgabe15() {
	lpm_status=4;	//lpm_status initial auf 4 setzen
	tick=0;			//tick auf null setzen
	_bis_SR_register(GIE); //Interrupts zulassen
	P1IE |= 0x02;	//Interrupts fur Schalter eins einschalten
	P1IES = 0x00;	//
	IE1 |= WDTIE;	//Watchdog Interrupt aktivieren
	while(1) {
		if (lpm_status == -1) { //Wenn Aktiver Modues schalte LED ein
			LED_ON(RED);
		}
		else {					//Wenn LPM4 angefordert LEDs abschalten und in LPM4 gehen
			LED_OFF(RED);
			LED_OFF(GREEN);
			LPM4;
		}
		while(P1IN & 0x02) {	//Wenn Schalter 2sec gedrieck LPM4 anfordern
			if(tick>2) {
				lpm_status=4;
				break;
			}
			wait(100);
		}
	}

}
