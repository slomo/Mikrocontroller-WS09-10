// Standart Headerfiles siehe aufgabe1.c

void aufgabe17() {
	// Timer soll hochzaehlen und den ACLK Takt als Quelle verwenden
	TBCTL = MC_1 + TBSSEL_1;
	// Interrupts fuer Timer B aktivieren
	TBCCTL0 = CCIE; 
	// Timerinterrupt nach 32000 Takten auslösen
	// das fuehrt bei 32kHz Takt zu einem Interrupt pro Sekunde
	TBCCR0 = 32000; //bei 32kHz
	
	//Interrupts allgemein zulassen
	_bis_SR_register(GIE); 
	while(1){
	}
}
