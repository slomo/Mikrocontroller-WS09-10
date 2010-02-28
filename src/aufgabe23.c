// Standart Headerfiles siehe aufgabe1.c
#include "aufgabe23.h"

// Definitionen um das Ein- und Ausschalten von LEDs zu erleichtern
#define RED					(0x01)
#define YELLOW				(0x02)
#define GREEN				(0x04)
#define LED_OFF(led)	    (P4OUT |= led)    
#define LED_ON(led)      	(P4OUT &= ~led)    

// Gibt an ob eine Wandlung durchgefuehrt werden soll.
int do_a_barrel_roll = 0;

void aufgabe23() {
	// Buffer fuer die Erzeugung der Ausgabe
	char str[255];
	// Zwischenspeicher fuer die gemessene Spannung
	double voltage;

	// Port6 als Eingang setzen
	P6SEL = 0x10;
	// AVCC als Positive Refferenz und AVSS als negative Refferenz
	// waehlen sowie Port6.4 als Eingang auswaehlen
	ADC12MCTL0 = SREF_0 + INCH_4;
	// Single-Channel/Single-Conversion als Modus wahlen
	// und ADC12SC bit als Sample-and-hold Quelle waehlen
	ADC12CTL1 = CONSEQ_0 + SHS_0;
	// AD Wandler einschalten
	ADC12CTL0 = ADC12ON;

	// Timer soll hochzaehlen und den ACLK Takt mit 8fachem Teiler 
	// als Quelle verwenden
	TBCTL = MC_1 + TBSSEL_1 + ID0 + ID1;
	// Interrupts fuer Timer B aktivieren
	TBCCTL0 = CCIE;
	// Timerinterrupt nach 4000 Takten auslösen also nach einer Sekunde
	TBCCR0 = 4000;
	//Interrupts allgemein zulassen
	_bis_SR_register(GIE);

	while(1){
		// Wenn eine Wandlung durchgefuehr werden soll
		if (do_a_barrel_roll == 1) {
			// ENC setzen um eine Wandlung durchzufuehren
			ADC12CTL0 = ADC12ON + ENC + ADC12SC;

			// Warten bis die Wandlung beendet ist und in dieser Zeit
			// keine neue Wandlung zulassen
			while (ADC12CTL1 & ADC12BUSY) {
				ADC12CTL0 &= ~ADC12SC;
			}

			// Ergebnis der Wandlung in die Spannung umrechnen
			voltage = ADC12MEM0*3.0/4096;
			// Zeichenkette fuer die Ausgabe erstellen
			sprintf(str,"U= %.2f V\n",voltage);

			// wenn die gemessene Spannung kleiner gleich eins ist
			// schalte die gelbe LED ein
			if(voltage <= 1){
				LED_ON(YELLOW);
				LED_OFF(GREEN);
				LED_OFF(RED);
			}
			// wenn die gemessene Spannung kleiner gleich 2  und groesser
			// als 1 ist schalte die gruene LED ein
			if(voltage <= 2 && voltage > 1 ){
				LED_OFF(YELLOW);
				LED_ON(GREEN);
				LED_OFF(RED);
			}
			// wenn die gemessene Spannung groesser als 2 ist
			// schalte die rote LED ein
			if(voltage > 2){
				LED_OFF(YELLOW);
				LED_OFF(GREEN);
				LED_ON(RED);
			}
			// ENC bit entfernen
			ADC12CTL0 &= ~ENC;
			// Zeichenkette mit der gemessenen Spannung ausgeben
			writestr(str);
			// Wandlung abgeschlossen erst wieder Wandeln, wenn dieser
			// Wert von der ISR gesetzt wird
			do_a_barrel_roll = 0;
		}
	}
}
