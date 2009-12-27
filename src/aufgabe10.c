#include "msp430x16x.h"		// Systemdefinitionen von TI fur den MSP430F1612
#include "init.h"			// Initialisierung des Mikrocontrollers
#include "CC1100.h"			// CC1100 Funktransceiver
#include "system.h"			// Systemfunktionen MSB430H
#include "interrupts.h"		// ISR - Interrupt Service Routinen
#include "stdio.h"			// includes TI MSP430F1612 
#include "SHT11.h"			// SHT11 Temperatur- und Feuchtesensor

// Definitionen um das Ein- und Ausschalten von LEDs zu erleichtern
#define RED					(0x01)
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

void aufgabe10() {
	// Teiler für den ACLK Takt auf 8 setzen
	BCSCTL1	|= DIVA0;
	BCSCTL1	|= DIVA1;
    
    // Wachund rauslassen
    // WDTCNTCL setzt den Watchdog Timer zurueck
    // WDTSSEL wählt den ACLK Takt als Quelle fuer den Watchdogzaehler
    // Bei dieser Operation werden WDTIS0/1 und WDTHOLD auf 0 gesetzt
    // Das hat zur Folge, dass der Watchdog auf RESET nach 32768 Takten
    // gestellt und aktiviert wird.
    // Neue werte müssen in Verbindung mit dem Watchdog Passwort WDTPW
    // übergeben werden sonst wird ein RESET ausgelöst
	WDTCTL = (WDTPW+WDTCNTCL+WDTSSEL);
    
    // wenn direkt nach einem RESET ein Taster gedrückt wird, wird die
    // gruene LED eingeschaltet
    // das hilft in unserem Fall zu erkennen, wenn ein RESET vom
    // Watchdog ausgeloest wurde
	if(P1IN & 0x03) {
		LED_ON(GREEN);
	}
	
	while(1){
        // 500 msec warten dann rote und gruene LED ausschalten
		delay(500);
		LED_OFF(RED);
		LED_OFF(GREEN);
		
        // solange einer der beiden Taster gedruekt wird
		while(P1IN & 0x03) {
            
            // Watchdog zähler zuruecksetzen
            // wird diese Zeile entfernt, loest der Watchdog nach ca.
            // 8 sec einen RESET aus
			WDTCTL = (WDTPW+WDTCNTCL+WDTSSEL);
            
            // eine kurze Verzoegerung um ein moegliches Prellen des
            // Tasters abzufangen
			delay(1);
		}
		
        // Watchdog zähler zuruecksetzen
		WDTCTL = (WDTPW+WDTCNTCL+WDTSSEL);
        
        // 500 msec warten dann rote einsschalten
		delay(500);
		LED_ON(RED);
	}
}
