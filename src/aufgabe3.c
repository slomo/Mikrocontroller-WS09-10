// Standart Headerfiles siehe aufgabe1.c

// Definitionen um das Ein- und Ausschalten von LEDs zu erleichtern
#define RED					(0x01)
#define YELLOW				(0x02)
#define GREEN				(0x04)
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

void aufgabe3() {
    // wenn einer der beinen Taster gedrueckt wird, starte einen Ampelzyklus
	if(P1IN & 0x03) {
		LED_ON(YELLOW);
		delay(2000);
		LED_OFF(YELLOW);
		LED_ON(RED);
		delay(3000);
		LED_ON(YELLOW);
		delay(2000);
		LED_OFF(RED);
		LED_OFF(YELLOW);
		LED_ON(GREEN);
		delay(3000);
		LED_OFF(GREEN);

        //Ampel ist wieder aus und es kann 4 sec lang kein neuer Zyklus gestartet werden
		delay(4000);
	}
}
