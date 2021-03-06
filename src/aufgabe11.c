// Standart Headerfiles siehe aufgabe1.c

// Definitionen um das Ein- und Ausschalten von LEDs zu erleichtern
#define RED					(0x01)
#define GREEN				(0x04)
#define LED_OFF(led)	    (P4OUT |= led)    
#define LED_ON(led)      	(P4OUT &= ~led)

// define um das System zu resetten indem das Watchdogregister ohne
// das richtige Passwort beschreiben wird
// (in unserem Fall mit dem Wert 0xDEAD weil es gut passt)
#define MCU_RESET 			(WDTCTL = 0xDEAD)

// Funtion, welche die bereits vorhandene Funktion wait verwendet
// um den Programmfluss zu um n Millisekunden zu verzoeger
void delay(unsigned int time_mill) {	
	unsigned int i;
	for(i=0;i<=time_mill;++i){
		wait(100);
	}
}

void aufgabe11() {
        // gruene LED einschalten um zu signalisieren, dass der
        // Controller neu gestartet wurde
		LED_ON(GREEN);
	while(1){
        // 500 msec warten dann rote und gruene LED ausschalten
		delay(500);
		LED_OFF(RED);
		LED_OFF(GREEN);
		
        // wenn einer der Taster gedrueckt wird, loese einen RESET aus
		while(P1IN & 0x03) {
			MCU_RESET;
			delay(1);
		}
        
        // 500 msec warten dann rote LED einschalten
		delay(500);
		LED_ON(RED);
	}
}
