#include "msp430x16x.h"		// Systemdefinitionen von TI für den MSP430F1612
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
// um den Programmfluss zu um n Millisekunden zu verzögern
void delay(unsigned int time_mill) {	
	unsigned int i;
	for(i=0;i<=time_mill;++i){
		wait(100);
	}
}

// Funktion um die Ausgabe von Zahlen in Binärer form über die LEDs zu erleichtern
void ausgabe(int zahler){
    //Ausgabe Varriable Nullen
	int out = 0;

    // Mittleres der 3 bits setzen
	out = zahler & 0x02; 

    // erstes Bit an 3. Stelle setzen
	out |= (zahler & 0x01) << 2;

    // drittes Bit an 1. Stelle setzen
	out |= (zahler & 0x04) >> 2;
	
    // erstellte Bitfolge unter der Verwendung von xor in den Ausgang Schreiben
	P4OUT = ~ out;  
}

//Globaler Zähler
int unsigned i;

void aufgabe4() {
	
    // Wenn ein Taster gedrückt wird
	if(P1IN & 0x03){
		switch (P1IN & 0x03) {
			case 0x00 : // kein Taster gedrückt
			break;
			case 0x01 :  // rechter Taster gedrückt decrementiere
				i = (i - 1) % 8;
			break;
			case 0x02 : // linker Taster gedrückt incrementiere 
				i = (i + 1) % 8;
			break;
			case 0x03 : //beide gedrückt setze Zähler auf 0
				i = 0;
			break;
			default :
		}
		
		ausgabe(i);

        // Warte eine kurze Zeit nach der Ausgabe um Prellverhalten vom Taster abzufangen
		delay(100);
	}
}
