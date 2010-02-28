// Standart Headerfiles siehe aufgabe1.c

// Definitionen um das Ein- und Ausschalten der roten LED zu erleichtern
#define RED					(0x01)
#define LED_OFF(led)	    (P4OUT |= led)
#define LED_ON(led)      	(P4OUT &= ~led)

// Funtion, welche die bereits vorhandene Funktion wait verwendet
// um den Programmfluss zu um n Millisekunden zu verzoegern
void delay(unsigned int time_mill) {	
	unsigned int i;
	for(i=0;i<=time_mill;++i){
		wait(100);
	}
}

void aufgabe9() {
	// Teiler fuer den ACLK Takt auf 8 setzen
	BCSCTL1	|= DIVA0;
	BCSCTL1	|= DIVA1;
    
	// Wachund rauslassen
    // WDTCNTCL setzt den Watchdog Timer zurueck
    // WDTSSEL waehlt den ACLK Takt als Quelle fuer den Watchdogzaehler
    // Bei dieser Operation werden WDTIS0/1 und WDTHOLD auf 0 gesetzt
    // Das hat zur Folge, dass der Watchdog auf RESET nach 32768 Takten
    // gestellt und aktiviert wird.
    // Neue werte muessen in Verbindung mit dem Watchdog Passwort WDTPW
    // uebergeben werden sonst wird ein RESET ausgeloest
    WDTCTL = (WDTPW+WDTCNTCL+WDTSSEL);
    
	while(1){
        // 500 msec warten dann rote LED ausschalten
		delay(500);
		LED_OFF(RED);
        
        // Watchdog zaehler zuruecksetzen um einen RESET zu vermeiden
        // waehre diese Zeile nicht da, wuerde die rote LED nie
        // eingeschaltet, da vor erreichen der Anweisung ein RESET
        // durch den Watchdog erfolgen wuerde
		WDTCTL = (WDTPW+WDTCNTCL+WDTSSEL);
        
        // 500 msec warten dann rote LED einschalten
		delay(500);
		LED_ON(RED);
	}
}
