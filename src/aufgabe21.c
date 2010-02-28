// Standart Headerfiles siehe aufgabe1.c
#include "aufgabe21.h"

// Diese Struktur enthaelt die  aktuelle Zeit (das struct time ist in
// aufgabe21.h definiert und enhaelt werte fuer Stunden, Minuten,
// Sekunden und Zentelsekunden
struct time now;

}

// diese Funktion gibt Zeichenketten auf UART1 aus
// sie erwartet einen Pointer auf eine Zeichenkette und deren Laenge
void output_str_on_serial(char* str, int len) {
	int i = 0;
	// solange kein Nullbyte gelesen wird oder die uebergebene Laenge
	// nicht erreicht ist, versuche die Zeichen der Zeichenkette
	// auszugeben
	while((str[i] != '\0') && (len > i)){
		// wenn der Sendebuffer leer ist, schreibe das aktuelle Zeichen
		// hinein und Inkrementiere im gleichen Zug den Zeichenzaehler
		if (U1TCTL & TXEPT) {
			U1TXBUF = str[i++];
		}
	}
}

// diese Funktion git eine Zeichenkette welche die aktuelle Uhrzeit
// sowie die aktuelle Temperatur und die aktuelle Luftfeuchtigkeit
// enthaelt auf UART1 aus
void output_data() {
	// temporaerer Puffer fuer die Zeichenkette
	char str[255];
	// lese aktuelle Werte vom Sensor
	SHT11_Read_Sensor();
	// erzeuge die Zeichenkette
	sprintf(str,"%d:%d:%d.%d - Temp: %s Hum: %s\r\n",now.hh,now.mm,now.ss,now.s,temp_char,humi_char);
	// gitb die Zeichenkette auf UART1 aus
	output_str_on_serial(str,255);
}

void aufgabe21() {
	//Initialisierung der aktuellen Zeit mit 0
	now.hh=0;
	now.mm=0;
	now.ss=0;
	now.s=0;
	
	// Timer soll hochzaehlen und den ACLK Takt mit 8fachem Teiler 
	// als Quelle verwenden
	TBCTL = MC_1 + TBSSEL_1 + ID0 + ID1;
	// Interrupts fuer Timer B aktivieren
	TBCCTL0 = CCIE;
	// Timerinterrupt nach 400 Takten auslösen also 0,1 Sekunden
	TBCCR0 = 400;
	//Interrupts allgemein zulassen
	_bis_SR_register(GIE);
	
	// Nichts weiter tun und den Rest der ISR ueberlassen 	
	while(1){	
	}
}
