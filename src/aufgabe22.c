#include "msp430x16x.h"		// Systemdefinitionen von TI fur den MSP430F1612
#include "init.h"			// Initialisierung des Mikrocontrollers
#include "CC1100.h"			// CC1100 Funktransceiver
#include "system.h"			// Systemfunktionen MSB430H
#include "interrupts.h"		// ISR - Interrupt Service Routinen
#include "stdio.h"			// includes TI MSP430F1612 
#include "SHT11.h"			// SHT11 Temperatur- und Feuchtesensor
#include "aufgabe22.h"

// Zaehler fuer die laenge der Zeichenkette
int count=0;

void aufgabe22() {
	// Interrupts von UART1 erlauben
	IE2 |= URXIE1;
	//Interrupts allgemein zulassen
	_bis_SR_register(GIE);
	while(1){
	}
}
