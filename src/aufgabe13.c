// Standart Headerfiles siehe aufgabe1.c

// Definitionen um das Ein- und Ausschalten von LEDs zu erleichtern
#define RED					(0x01)
#define YELLOW				(0x02)
#define GREEN				(0x04)
#define LED_OFF(led)		(P4OUT |= led)    
#define LED_ON(led)			(P4OUT &= ~led)    
#define LED_TOGGLE(led)		(P4OUT ^=  led)

// Funtion, welche die bereits vorhandene Funktion wait verwendet
// um den Programmfluss zu um n Millisekunden zu verzoegern
void delay(unsigned int time_mill) {
	unsigned int i;
	for(i=0;i<=time_mill;++i){
		wait(100);
	}
}

// Diese Funktion erzeugt eine Ampelsequenz in einer Endloschleife
void ampel() {
	while(1) {
		LED_ON(YELLOW);
		delay(200);
		LED_OFF(YELLOW);
		LED_ON(RED);
		delay(300);
		LED_ON(YELLOW);
		delay(200);
		LED_OFF(RED);
		LED_OFF(YELLOW);
		LED_ON(GREEN);
		delay(300);
		LED_OFF(GREEN);
	}
}

void aufgabe13() {
	// Interrupts allgemein erlauben
	_bis_SR_register(GIE);
	// Taster an P1.0 Interruptfaehig machen
	P1IE |= 0x01;

	// Alle LEDs abschalten
	LED_OFF(RED);
	LED_OFF(YELLOW);
	LED_OFF(GREEN);

	// ACLK Teiler auf 4 setzen
	BCSCTL1	|= DIVA1;
	// Interrupts vom Watchdog erlaugen
	IE1 |= WDTIE;
	// Wachund rauslassen
	// WDTCNTCL setzt den Watchdog Timer zurueck
	// WDTSSEL waehlt den ACLK Takt als Quelle fuer den Watchdogzaehler
	// WDTTMSEL versetzt den Watchdog in den Timermode
	// Bei dieser Operation werden WDTIS0/1 und WDTHOLD auf 0 gesetzt
	// Das hat zur Folge, dass der Watchdog auf "Interrupt nach 32768
	// Takten" gestellt und aktiviert wird.
	// Neue werte muessen in Verbindung mit dem Watchdog Passwort WDTPW
	// uebergeben werden sonst wird ein RESET ausgeloest
	WDTCTL = (WDTPW+WDTTMSEL+WDTCNTCL+WDTSSEL);

	// Die Rote LED alle 0,5 sec ein- bzw. ausschalten
	while(1){
		delay(500);
		LED_OFF(RED);
		delay(500);
		LED_ON(RED);
	}
}
