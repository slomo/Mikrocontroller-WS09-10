// Standart Headerfiles siehe aufgabe1.c
#include "aufgabe26.h"
#include <math.h>

#define PI 3.14

int values[100];
int i;

void aufgabe26() {
	
	// Sinuswerte fur Ausgabe berechnen
	int i;
	for(i=0;i<100;i++) {
		values[i]=(sin(i*PI/50.0)+1.5)*4096/3;
	}
	
	// DMA fuer Gruppentransfer vorbereiten und Timer als Trigger nutzen
	DMACTL0 = DMA0TSEL_8;
	DMA0CTL = DMASRCINCR_3 + DMADT_4 + DMAEN;
    // Groesse des Quellspeichers
	DMA0SZ = 100;
    // Quellspeicher
	DMA0SA = (unsigned int) values;
    // Zielspeicher
	DMA0DA = (unsigned int ) &DAC12_1DAT;
	
	// DAC vorbereiten
	DAC12_1CTL = DAC12SREF0 + DAC12SREF1 + DAC12IR + DAC12AMP1;
	
	//Timer und Output einstellen
	P6SEL = 0x80;
	TBCTL 	= MC_1 + TASSEL_2 + ID_3;
	//TBCCTL0 = CCIE; 
	TBCCR0 	= 93;

    // Interrupts zulassen
	_bis_SR_register(GIE);
	
	while(1) {
	}

}
