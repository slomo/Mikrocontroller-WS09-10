// Standart Headerfiles siehe aufgabe1.c
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
