// Standart Headerfiles siehe aufgabe1.c

// Funtion, welche die bereits vorhandene Funktion wait verwendet
// um den Programmfluss zu um n Millisekunden zu verzoegern
void delay(unsigned int time_mill) {	
	unsigned int i;
	for(i=0;i<=time_mill;++i){
		wait(100);
	}
}

// Funktion um die Ausgabe von Zahlen in bin form ueber die LEDs zu erleichtern
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

//Globaler Zaehler
int unsigned i;

void aufgabe4() {
	
    // Wenn ein Taster gedrueckt wird
	if(P1IN & 0x03){
		switch (P1IN & 0x03) {
			case 0x00 : // kein Taster gedrueckt
			break;
			case 0x01 :  // rechter Taster gedrueckt decrementiere
				i = (i - 1) % 8;
			break;
			case 0x02 : // linker Taster gedrueckt incrementiere
				i = (i + 1) % 8;
			break;
			case 0x03 : //beide gedrueckt setze Zaehler auf 0
				i = 0;
			break;
			default :
		}
		
		ausgabe(i);

        // Warte eine kurze Zeit nach der Ausgabe um Prellverhalten
        // vom Taster abzufangen
		delay(100);
	}
}
