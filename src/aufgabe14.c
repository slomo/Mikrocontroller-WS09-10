// Standart Headerfiles siehe aufgabe1.c

void aufgabe15() {
	// Interrupts allgemein zulassen
	_bis_SR_register(GIE);
	// Interrupts fuer Schalter eins einschalten
	P1IE |= 0x02;
	// Interrupt bei low to high Flanke ausloesen
	P1IES = 0x00;
	
	while(1) {
		// in LPM4 gehen
        LPM4;
	}

}
