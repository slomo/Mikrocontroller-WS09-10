#include "msp430x16x.h"		// Systemdefinitionen von TI fur den MSP430F1612
#include "init.h"			// Initialisierung des Mikrocontrollers
#include "CC1100.h"			// CC1100 Funktransceiver
#include "system.h"			// Systemfunktionen MSB430H
#include "interrupts.h"		// ISR - Interrupt Service Routinen
#include "stdio.h"			// includes TI MSP430F1612 
#include "SHT11.h"			// SHT11 Temperatur- und Feuchtesensor
#include "aufgabe26.h"
#include <math.h>

#define RED					(0x01)
#define YELLOW				(0x02)
#define GREEN				(0x04)
#define LED_OFF(led)	    (P4OUT |= led)    
#define LED_ON(led)      	(P4OUT &= ~led)    
#define LED_TOGGLE(led)  	(P4OUT ^=  led)

#define PI 3.14

int values[100];
int i;

void aufgabe26() {
	
	//Sinuswerte fur Ausgabe berechnen
	int i;
	for(i=0;i<100;i++) {
		values[i]=(sin(i*PI/50.0)+1.5)*4096/3;
	}
	
	//DMA vorbereiten
	DMACTL0 = DMA0TSEL_8;
	DMA0CTL = DMASRCINCR_3 + DMADT_4 + DMAEN;
	DMA0SZ = 100;
	DMA0SA = (unsigned int) values;
	DMA0DA = (unsigned int ) &DAC12_1DAT;
	
	//DAC vorbereiten
	DAC12_1CTL = DAC12SREF0 + DAC12SREF1 + DAC12IR + DAC12AMP1;
	
	//Timer und output
	P6SEL = 0x80;
	TBCTL 	= MC_1 + TASSEL_2 + ID_3;
	//TBCCTL0 = CCIE; 
	TBCCR0 	= 93;
	
	_bis_SR_register(GIE); //Interrupts zulassen
	
	while(1) {
	}

}
