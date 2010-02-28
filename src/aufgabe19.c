// Standart Headerfiles siehe aufgabe1.c
#include "aufgabe19.h"

// Diese Strukturen enthalten die beiden Schaltzeiten und die 
// aktuelle Zeit (das struct ist in aufgabe19.h definiert und enhaelt
// werte fuer Stunden, Minuten, Sekunden und Zentelsekunden
struct time t1,t2,now;

// Diese Funktion vergleicht 2 time structs und liefert 1 zurueck wenn
// beide structs die selbe Zeit enthalten
int timecmp(struct time *s1, struct time *s2) {
	return ((s1->s == s2->s) && (s1->ss == s2->ss) && (s1->mm == s2->mm) && (s1->hh == s2->hh));
}

void aufgabe19() {
	//Initialisierung der aktuellen Zeit mit 0
	now.hh=0;
	now.mm=0;
	now.ss=0;
	now.s=0;

	//Initialisierung der ersten Schaltzeit (nach 5 Sekunden schalten)
	t1.hh=0;
	t1.mm=0;
	t1.ss=5;
	t1.s=0;

	//Initialisierung der zweiten Schaltzeit (nach 10 Sekunden schalten)
	t2.hh=0;
	t2.mm=0;
	t2.ss=10;
	t2.s=0;

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
