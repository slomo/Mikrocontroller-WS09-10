#include "msp430x16x.h"        // Systemdefinitionen von TI für den MSP430F1612
#include "../init.h"            // Initialisierung des Mikrocontrollers
#include "../CC1100.h"            // CC1100 Funktransceiver
#include "../system.h"            // Systemfunktionen MSB430H
#include "interrupts.h"        // ISR - Interrupt Service Routinen
#include "stdio.h"            // includes TI MSP430F1612 
#include "../SHT11.h"            // SHT11 Temperatur- und Feuchtesensor
#include "project.h"

int ready;

void delay(unsigned int time_mill) {	
	unsigned int i;
	for(i=0;i<=time_mill;++i){
		wait(100);
	}
}

void project(){
	ready = 0;
	
	// Funk vorbereiten
	initUART0_SPI();
	initCC1100();
	setUid(1);
	switchFreq(7);
	
	P1IE |= 0x03;
	P1IES &= ~0x03;
	
	LED_OFF(RED);
	LED_OFF(YELLOW);
	LED_OFF(GREEN);
	
	while (1) {
		if (ready == 2) {
			LED_ON(RED);
			delay(350);
			LED_OFF(RED);
			LED_ON(YELLOW);
			delay(350);
			LED_OFF(YELLOW);
			LED_ON(GREEN);
			delay(350);
			LED_OFF(GREEN);
			LED_ON(YELLOW);
			delay(350);
			LED_OFF(YELLOW);
			LED_ON(RED);
			delay(350);
		}
		else if (ready) {
			LED_ON(RED);
			LED_ON(YELLOW);
			LED_ON(GREEN);
			delay(500);
			LED_OFF(RED);
			LED_OFF(YELLOW);
			LED_OFF(GREEN);
			delay(500);
		}
	}
}	

