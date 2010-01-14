#include "msp430x16x.h"	// Systemdefinitionen von TI fur den MSP430F1612
#include "interrupts.h"	// ISR - Interrupt Service Routinen
#include "system.h"		// Systemfunktionen MSB430H
#include "CC1100.h"		// CC1100 Funktransceiver
#include "stdio.h"		// includes TI MSP430F1612
#include "aufgabe22.h"	// Headerdatei des Haubtprogramms
#include "string.h"		// Header fuer die Manipulation von Strings

//Zuweisung der Interruptvektoren auf die ISR Behandlungsroutinen
USART1RX_ISR(ISR_USART1_Receive) //int03 | 0xFFE6 | USART1 Receive

__interrupt void ISR_USART1_Receive (void) {
	// empfangenes Zeichen welches den Interrupt ausgelöst hat
	unsigned char receivedChar;
	// Buffer in dem die Ausgabe zwischengespeichert wird
	char str[32];

	// Empfangs Interrupt fuer UART1 deaktivieren um eine Unterbrechung
	// der ISR durch ein weiteres Zeichen zu verhindern
	IE2 &= ~URXIE1;
	// empfangenes Zeichen abholen
	receivedChar = U1RXBUF;
 	if (receivedChar != '\n') {
		// Zeichen zurueckschicken das es im Terminal erscheint
		U1TXBUF = receivedChar;
		// Zähler fuer die Anzahl der empfangenen Zeichen erhoehen
		count++
	} 
	else {
		// Zeichenkette mit der Menge der uebertragnen Zeichen
		// erstellen
		sprintf(str,"\r\n%d\r\n",count-1);
		// und ausgeben
		writestr(str);
		// Zaehler wieder auf 0 zuruecksetzen
		count=0;
	}
	// Empfangs Interrupt fuer UART1 wieder aktivieren um eine den
	// Empfang weiterer Zeichen zu ermöglichen
	IE2 |= URXIE1;
}
