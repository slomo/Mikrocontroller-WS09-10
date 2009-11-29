#include "msp430x16x.h"		// Systemdefinitionen von TI fur den MSP430F1612
#include "init.h"			// Initialisierung des Mikrocontrollers
#include "CC1100.h"			// CC1100 Funktransceiver
#include "system.h"			// Systemfunktionen MSB430H
#include "interrupts.h"		// ISR - Interrupt Service Routinen
#include "stdio.h"			// includes TI MSP430F1612 
#include "SHT11.h"			// SHT11 Temperatur- und Feuchtesensor
#include "aufgabe24.h"

#define RED					(0x01)
#define YELLOW				(0x02)
#define GREEN				(0x04)
#define LED_OFF(led)	    (P4OUT |= led)    
#define LED_ON(led)      	(P4OUT &= ~led)    
#define LED_TOGGLE(led)  	(P4OUT ^=  led)

int do_a_barrel_roll = 0;

void aufgabe24() {
	char str[255];
	double ax,ay,az;
	//
	P6SEL = 0x07;
	ADC12MCTL0 = SREF_0 + INCH_0;
	ADC12MCTL1 = SREF_0 + INCH_1;
	ADC12MCTL2 = SREF_0 + INCH_2 + EOS;
	ADC12CTL1 = CONSEQ_1 + SHP;
	ADC12CTL0 = ADC12ON + ENC + ADC12SC + MSC;
	
	P5DIR |= (0x64+0x32+0x16);
	P5OUT &= ~(0x32 + 0x16); //GS1 und GS2 (empfindlichkeit) einstellen
	P5OUT |= 0x64;			//Wake up, wake up, wake up now... so tierd of wa~i~ting ...*sing*

	//Optionen zur Sekuendlichen ausloesung eines timerinterupts
	TBCTL 	= MC_1 + TASSEL_1 + ID0 + ID1;
	TBCCTL0 = CCIE; 
	TBCCR0 	= 400;
	
	ADC12IE = 0x01 + 0x02 + 0x04; // AD-Wandler Intterrup enable
	_bis_SR_register(GIE); //Interrupts zulassen

	while(1){
		if (do_a_barrel_roll == 1) {
			//ENC Toggle um eine Wandlung zu bekommen
			
			
			ADC12CTL0 |=  ADC12SC;
			ADC12CTL0 &= ~ADC12SC;
			
			do_a_barrel_roll = 0;
		}
	}
}
