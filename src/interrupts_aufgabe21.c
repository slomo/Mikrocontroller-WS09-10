// alle StandartHeaderfiles (siehe Aufgabe 12) einbinden
#include "aufgabe21.h"	// Headerdatei des Haubtprogramms

//Zuweisung der Interruptvektoren auf die ISR Behandlungsroutinen
TIMERB0_ISR(ISR_Timer_B_CCR0)     //int13 | 0xFFFA | Timer B CC0

__interrupt void ISR_Timer_B_CCR0 (void) { 
	// Feststellen welche Werte im timestruct der aktuellen Zeit
	// geaendert werden muessen
	if (now.s==10) { // eine Sekunde ist vergangen
		now.s=0;
		now.ss++;
		// im Sekundentakt Uhrzeit und Werte des SHT11 ausgeben
		output_data();
		if (now.ss==60) { // eine Minute ist vergangen
			now.ss=0;
			now.mm++;
			if (now.mm==60) { // eine Stunde ist vergangen
				now.mm=0;
				now.hh++;
				if(now.hh==24) { // ein Tag ist vergangen
					now.hh=0;
				}
			}
		}
	}
	else { // eine Zentelsekunde ist Vergangen
		now.s++;
	}

	// Interruptflag entfernen um eine Wiederholung der ISR, wegen
	// gesetzem Interruptflag, zu verhindern
	TBCCTL0 &= ~CCIFG;
}
