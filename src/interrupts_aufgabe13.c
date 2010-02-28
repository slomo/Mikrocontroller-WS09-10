// alle StandartHeaderfiles (siehe Aufgabe 12) einbinden

//Zuweisung der Interruptvektoren auf die ISR Behandlungsroutinen
PORT1_ISR(ISR_Port1)    //int04 | 0xFFE8 | Port1
WDT_ISR(ISR_Watchdog)   //int10 | 0xFFF4 | Watchdog Timer

__interrupt void ISR_Port1 (void) { 
	switch (P1IFG & 0x03) {
		case 0x01 :  // rechter Taster gedruckt
			// gelbe LED ausschalten
			P4OUT |= 0x02;
			// Watchdogtimer zuruecksetzen
			WDTCTL = (WDTPW+WDTTMSEL+WDTCNTCL+WDTSSEL);
			// kurz warten um eventuelles Prellverhalten des Tasters
			// abzufangen
			wait(100);
		break;
		case 0x02 : // linker Taster gedruckt tue nichts
		break;
	}
	// Alle Interruptflags fuer Port 1 entfernen um eine Wiederholung
	// der ISR, wegen gesetzem Interruptflag, zu verhindern
	CLEAR(P1IFG, 0xFF);

}

__interrupt void ISR_Watchdog (void) {
	// Wenn die gelbe LED aus ist also in P4OUT.1 eine 1 steht...
	if (P4OUT & 0x02) {
		// ...schalte die gelbe LED ein
		P4OUT &= ~0x02;
	}
	else {
		// ... anderfalls Watchdog in den Zwinger sperren
		WDTCTL = WDTPW | WDTHOLD;
		// under eine Ampelsequenz in einer Endlosschleife Anzeigen
		ampel();
	}
	
	// Das Watchdog Interruptflag entfernen um eine Wiederholung
	// der ISR, wegen gesetzem Interruptflag, zu verhindern
	CLEAR(IFG1, WDTIFG);
}
