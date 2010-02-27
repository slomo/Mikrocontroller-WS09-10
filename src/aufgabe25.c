#include "msp430x16x.h"		// Systemdefinitionen von TI fur den MSP430F1612
#include "init.h"			// Initialisierung des Mikrocontrollers
#include "CC1100.h"			// CC1100 Funktransceiver
#include "system.h"			// Systemfunktionen MSB430H
#include "interrupts.h"		// ISR - Interrupt Service Routinen
#include "stdio.h"			// includes TI MSP430F1612 
#include "SHT11.h"			// SHT11 Temperatur- und Feuchtesensor
#include "aufgabe25.h"
#include <math.h>

#define PI 3.14

// Array welches Ausgabewerte enthaelt
int values[100];
int i;

void aufgabe25() {
	
	//Sinuswerte fur Ausgabe berechnen
	int i;
	for(i=0;i<100;i++) {
		values[i]=(sin(i*PI/50.0)+1.5)*4096/3;
	}
	
	// DAC vorbereiten
	DAC12_1CTL = DAC12SREF0 + DAC12SREF1 + DAC12IR + DAC12AMP1;
	
	// Timer und Output einrichten
	P6SEL = 0x80;
	TBCTL 	= MC_1 + TASSEL_2 + ID_3;
	TBCCTL0 = CCIE; 
	TBCCR0 	= 91;
	
    // Interrupts zulassen
	_bis_SR_register(GIE);
	
	while(1) {
	}

}
