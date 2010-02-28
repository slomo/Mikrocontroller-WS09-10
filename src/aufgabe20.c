// Standart Headerfiles siehe aufgabe1.c
#include "aufgabe20.h"

void aufgabe20() {
	// Zur Initialisierung des UART1 muss die Funktion initUART() in der
	// main einkommentiert werden
	while(1){
		// Wenn der Sendepuffer leer ist ...
		if (U1TCTL & TXEPT) {
			// Schreibe ein '?' in den Sendepuffer, dass es ueber UART1
			// ausgegeben wird
			U1TXBUF = '?';
		}
	}
}
